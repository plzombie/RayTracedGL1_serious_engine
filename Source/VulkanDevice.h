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

#include <RTGL1/RTGL1.h>

#include <memory>

#include "Common.h"

#include "CommandBufferManager.h"
#include "PhysicalDevice.h"
#include "Scene.h"
#include "Swapchain.h"
#include "Queues.h"
#include "GlobalUniform.h"
#include "PathTracer.h"
#include "Rasterizer.h"
#include "Framebuffers.h"
#include "MemoryAllocator.h"
#include "TextureManager.h"
#include "BlueNoise.h"
#include "ImageComposition.h"
#include "Tonemapping.h"
#include "CubemapManager.h"
#include "Denoiser.h"
#include "UserFunction.h"

namespace RTGL1
{

class VulkanDevice
{
public:
    explicit VulkanDevice(const RgInstanceCreateInfo *pInfo);
    ~VulkanDevice();

    VulkanDevice(const VulkanDevice& other) = delete;
    VulkanDevice(VulkanDevice&& other) noexcept = delete;
    VulkanDevice& operator=(const VulkanDevice& other) = delete;
    VulkanDevice& operator=(VulkanDevice&& other) noexcept = delete;

    void UploadGeometry(const RgGeometryUploadInfo *pUploadInfo);
    void UpdateGeometryTransform(const RgUpdateTransformInfo *pUpdateInfo);
    void UpdateGeometryTexCoords(const RgUpdateTexCoordsInfo *pUpdateInfo);

    void UploadRasterizedGeometry(const RgRasterizedGeometryUploadInfo *pUploadInfo,
                                      const float *pViewProjection, const RgViewport *pViewport);

    void SubmitStaticGeometries();
    void StartNewStaticScene();

    void UploadLight(const RgDirectionalLightUploadInfo *pLightInfo);
    void UploadLight(const RgSphericalLightUploadInfo *pLightInfo);
    void UploadLight(const RgSpotlightUploadInfo *pLightInfo);

    void CreateStaticMaterial(const RgStaticMaterialCreateInfo *pCreateInfo, RgMaterial *pResult);
    void CreateAnimatedMaterial(const RgAnimatedMaterialCreateInfo *pCreateInfo, RgMaterial *pResult);
    void ChangeAnimatedMaterialFrame(RgMaterial animatedMaterial, uint32_t frameIndex);
    void CreateDynamicMaterial(const RgDynamicMaterialCreateInfo *pCreateInfo, RgMaterial *pResult);
    void UpdateDynamicMaterial(const RgDynamicMaterialUpdateInfo *pUpdateInfo);
    void DestroyMaterial(RgMaterial material);

    void CreateSkyboxCubemap(const RgCubemapCreateInfo *pCreateInfo, RgCubemap *pResult);
    void DestroyCubemap(RgCubemap cubemap);


    void StartFrame(const RgStartFrameInfo *pStartInfo);
    void DrawFrame(const RgDrawFrameInfo *pFrameInfo);


    void Print(const char *pMessage) const;

private:
    void CreateInstance(const RgInstanceCreateInfo &info);
    void CreateDevice();
    void CreateSyncPrimitives();
    static VkSurfaceKHR GetSurfaceFromUser(VkInstance instance, const RgInstanceCreateInfo &info);
    void ValidateCreateInfo(const RgInstanceCreateInfo *pInfo);

    void DestroyInstance();
    void DestroyDevice();
    void DestroySyncPrimitives();

    void FillUniform(ShGlobalUniform *gu, const RgDrawFrameInfo &drawInfo) const;

    VkCommandBuffer BeginFrame(const RgStartFrameInfo &startInfo);
    void Render(VkCommandBuffer cmd, const RgDrawFrameInfo &drawInfo);
    void EndFrame(VkCommandBuffer cmd);

private:
    VkInstance          instance;
    VkDevice            device;
    VkSurfaceKHR        surface;

    // [0..MAX_FRAMES_IN_FLIGHT-1]
    uint32_t            currentFrameIndex;
    VkCommandBuffer     currentFrameCmd;
    // incremented every frame
    uint32_t            frameId;

    VkFence             frameFences[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore         imageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
    VkSemaphore         renderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];

    std::shared_ptr<PhysicalDevice>         physDevice;
    std::shared_ptr<Queues>                 queues;
    std::shared_ptr<Swapchain>              swapchain;

    std::shared_ptr<MemoryAllocator>        memAllocator;

    std::shared_ptr<CommandBufferManager>   cmdManager;

    std::shared_ptr<Framebuffers>           framebuffers;

    std::shared_ptr<GlobalUniform>          uniform;
    std::shared_ptr<Scene>                  scene;

    std::shared_ptr<ShaderManager>          shaderManager;
    std::shared_ptr<RayTracingPipeline>     rtPipeline;
    std::shared_ptr<PathTracer>             pathTracer;
    std::shared_ptr<Rasterizer>             rasterizer;
    std::shared_ptr<Denoiser>               denoiser;
    std::shared_ptr<Tonemapping>            tonemapping;
    std::shared_ptr<ImageComposition>       imageComposition;

    std::shared_ptr<SamplerManager>         samplerManager;
    std::shared_ptr<BlueNoise>              blueNoise;
    std::shared_ptr<TextureManager>         textureManager;
    std::shared_ptr<CubemapManager>         cubemapManager;

    bool                                    enableValidationLayer;
    VkDebugUtilsMessengerEXT                debugMessenger;
    std::unique_ptr<UserPrint>              userPrint;
    std::shared_ptr<UserFileLoad>           userFileLoad;

    VertexBufferProperties                  vbProperties;

    double                                  previousFrameTime;
    double                                  currentFrameTime;

    bool                                    disableRayTracedSkybox;
};

}
