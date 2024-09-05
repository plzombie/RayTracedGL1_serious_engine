// Copyright (c) 2020-2021 Sultim Tsyrendashiev
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Framebuffers.h"

using namespace RTGL1;

#include "Swapchain.h"
#include "Utils.h"
#include "CmdLabel.h"

#include <vector>

static_assert(MAX_FRAMES_IN_FLIGHT == FRAMEBUFFERS_HISTORY_LENGTH, "Framebuffers class logic must be changed if history length is not equal to max frames in flight");

FramebufferImageIndex Framebuffers::FrameIndexToFBIndex(FramebufferImageIndex framebufferImageIndex, uint32_t frameIndex)
{
    assert(frameIndex < FRAMEBUFFERS_HISTORY_LENGTH);
    assert(framebufferImageIndex >= 0 && framebufferImageIndex < ShFramebuffers_Count);

    // if framubuffer with given index can be swapped,
    // use one that is currently in use
    if (ShFramebuffers_Bindings[framebufferImageIndex] != ShFramebuffers_BindingsSwapped[framebufferImageIndex])
    {
        return (FramebufferImageIndex)(framebufferImageIndex + frameIndex);
    }

    return framebufferImageIndex;
}

Framebuffers::Framebuffers(
    VkDevice _device, 
    std::shared_ptr<MemoryAllocator> _allocator, 
    std::shared_ptr<CommandBufferManager> _cmdManager)
: 
    device(_device),
    bilinearSampler(VK_NULL_HANDLE),
    nearestSampler(VK_NULL_HANDLE),
    allocator(std::move(_allocator)),
    cmdManager(std::move(_cmdManager)),
    currentResolution{},
    descSetLayout(VK_NULL_HANDLE),
    descPool(VK_NULL_HANDLE),
    descSets{}
{
    images.resize(ShFramebuffers_Count);
    imageMemories.resize(ShFramebuffers_Count);
    imageViews.resize(ShFramebuffers_Count);

    CreateDescriptors();
    CreateSamplers();
}

Framebuffers::~Framebuffers()
{
    DestroyImages();

    vkDestroySampler(device, nearestSampler, nullptr);
    vkDestroySampler(device, bilinearSampler, nullptr);
    
    vkDestroyDescriptorPool(device, descPool, nullptr);
    vkDestroyDescriptorSetLayout(device, descSetLayout, nullptr);
}

void Framebuffers::CreateDescriptors()
{
    VkResult r;

    const uint32_t allBindingsCount = ShFramebuffers_Count * 2;
    const uint32_t samplerBindingOffset = ShFramebuffers_Count;

    std::vector<VkDescriptorSetLayoutBinding> bindings(allBindingsCount);
    uint32_t bndCount = 0;

    // gimage2D
    for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
    {
        VkDescriptorSetLayoutBinding &bnd = bindings[bndCount];

        // after swapping bindings, cur will become prev, and prev - cur
        bnd.binding = ShFramebuffers_Bindings[i];
        bnd.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bnd.descriptorCount = 1;
        bnd.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        bndCount++;
    }

    // gsampler2D
    for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
    {
        VkDescriptorSetLayoutBinding &bnd = bindings[bndCount];

        if (ShFramebuffers_Sampler_Bindings[i] == FB_SAMPLER_INVALID_BINDING)
        {
            continue;
        }

        // after swapping bindings, cur will become prev, and prev - cur
        bnd.binding = ShFramebuffers_Sampler_Bindings[i];
        bnd.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bnd.descriptorCount = 1;
        bnd.stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        bndCount++;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bndCount;
    layoutInfo.pBindings = bindings.data();

    r = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descSetLayout);
    VK_CHECKERROR(r);

    SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, descSetLayout, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT, "Framebuffers Desc set layout");

    std::array<VkDescriptorPoolSize, 2> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[0].descriptorCount = allBindingsCount * FRAMEBUFFERS_HISTORY_LENGTH;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_SAMPLER;
    poolSizes[1].descriptorCount = allBindingsCount * FRAMEBUFFERS_HISTORY_LENGTH;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = FRAMEBUFFERS_HISTORY_LENGTH;

    r = vkCreateDescriptorPool(device, &poolInfo, nullptr, &descPool);
    VK_CHECKERROR(r);

    SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, descPool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, "Framebuffers Desc pool");

    for (uint32_t i = 0; i < FRAMEBUFFERS_HISTORY_LENGTH; i++)
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &descSetLayout;

        r = vkAllocateDescriptorSets(device, &allocInfo, &descSets[i]);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, descSets[i], VK_OBJECT_TYPE_DESCRIPTOR_SET, "Framebuffers Desc set");
    }
}

void RTGL1::Framebuffers::CreateSamplers()
{
    VkSamplerCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.addressModeU = info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.mipLodBias = 0.0f;
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 0;
    info.compareEnable = VK_FALSE;
    info.minLod = 0.0f;
    info.maxLod = VK_LOD_CLAMP_NONE;
    info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;

    {
        info.minFilter = info.magFilter = VK_FILTER_NEAREST;

        VkResult r = vkCreateSampler(device, &info, nullptr, &nearestSampler);
        VK_CHECKERROR(r);
    }

    {
        info.minFilter = info.magFilter = VK_FILTER_LINEAR;

        VkResult r = vkCreateSampler(device, &info, nullptr, &bilinearSampler);
        VK_CHECKERROR(r);
    }
}

bool RTGL1::Framebuffers::PrepareForSize(ResolutionState resolutionState)
{
    if (currentResolution == resolutionState)
    {
        return false;
    }

    vkDeviceWaitIdle(device);

    DestroyImages();
    CreateImages(resolutionState);

    assert(currentResolution == resolutionState);
    return true;
}

void RTGL1::Framebuffers::BarrierOne(VkCommandBuffer cmd, uint32_t frameIndex, FramebufferImageIndex framebufImageIndex, BarrierType barrierTypeFrom)
{
    FramebufferImageIndex fs[] = { framebufImageIndex };
    BarrierMultiple(cmd, frameIndex, fs, barrierTypeFrom);
}

void Framebuffers::PresentToSwapchain(
    VkCommandBuffer cmd, uint32_t frameIndex, const std::shared_ptr<Swapchain> &swapchain,
    FramebufferImageIndex framebufImageIndex, VkFilter filter)
{
    CmdLabel label(cmd, "Present to swapchain");

    BarrierOne(cmd, frameIndex, framebufImageIndex);

    VkExtent2D srcExtent = GetFramebufSize(ShFramebuffers_Flags[framebufImageIndex], currentResolution);

    swapchain->BlitForPresent(
        cmd, GetImage(framebufImageIndex, frameIndex),
        srcExtent.width, srcExtent.height, filter, VK_IMAGE_LAYOUT_GENERAL);
}

RTGL1::FramebufferImageIndex RTGL1::Framebuffers::BlitForEffects(VkCommandBuffer cmd, uint32_t frameIndex, FramebufferImageIndex framebufImageIndex, VkFilter filter)
{
    FramebufferImageIndex src = FrameIndexToFBIndex(framebufImageIndex, frameIndex);
    FramebufferImageIndex dst;

    switch (src)
    {
        case FB_IMAGE_INDEX_FINAL:         dst = FrameIndexToFBIndex(FB_IMAGE_INDEX_UPSCALED_PING, frameIndex); break;
        case FB_IMAGE_INDEX_UPSCALED_PING: dst = FrameIndexToFBIndex(FB_IMAGE_INDEX_UPSCALED_PONG, frameIndex); break;
        case FB_IMAGE_INDEX_UPSCALED_PONG: dst = FrameIndexToFBIndex(FB_IMAGE_INDEX_UPSCALED_PING, frameIndex); break;
        default: assert(0); return FB_IMAGE_INDEX_UPSCALED_PING;
    }

    VkImage srcImage = images[src];
    VkImage dstImage = images[dst];

    VkExtent2D srcExtent = GetFramebufSize(ShFramebuffers_Flags[src], currentResolution);
    VkExtent2D dstExtent = GetFramebufSize(ShFramebuffers_Flags[dst], currentResolution);

    // if source has almost the same size as the surface, then use nearest blit
    if (std::abs((int)srcExtent.width  - (int)dstExtent.width) < 8 &&
        std::abs((int)srcExtent.height - (int)dstExtent.height) < 8)
    {
        filter = VK_FILTER_NEAREST;
    }


    VkImageBlit region = {};

    region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    region.srcOffsets[0] = { 0, 0, 0 };
    region.srcOffsets[1] = { static_cast<int32_t>(srcExtent.width), static_cast<int32_t>(srcExtent.height), 1 };

    region.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
    region.dstOffsets[0] = { 0, 0, 0 };
    region.dstOffsets[1] = { static_cast<int32_t>(dstExtent.width), static_cast<int32_t>(dstExtent.height), 1 };

    

    // set layout for blit
    Utils::BarrierImage(
        cmd, srcImage,
        VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

    Utils::BarrierImage(
        cmd, dstImage,
        0, VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vkCmdBlitImage(
        cmd, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &region, filter);

    // restore layouts
    Utils::BarrierImage(
        cmd, srcImage,
        VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

    Utils::BarrierImage(
        cmd, dstImage,
        VK_ACCESS_TRANSFER_WRITE_BIT, 0,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);

    return dst;
}

VkDescriptorSet Framebuffers::GetDescSet(uint32_t frameIndex) const
{
    return descSets[frameIndex];
}

VkDescriptorSetLayout Framebuffers::GetDescSetLayout() const
{
    return descSetLayout;
}

VkImage Framebuffers::GetImage(FramebufferImageIndex framebufferImageIndex, uint32_t frameIndex) const
{
    framebufferImageIndex = FrameIndexToFBIndex(framebufferImageIndex, frameIndex);
    return images[framebufferImageIndex];
}

VkImageView Framebuffers::GetImageView(FramebufferImageIndex framebufferImageIndex, uint32_t frameIndex) const
{
    framebufferImageIndex = FrameIndexToFBIndex(framebufferImageIndex, frameIndex);
    return imageViews[framebufferImageIndex];
}

void RTGL1::Framebuffers::GetImageHandles(FramebufferImageIndex framebufferImageIndex, uint32_t frameIndex, VkImage *pOutImage, VkImageView *pOutView, VkFormat *pOutFormat) const
{
    framebufferImageIndex = FrameIndexToFBIndex(framebufferImageIndex, frameIndex);

    if (pOutImage)
    {
        *pOutImage = images[framebufferImageIndex];
    }

    if (pOutView)
    {
        *pOutView = imageViews[framebufferImageIndex];
    }
    
    if (pOutFormat)
    {
        *pOutFormat = ShFramebuffers_Formats[framebufferImageIndex];
    }
}

VkExtent2D RTGL1::Framebuffers::GetFramebufSize(FramebufferImageFlags flags, const ResolutionState &resolutionState)
{
    if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_UPSCALED_SIZE)
    {
        return { resolutionState.upscaledWidth, resolutionState.upscaledHeight };
    }

    if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_SINGLE_PIXEL_SIZE)
    {
        return { 1,1 };
    }

    int downscale = 1;

    if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_FORCE_SIZE_1_2)
    {
        downscale = 2;
    }
    else if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_FORCE_SIZE_1_3)
    {
        downscale = 3;
    }
    else if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_FORCE_SIZE_1_4)
    {
        downscale = 4;
    }
    else if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_FORCE_SIZE_1_8)
    {
        downscale = 8;
    }
    else if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_FORCE_SIZE_1_16)
    {
        downscale = 16;
    }
    else if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_FORCE_SIZE_1_32)
    {
        downscale = 32;
    }
    else
    {
        return { resolutionState.renderWidth, resolutionState.renderHeight };
    }

    VkExtent2D extent;

    extent.width  = (resolutionState.renderWidth  + 1) / downscale;
    extent.height = (resolutionState.renderHeight + 1) / downscale;

    extent.width  = std::max(1u, extent.width);
    extent.height = std::max(1u, extent.height);

    return extent;
}

void Framebuffers::CreateImages(ResolutionState resolutionState)
{
    VkResult r;

    VkCommandBuffer cmd = cmdManager->StartGraphicsCmd();

    for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
    {
        VkFormat format = ShFramebuffers_Formats[i];
        FramebufferImageFlags flags = ShFramebuffers_Flags[i];

        VkExtent2D extent = GetFramebufSize(flags, resolutionState);

        // create image
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.format = format;
        imageInfo.extent = { extent.width, extent.height, 1};
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.usage = 
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
            VK_IMAGE_USAGE_STORAGE_BIT |
            VK_IMAGE_USAGE_SAMPLED_BIT; 
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_IS_ATTACHMENT)
        {
            imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        }

        if (flags & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_USAGE_TRANSFER)
        {
            imageInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        r = vkCreateImage(device, &imageInfo, nullptr, &images[i]);
        VK_CHECKERROR(r);

        // allocate dedicated memory
        VkMemoryRequirements memReqs;
        vkGetImageMemoryRequirements(device, images[i], &memReqs);

        imageMemories[i] = allocator->AllocDedicated(memReqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, MemoryAllocator::AllocType::DEFAULT, ShFramebuffers_DebugNames[i]);

        r = vkBindImageMemory(device, images[i], imageMemories[i], 0);
        VK_CHECKERROR(r);

        // create image view
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange = {};
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.image = images[i];
        r = vkCreateImageView(device, &viewInfo, nullptr, &imageViews[i]);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, images[i], VK_OBJECT_TYPE_IMAGE, ShFramebuffers_DebugNames[i]);
        SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, imageViews[i], VK_OBJECT_TYPE_IMAGE_VIEW, ShFramebuffers_DebugNames[i]);

        // to general layout
        Utils::BarrierImage(
            cmd, images[i],
            0, VK_ACCESS_SHADER_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
    }

    // image creation happens rarely
    cmdManager->Submit(cmd);
    cmdManager->WaitGraphicsIdle();

    currentResolution = resolutionState;


    UpdateDescriptors();

    NotifySubscribersAboutResize(resolutionState);
}

void Framebuffers::UpdateDescriptors()
{
    const uint32_t allBindingsCount = ShFramebuffers_Count * 2;
    const uint32_t samplerBindingOffset = ShFramebuffers_Count;

    std::vector<VkDescriptorImageInfo> imageInfos(allBindingsCount);

    // gimage2D
    for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
    {
        imageInfos[i].sampler = VK_NULL_HANDLE;
        imageInfos[i].imageView = imageViews[i];
        imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    }

    // gsampler2D
    for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
    {    
        // texelFetch should be used to get a specific texel,
        // and texture/textureLod for sampling with bilinear interpolation

        bool useBilinear = ShFramebuffers_Flags[i] & FB_IMAGE_FLAGS_FRAMEBUF_FLAGS_BILINEAR_SAMPLER;

        imageInfos[samplerBindingOffset + i].sampler = useBilinear ? bilinearSampler : nearestSampler;
        imageInfos[samplerBindingOffset + i].imageView = imageViews[i];
        imageInfos[samplerBindingOffset + i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    }

    std::vector<VkWriteDescriptorSet> writes(allBindingsCount * FRAMEBUFFERS_HISTORY_LENGTH);
    uint32_t wrtCount = 0;

    for (uint32_t k = 0; k < FRAMEBUFFERS_HISTORY_LENGTH; k++)
    {
        // gimage2D
        for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
        {
            auto &wrt = writes[wrtCount];

            wrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            wrt.dstSet = descSets[k];
            wrt.dstBinding = k == 0 ?
                ShFramebuffers_Bindings[i] :
                ShFramebuffers_BindingsSwapped[i];
            wrt.dstArrayElement = 0;
            wrt.descriptorCount = 1;
            wrt.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            wrt.pImageInfo = &imageInfos[i];

            wrtCount++;
        }

        // gsampler2D
        for (uint32_t i = 0; i < ShFramebuffers_Count; i++)
        {
            auto &wrt = writes[wrtCount];

            uint32_t dstBinding = k == 0 ?
                ShFramebuffers_Sampler_Bindings[i] :
                ShFramebuffers_Sampler_BindingsSwapped[i];

            if (dstBinding == FB_SAMPLER_INVALID_BINDING)
            {
                continue;
            }

            wrt.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            wrt.dstSet = descSets[k];
            wrt.dstBinding = dstBinding;
            wrt.dstArrayElement = 0;
            wrt.descriptorCount = 1;
            wrt.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            wrt.pImageInfo = &imageInfos[samplerBindingOffset + i];

            wrtCount++;
        }
    }

    vkUpdateDescriptorSets(device, wrtCount, writes.data(), 0, nullptr);
}

void Framebuffers::DestroyImages()
{
    for (auto &i : images)
    {
        if (i != VK_NULL_HANDLE)
        {
            vkDestroyImage(device, i, nullptr);
            i = VK_NULL_HANDLE;
        }
    }

    for (auto &m : imageMemories)
    {
        if (m != VK_NULL_HANDLE)
        {
            vkFreeMemory(device, m, nullptr);
            m = VK_NULL_HANDLE;
        }
    }

    for (auto &v : imageViews)
    {
        if (v != VK_NULL_HANDLE)
        {
            vkDestroyImageView(device, v, nullptr);
            v = VK_NULL_HANDLE;
        }
    }
}

void Framebuffers::NotifySubscribersAboutResize(const ResolutionState &resolutionState)
{
    for (auto &ws : subscribers)
    {
        if (auto s = ws.lock())
        {
            s->OnFramebuffersSizeChange(resolutionState);
        }
    }
}

void Framebuffers::Subscribe(std::shared_ptr<IFramebuffersDependency> subscriber)
{
    subscribers.emplace_back(subscriber);
}

void Framebuffers::Unsubscribe(const IFramebuffersDependency *subscriber)
{
    subscribers.remove_if([subscriber] (const std::weak_ptr<IFramebuffersDependency> &ws)
    {
        if (const auto s = ws.lock())
        {
            return s.get() == subscriber;
        }

        return true;
    });
}
