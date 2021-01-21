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

#pragma once

#include <vector>

#include "Common.h"
#include "MemoryAllocator.h"
#include "RTGL1/RTGL1.h"

// Uploads texture data to device.
class TextureUploader
{
public:
    struct UploadResult
    {
        bool wasUploaded;
        VkImage image;
        VkImageView view;
    };

public:
    TextureUploader(VkDevice device, std::shared_ptr<MemoryAllocator> memAllocator);
    ~TextureUploader();

    TextureUploader(const TextureUploader &other) = delete;
    TextureUploader(TextureUploader &&other) noexcept = delete;
    TextureUploader &operator=(const TextureUploader &other) = delete;
    TextureUploader &operator=(TextureUploader &&other) noexcept = delete;

    // Clear staging buffer for given frame index.
    void ClearStaging(uint32_t frameIndex);

    UploadResult UploadStaticImage(VkCommandBuffer cmd, uint32_t frameIndex, const void *data, const RgExtent2D &size, const char *debugName);

    void DestroyImage(VkImage image, VkImageView view);

private:
    static uint32_t GetMipmapCount(const RgExtent2D &size);

private:
    VkDevice device;

    std::shared_ptr<MemoryAllocator> memAllocator;

    // Staging buffers that were used for uploading must be destroyed
    // on the frame with same index when it'll be certainly not in use.
    std::vector<VkBuffer> stagingToFree[MAX_FRAMES_IN_FLIGHT];

};