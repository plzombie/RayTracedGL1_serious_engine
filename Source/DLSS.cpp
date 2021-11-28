// Copyright (c) 2021 Sultim Tsyrendashiev
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

#include "DLSS.h"

#include <nvsdk_ngx_helpers_vk.h>
#include <nvsdk_ngx_helpers.h>

#include "RTGL1/RTGL1.h"
#include "CmdLabel.h"
#include "RenderResolutionHelper.h"

static void PrintCallback(const char *message, NVSDK_NGX_Logging_Level loggingLevel, NVSDK_NGX_Feature sourceComponent)
{
    printf("DLSS (sourceComponent = %d): %s \n", sourceComponent,  message);
}

// TODO: DLSS: add LOG INFO / ERROR

RTGL1::DLSS::DLSS(
    VkInstance _instance,
    VkDevice _device,
    VkPhysicalDevice _physDevice,
    bool _enableDebug)
:
    device(_device),
    isInitialized(false),
    pParams(nullptr),
    pDlssFeature(nullptr),
    prevDlssFeatureValues{}
{
    isInitialized = TryInit(_instance, _device, _physDevice, _enableDebug);

    if (!CheckSupport())
    {
        Destroy();
    }
}

bool RTGL1::DLSS::TryInit(VkInstance instance, VkDevice device, VkPhysicalDevice physDevice, bool enableDebug)
{
    NVSDK_NGX_Result r;

    wchar_t *debugPath = L"dev/";
    wchar_t *releasePath = L"rel/";

    NVSDK_NGX_PathListInfo debugPathsInfo = {};
    debugPathsInfo.Path = &debugPath;
    debugPathsInfo.Length = 1;

    NVSDK_NGX_PathListInfo releasePathsInfo = {};
    releasePathsInfo.Path = &releasePath;
    releasePathsInfo.Length = 1;

    NGSDK_NGX_LoggingInfo debugLogInfo = {};
    debugLogInfo.LoggingCallback = &PrintCallback;
    debugLogInfo.MinimumLoggingLevel = NVSDK_NGX_Logging_Level::NVSDK_NGX_LOGGING_LEVEL_VERBOSE;

    NGSDK_NGX_LoggingInfo releaseLogInfo = {};

    NVSDK_NGX_FeatureCommonInfo commonInfo = {};
    commonInfo.PathListInfo = enableDebug ? debugPathsInfo : releasePathsInfo;
    commonInfo.LoggingInfo = enableDebug ? debugLogInfo : releaseLogInfo;

    r = NVSDK_NGX_VULKAN_Init_with_ProjectID(
        "",
        NVSDK_NGX_EngineType::NVSDK_NGX_ENGINE_TYPE_CUSTOM, RG_RTGL_VERSION_API, L"DLSSTemp/", instance, physDevice, device, &commonInfo);

    if (NVSDK_NGX_SUCCEED(r))
    {
        return false;
    }

    r = NVSDK_NGX_VULKAN_GetCapabilityParameters(&pParams);
    if (NVSDK_NGX_FAILED(r))
    {
        NVSDK_NGX_VULKAN_Shutdown();
        pParams = nullptr;
        
        return false;
    }

    return true;
}

bool RTGL1::DLSS::CheckSupport() const
{
    int needsUpdatedDriver = 0;
    unsigned int minDriverVersionMajor = 0;
    unsigned int minDriverVersionMinor = 0;

    NVSDK_NGX_Result r_upd = pParams->Get(NVSDK_NGX_Parameter_SuperSampling_NeedsUpdatedDriver, &needsUpdatedDriver);
    NVSDK_NGX_Result r_mjr = pParams->Get(NVSDK_NGX_Parameter_SuperSampling_MinDriverVersionMajor, &minDriverVersionMajor);
    NVSDK_NGX_Result r_mnr = pParams->Get(NVSDK_NGX_Parameter_SuperSampling_MinDriverVersionMinor, &minDriverVersionMinor);

    if (NVSDK_NGX_SUCCEED(r_upd) && NVSDK_NGX_SUCCEED(r_mjr) && NVSDK_NGX_SUCCEED(r_mnr))
    {
        if (needsUpdatedDriver)
        {
            // LOG ERROR("NVIDIA DLSS cannot be loaded due to outdated driver.
            //            Min Driver Version required : minDriverVersionMajor.minDriverVersionMinor");
            return false;
        }
        else
        {
            // LOG INFO("NIDIA DLSS Minimum driver version was reported as: minDriverVersionMajor.minDriverVersionMinor");
            assert(0);
        }
    }
    else
    {
        // LOG INFO("NVIDIA DLSS Minimum driver version was not reported.");
        assert(0);
    }


    int isDlssSupported = 0;
    int featureInitResult;
    NVSDK_NGX_Result r;
    
    r = pParams->Get(NVSDK_NGX_Parameter_SuperSampling_Available, &isDlssSupported);
    if (NVSDK_NGX_FAILED(r) || !isDlssSupported)
    {
        // more details about what failed (per feature init result)
        r = NVSDK_NGX_Parameter_GetI(pParams, NVSDK_NGX_Parameter_SuperSampling_FeatureInitResult, &featureInitResult);
        if (NVSDK_NGX_FAILED(r))
        {
            // LOG ERROR("NVIDIA DLSS not available on this hardward/platform., FeatureInitResult = 0x%08x, info: %ls", featureInitResult, GetNGXResultAsString(featureInitResult));
        }

        return false;
    }

    return true;
}

RTGL1::DLSS::~DLSS()
{
    Destroy();
}

void RTGL1::DLSS::DestroyDlssFeature()
{
    assert(pDlssFeature != nullptr);

    vkDeviceWaitIdle(device);

    NVSDK_NGX_Result r = NVSDK_NGX_VULKAN_ReleaseFeature(pDlssFeature);
    pDlssFeature = nullptr;

    if (NVSDK_NGX_FAILED(r))
    {
        // LOG ERROR("Failed to NVSDK_NGX_VULKAN_ReleaseFeature, code = 0x%08x, info: %ls", r, GetNGXResultAsString(t));
    }
}

void RTGL1::DLSS::Destroy()
{
    if (isInitialized)
    {
        vkDeviceWaitIdle(device);

        if (pDlssFeature != nullptr)
        {
            DestroyDlssFeature();
        }

        NVSDK_NGX_VULKAN_DestroyParameters(pParams);
        NVSDK_NGX_VULKAN_Shutdown();

        pParams = nullptr;
        isInitialized = false;
    }
}

bool RTGL1::DLSS::IsDlssAvailable() const
{
    return isInitialized && pParams != nullptr && pDlssFeature != nullptr;
}

static NVSDK_NGX_PerfQuality_Value ToNGXPerfQuality(RgRenderResolutionMode mode)
{
    switch (mode)
    {
        case RG_RENDER_RESOLUTION_MODE_ULTRA_PERFORMANCE:
            return NVSDK_NGX_PerfQuality_Value::NVSDK_NGX_PerfQuality_Value_UltraPerformance;
        case RG_RENDER_RESOLUTION_MODE_PERFORMANCE:
            return NVSDK_NGX_PerfQuality_Value::NVSDK_NGX_PerfQuality_Value_MaxPerf;
        case RG_RENDER_RESOLUTION_MODE_BALANCED:
            return NVSDK_NGX_PerfQuality_Value::NVSDK_NGX_PerfQuality_Value_Balanced;
        case RG_RENDER_RESOLUTION_MODE_QUALITY:
            return NVSDK_NGX_PerfQuality_Value::NVSDK_NGX_PerfQuality_Value_MaxQuality;
        case RG_RENDER_RESOLUTION_MODE_ULTRA_QUALITY:
            return NVSDK_NGX_PerfQuality_Value::NVSDK_NGX_PerfQuality_Value_UltraQuality;
        default:
            assert(0); 
            return NVSDK_NGX_PerfQuality_Value::NVSDK_NGX_PerfQuality_Value_Balanced;
    }
}

bool RTGL1::DLSS::AreSameDlssFeatureValues(const RenderResolutionHelper &renderResolution) const
{
    return  
        prevDlssFeatureValues.renderWidth    == renderResolution.Width() &&
        prevDlssFeatureValues.renderHeight   == renderResolution.Height() &&
        prevDlssFeatureValues.upscaledWidth  == renderResolution.UpscaledWidth() &&
        prevDlssFeatureValues.upscaledHeight == renderResolution.UpscaledHeight();
}

bool RTGL1::DLSS::ValidateDlssFeature(VkCommandBuffer cmd, const RenderResolutionHelper &renderResolution)
{
    if (!isInitialized || pParams == nullptr)
    {
        return false;
    }


    if (AreSameDlssFeatureValues(renderResolution))
    {
        return true;
    }


    if (pDlssFeature != nullptr)
    {
        DestroyDlssFeature();
    }


    NVSDK_NGX_DLSS_Create_Params dlssParams = {};
    dlssParams.Feature.InWidth = renderResolution.Width();
    dlssParams.Feature.InHeight = renderResolution.Height();
    dlssParams.Feature.InTargetWidth = renderResolution.UpscaledWidth();
    dlssParams.Feature.InTargetHeight = renderResolution.UpscaledHeight();
    // dlssParams.Feature.InPerfQualityValue = ToNGXPerfQuality(renderResolution.GetResolutionMode());

    int &dlssCreateFeatureFlags = dlssParams.InFeatureCreateFlags;
    // motion vectors are in render resolution, not target resolution
    dlssCreateFeatureFlags |= NVSDK_NGX_DLSS_Feature_Flags_MVLowRes;
    // motion vectors contain jitter
    dlssCreateFeatureFlags |= NVSDK_NGX_DLSS_Feature_Flags_MVJittered;
    dlssCreateFeatureFlags |= NVSDK_NGX_DLSS_Feature_Flags_IsHDR;
    dlssCreateFeatureFlags |= NVSDK_NGX_DLSS_Feature_Flags_AutoExposure;
    dlssCreateFeatureFlags |= NVSDK_NGX_DLSS_Feature_Flags_DoSharpening;
    dlssCreateFeatureFlags |= 0; // NVSDK_NGX_DLSS_Feature_Flags_DepthInverted;

    // only one phys device
    uint32_t creationNodeMask = 1;
    uint32_t visibilityNodeMask = 1;


    NVSDK_NGX_Result r = NGX_VULKAN_CREATE_DLSS_EXT(cmd, creationNodeMask, visibilityNodeMask,
                                                    &pDlssFeature, pParams, &dlssParams);
    if (NVSDK_NGX_FAILED(r))
    {
        // LOG ERROR("Failed to create DLSS Features = 0x%08x, info: %ls", r, GetNGXResultAsString(r));
     
        pDlssFeature = nullptr;
        return false;
    }

    return true;
}

static NVSDK_NGX_Resource_VK ToNGXResource(const std::shared_ptr<RTGL1::Framebuffers> &framebuffers, uint32_t frameIndex,
                                           RTGL1::FramebufferImageIndex imageIndex, NVSDK_NGX_Dimensions size, bool withWriteAccess = false)
{
    VkImage image; VkImageView view; VkFormat format;

    framebuffers->GetImageHandles(imageIndex, frameIndex,
                                  &image, &view, &format);

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    return NVSDK_NGX_Create_ImageView_Resource_VK(view, image, subresourceRange, format, size.Width, size.Height, withWriteAccess);
}

RTGL1::FramebufferImageIndex RTGL1::DLSS::Apply(VkCommandBuffer cmd, uint32_t frameIndex,
                                                const std::shared_ptr<Framebuffers> &framebuffers,
                                                const RenderResolutionHelper &renderResolution, 
                                                RgFloat2D jitterOffset)
{
    ValidateDlssFeature(cmd, renderResolution);


    if (!IsDlssAvailable())
    {
        throw RgException(RG_WRONG_ARGUMENT, "Nvidia DLSS is not supported. Check availability before usage.");
    }


    typedef FramebufferImageIndex FI;
    const FI outputImage = FI::FB_IMAGE_INDEX_UPSCALED_OUTPUT;


    CmdLabel label(cmd, "DLSS");


    // TODO: DLSS: resettable accumulation
    //             offset of the viewport render
    int resetAccumulation = 0;
    NVSDK_NGX_Coordinates sourceOffset = { 0, 0 };
    NVSDK_NGX_Dimensions  sourceSize = { renderResolution.Width(),         renderResolution.Height()          };
    NVSDK_NGX_Dimensions  targetSize = { renderResolution.UpscaledWidth(), renderResolution .UpscaledHeight() };


    NVSDK_NGX_Resource_VK unresolvedColorResource   = ToNGXResource(framebuffers, frameIndex, FI::FB_IMAGE_INDEX_FINAL,     sourceSize);
    NVSDK_NGX_Resource_VK resolvedColorResource     = ToNGXResource(framebuffers, frameIndex, outputImage,                  targetSize, true);
    NVSDK_NGX_Resource_VK motionVectorsResource     = ToNGXResource(framebuffers, frameIndex, FI::FB_IMAGE_INDEX_MOTION,    sourceSize);
    NVSDK_NGX_Resource_VK depthResource             = ToNGXResource(framebuffers, frameIndex, FI::FB_IMAGE_INDEX_DEPTH,     sourceSize);
    NVSDK_NGX_Resource_VK exposureResource          = ToNGXResource(framebuffers, frameIndex, FI::FB_IMAGE_INDEX_EXPOSURE,  { 1, 1 });


    NVSDK_NGX_VK_DLSS_Eval_Params evalParams = {};
    evalParams.Feature.pInColor = &unresolvedColorResource;
    evalParams.Feature.pInOutput = &resolvedColorResource;
    evalParams.pInDepth = &depthResource;
    evalParams.pInMotionVectors = &motionVectorsResource;
    evalParams.pInExposureTexture = &exposureResource;
    evalParams.InJitterOffsetX = jitterOffset.data[0];
    evalParams.InJitterOffsetY = jitterOffset.data[1];
    evalParams.Feature.InSharpness = renderResolution.GetNvDlssSharpness();
    evalParams.InReset = resetAccumulation;
    evalParams.InMVScaleX = 1.0f;
    evalParams.InMVScaleY = 1.0f;
    evalParams.InColorSubrectBase = sourceOffset;
    evalParams.InDepthSubrectBase = sourceOffset;
    evalParams.InMVSubrectBase = sourceOffset;
    evalParams.InRenderSubrectDimensions = sourceSize;


    NVSDK_NGX_Result r = NGX_VULKAN_EVALUATE_DLSS_EXT(cmd, pDlssFeature, pParams, &evalParams);

    if (NVSDK_NGX_FAILED(r))
    {
       // LOG ERROR("Failed to NVSDK_NGX_VULKAN_EvaluateFeature for DLSS, code = 0x%08x, info: %ls", r, GetNGXResultAsString(r));
    }
}

void RTGL1::DLSS::GetOptimalSettings(uint32_t userWidth, uint32_t userHeight, RgRenderResolutionMode mode,
                                     uint32_t *pOutWidth, uint32_t *pOutHeight, float *pOutSharpness) const
{
    *pOutWidth = userWidth;
    *pOutHeight = userHeight;
    *pOutSharpness = 0.0f;

    if (isInitialized && pParams != nullptr)
    {
        return;
    }

    uint32_t minWidth, minHeight, maxWidth, maxHeight;
    NVSDK_NGX_Result r = NGX_DLSS_GET_OPTIMAL_SETTINGS(pParams,
                                                       userWidth, userHeight, ToNGXPerfQuality(mode),
                                                       pOutWidth, pOutHeight,
                                                       &maxWidth, &maxHeight, &minWidth, &minHeight,
                                                       pOutSharpness);
    if (NVSDK_NGX_FAILED(r))
    {
        // LOG INFO("Querying Optimal Settings failed! code = 0x%08x, info: %ls", r, GetNGXResultAsString(r));
    }
}