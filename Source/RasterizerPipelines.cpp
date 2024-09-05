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

#include "RasterizerPipelines.h"

#include <array>
#include <set>

#include "RasterizedDataCollector.h"

constexpr uint32_t PIPELINE_STATE_MASK_IS_ALPHA_TEST                    = 1 << 0;
constexpr uint32_t PIPELINE_STATE_MASK_BLEND_ENABLE                     = 1 << 1;
constexpr uint32_t PIPELINE_STATE_MASK_DEPTH_TEST_ENABLE                = 1 << 2;
constexpr uint32_t PIPELINE_STATE_MASK_DEPTH_WRITE_ENABLE               = 1 << 3;
constexpr uint32_t PIPELINE_STATE_MASK_IS_LINES                         = 1 << 4;
constexpr uint32_t PS_SRC_OFFSET                                        = 5;

constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_ONE                   = 1 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_ZERO                  = 2 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_SRC_COLOR             = 3 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_ONE_MINUS_SRC_COLOR   = 4 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_DST_COLOR             = 5 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_ONE_MINUS_DST_COLOR   = 6 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_SRC_ALPHA             = 7 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_SRC_ONE_MINUS_SRC_ALPHA   = 8 << PS_SRC_OFFSET;
constexpr uint32_t PIPELINE_STATE_MASK_BLEND_SRC                        = 15 << PS_SRC_OFFSET;
constexpr uint32_t PS_DST_OFFSET                                        = 4 + PS_SRC_OFFSET;

constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_ONE                   = 1 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_ZERO                  = 2 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_SRC_COLOR             = 3 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_ONE_MINUS_SRC_COLOR   = 4 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_DST_COLOR             = 5 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_ONE_MINUS_DST_COLOR   = 6 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_SRC_ALPHA             = 7 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_VALUE_BLEND_DST_ONE_MINUS_SRC_ALPHA   = 8 << PS_DST_OFFSET;
constexpr uint32_t PIPELINE_STATE_MASK_BLEND_DST                        = 15 << PS_DST_OFFSET;

static uint32_t ConvertToStateFlags(RgRasterizedGeometryStateFlags pipelineState, RgBlendFactor blendFuncSrc, RgBlendFactor blendFuncDst)
{
    uint32_t r = 0;

    if (pipelineState & RG_RASTERIZED_GEOMETRY_STATE_BLEND_ENABLE)
    {
        r |= PIPELINE_STATE_MASK_BLEND_ENABLE;

        switch (blendFuncSrc)
        {
            case RG_BLEND_FACTOR_ONE:                   r |= PIPELINE_STATE_VALUE_BLEND_SRC_ONE;            break;
            case RG_BLEND_FACTOR_ZERO:                  r |= PIPELINE_STATE_VALUE_BLEND_SRC_ZERO;           break;
            case RG_BLEND_FACTOR_SRC_COLOR:             r |= PIPELINE_STATE_VALUE_BLEND_SRC_SRC_COLOR;      break;
            case RG_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:   r |= PIPELINE_STATE_VALUE_BLEND_SRC_ONE_MINUS_SRC_COLOR;  break;
            case RG_BLEND_FACTOR_DST_COLOR:             r |= PIPELINE_STATE_VALUE_BLEND_SRC_DST_COLOR;      break;
            case RG_BLEND_FACTOR_ONE_MINUS_DST_COLOR:   r |= PIPELINE_STATE_VALUE_BLEND_SRC_ONE_MINUS_DST_COLOR;  break;
            case RG_BLEND_FACTOR_SRC_ALPHA:             r |= PIPELINE_STATE_VALUE_BLEND_SRC_SRC_ALPHA;      break;
            case RG_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:   r |= PIPELINE_STATE_VALUE_BLEND_SRC_ONE_MINUS_SRC_ALPHA;  break;
            default: assert(0); r = 0;
        }

        switch (blendFuncDst)
        {
            case RG_BLEND_FACTOR_ONE:                   r |= PIPELINE_STATE_VALUE_BLEND_DST_ONE;            break;
            case RG_BLEND_FACTOR_ZERO:                  r |= PIPELINE_STATE_VALUE_BLEND_DST_ZERO;           break;
            case RG_BLEND_FACTOR_SRC_COLOR:             r |= PIPELINE_STATE_VALUE_BLEND_DST_SRC_COLOR;      break;
            case RG_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:   r |= PIPELINE_STATE_VALUE_BLEND_DST_ONE_MINUS_SRC_COLOR;  break;
            case RG_BLEND_FACTOR_DST_COLOR:             r |= PIPELINE_STATE_VALUE_BLEND_DST_DST_COLOR;      break;
            case RG_BLEND_FACTOR_ONE_MINUS_DST_COLOR:   r |= PIPELINE_STATE_VALUE_BLEND_DST_ONE_MINUS_DST_COLOR;  break;
            case RG_BLEND_FACTOR_SRC_ALPHA:             r |= PIPELINE_STATE_VALUE_BLEND_DST_SRC_ALPHA;      break;
            case RG_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:   r |= PIPELINE_STATE_VALUE_BLEND_DST_ONE_MINUS_SRC_ALPHA;  break;
            default: assert(0); r = 0;
        }
    }

    if (pipelineState & RG_RASTERIZED_GEOMETRY_STATE_DEPTH_TEST)
    {
        r |= PIPELINE_STATE_MASK_DEPTH_TEST_ENABLE;
    }

    if (pipelineState & RG_RASTERIZED_GEOMETRY_STATE_DEPTH_WRITE)
    {
        r |= PIPELINE_STATE_MASK_DEPTH_WRITE_ENABLE;
    }

    if (pipelineState & RG_RASTERIZED_GEOMETRY_STATE_FORCE_LINE_LIST)
    {
        r |= PIPELINE_STATE_MASK_IS_LINES;
    }

    if (pipelineState & RG_RASTERIZED_GEOMETRY_STATE_ALPHA_TEST)
    {
        r |= PIPELINE_STATE_MASK_IS_ALPHA_TEST;
    }

    return r;
}

static bool TestFlags()
{
    const bool bs[] =
    {
        true,
        false
    };

    const RgBlendFactor fs[] = 
    {
        RG_BLEND_FACTOR_ONE,
        RG_BLEND_FACTOR_ZERO,
        RG_BLEND_FACTOR_SRC_COLOR,
        RG_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        RG_BLEND_FACTOR_DST_COLOR,
        RG_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        RG_BLEND_FACTOR_SRC_ALPHA,
        RG_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
    };

    std::set<uint32_t> un;

    for (auto al : bs) {
    for (auto be : bs) {
    for (auto dt : bs) {
    for (auto dw : bs) {
    for (auto ln : bs)
    {
        RgRasterizedGeometryStateFlags pstate = 0;
        if (al)
        {
            pstate |= RG_RASTERIZED_GEOMETRY_STATE_ALPHA_TEST;
        }
        if (dt)
        {
            pstate |= RG_RASTERIZED_GEOMETRY_STATE_DEPTH_TEST;
        }
        if (dw)
        {
            pstate |= RG_RASTERIZED_GEOMETRY_STATE_DEPTH_WRITE;
        }
        if (ln)
        {
            pstate |= RG_RASTERIZED_GEOMETRY_STATE_FORCE_LINE_LIST;
        }

        if (be)
        {
            pstate |= RG_RASTERIZED_GEOMETRY_STATE_BLEND_ENABLE;

            for (auto src : fs)
            {
                for (auto dst : fs)
                {
                    uint32_t flags = ConvertToStateFlags(pstate, src, dst);

                    if (un.count(flags) > 0)
                    {
                        assert(0);
                        return false;
                    }

                    un.insert(flags);
                }
            }
        }
        else
        {
            uint32_t flags = ConvertToStateFlags(pstate, RG_BLEND_FACTOR_ONE, RG_BLEND_FACTOR_ONE);

            if (un.count(flags) > 0)
            {
                assert(0);
                return false;
            }

            un.insert(flags);
        }
    } } } } }

    return true;
}

static VkBlendFactor ConvertBlendFactorToVk(RgBlendFactor f)
{
    switch (f)
    {
        case RG_BLEND_FACTOR_ONE:                   return VK_BLEND_FACTOR_ONE;
        case RG_BLEND_FACTOR_ZERO:                  return VK_BLEND_FACTOR_ZERO;
        case RG_BLEND_FACTOR_SRC_COLOR:             return VK_BLEND_FACTOR_SRC_COLOR;
        case RG_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:   return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case RG_BLEND_FACTOR_DST_COLOR:             return VK_BLEND_FACTOR_DST_COLOR;
        case RG_BLEND_FACTOR_ONE_MINUS_DST_COLOR:   return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case RG_BLEND_FACTOR_SRC_ALPHA:             return VK_BLEND_FACTOR_SRC_ALPHA;
        case RG_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:   return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        default: assert(0); return VK_BLEND_FACTOR_ONE;
    }
}

RTGL1::RasterizerPipelines::RasterizerPipelines(
    VkDevice _device,
    VkPipelineLayout _pipelineLayout, 
    VkRenderPass _renderPass,
    bool _applyVertexColorGamma)
:
    device(_device),
    pipelineLayout(_pipelineLayout),
    renderPass(_renderPass),
    vertShaderStage{},
    fragShaderStage{},
    pipelineCache(VK_NULL_HANDLE),
    applyVertexColorGamma(_applyVertexColorGamma)
{
    assert(TestFlags());

    VkPipelineCacheCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

    VkResult r = vkCreatePipelineCache(device, &info, nullptr, &pipelineCache);
    VK_CHECKERROR(r);
}

RTGL1::RasterizerPipelines::~RasterizerPipelines()
{
    for (auto &p : pipelines)
    {
        vkDestroyPipeline(device, p.second, nullptr);
    }

    vkDestroyPipelineCache(device, pipelineCache, nullptr);
}

void RTGL1::RasterizerPipelines::Clear()
{
    for (auto &p : pipelines)
    {
        vkDestroyPipeline(device, p.second, nullptr);
    }

    pipelines.clear();
}

void RTGL1::RasterizerPipelines::SetShaders(const ShaderManager *shaderManager, const char *vertexShaderName, const char *fragmentShaderName)
{
    vertShaderStage         = shaderManager->GetStageInfo(vertexShaderName);
    fragShaderStage         = shaderManager->GetStageInfo(fragmentShaderName);
}

void RTGL1::RasterizerPipelines::DisableDynamicState(const VkViewport &viewport, const VkRect2D &scissors)
{
    assert(pipelines.empty());

    dynamicState.isEnabled = false;

    dynamicState.viewport = viewport;
    dynamicState.scissors = scissors;
}

VkPipeline RTGL1::RasterizerPipelines::GetPipeline(RgRasterizedGeometryStateFlags pipelineState, RgBlendFactor blendFuncSrc, RgBlendFactor blendFuncDst)
{
    uint32_t flags = ConvertToStateFlags(pipelineState, blendFuncSrc, blendFuncDst);

    auto f = pipelines.find(flags);

    // if such pipeline already exist
    if (f != pipelines.end())
    {
        return f->second;
    }
    else
    {
        VkPipeline p = CreatePipeline(pipelineState, blendFuncSrc, blendFuncDst);

        pipelines[flags] = p;
        return p;
    }
}

VkPipelineLayout RTGL1::RasterizerPipelines::GetPipelineLayout()
{
    return pipelineLayout;
}

VkPipeline RTGL1::RasterizerPipelines::CreatePipeline(RgRasterizedGeometryStateFlags pipelineState, RgBlendFactor blendFuncSrc, RgBlendFactor blendFuncDst) const
{
    assert(vertShaderStage.sType != 0 && fragShaderStage.sType != 0);


    RgBool32 alphaTest  = pipelineState & RG_RASTERIZED_GEOMETRY_STATE_ALPHA_TEST ? 1 : 0;
    bool blendEnable    = pipelineState & RG_RASTERIZED_GEOMETRY_STATE_BLEND_ENABLE;
    bool depthTest      = pipelineState & RG_RASTERIZED_GEOMETRY_STATE_DEPTH_TEST;
    bool depthWrite     = pipelineState & RG_RASTERIZED_GEOMETRY_STATE_DEPTH_WRITE;
    bool isLines        = pipelineState & RG_RASTERIZED_GEOMETRY_STATE_FORCE_LINE_LIST;


    VkSpecializationMapEntry vertMapEntry = {};
    vertMapEntry.constantID = 0;
    vertMapEntry.offset = 0;
    vertMapEntry.size = sizeof(uint32_t);

    VkSpecializationInfo vertSpecInfo = {};
    vertSpecInfo.mapEntryCount = 1;
    vertSpecInfo.pMapEntries = &vertMapEntry;
    vertSpecInfo.dataSize = sizeof(applyVertexColorGamma);
    vertSpecInfo.pData = &applyVertexColorGamma;

    VkSpecializationMapEntry fragMapEntry = {};
    fragMapEntry.constantID = 0;
    fragMapEntry.offset = 0;
    fragMapEntry.size = sizeof(uint32_t);

    VkSpecializationInfo fragSpecInfo = {};
    fragSpecInfo.mapEntryCount = 1;
    fragSpecInfo.pMapEntries = &fragMapEntry;
    fragSpecInfo.dataSize = sizeof(alphaTest);
    fragSpecInfo.pData = &alphaTest;

    VkPipelineShaderStageCreateInfo shaderStages[] =
    {
        vertShaderStage,
        fragShaderStage
    };
    shaderStages[0].pSpecializationInfo = &vertSpecInfo;
    shaderStages[1].pSpecializationInfo = &fragSpecInfo;


    VkDynamicState dynamicStates[2] =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkVertexInputBindingDescription vertBinding = {};
    vertBinding.binding = 0;
    vertBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertBinding.stride = RasterizedDataCollector::GetVertexStride();

    std::array<VkVertexInputAttributeDescription, 8> attrs = {};
    uint32_t attrsCount;

    RasterizedDataCollector::GetVertexLayout(attrs.data(), &attrsCount);
    assert(attrsCount <= attrs.size());

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &vertBinding;
    vertexInputInfo.vertexAttributeDescriptionCount = attrsCount;
    vertexInputInfo.pVertexAttributeDescriptions = attrs.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = isLines ? VK_PRIMITIVE_TOPOLOGY_LINE_LIST : VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = dynamicState.isEnabled ? nullptr : &dynamicState.viewport; 
    viewportState.scissorCount = 1; 
    viewportState.pScissors = dynamicState.isEnabled ? nullptr : &dynamicState.scissors; 

    VkPipelineRasterizationStateCreateInfo raster = {};
    raster.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    raster.polygonMode = VK_POLYGON_MODE_FILL;
    raster.cullMode = VK_CULL_MODE_NONE;
    raster.depthClampEnable = VK_FALSE;
    raster.rasterizerDiscardEnable = VK_FALSE;
    raster.lineWidth = 1.0f;
    raster.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.sampleShadingEnable = VK_FALSE;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    // must be true, if depthWrite is true
    depthStencil.depthTestEnable = depthTest || depthWrite;
    depthStencil.depthWriteEnable = depthWrite;
    depthStencil.stencilTestEnable = VK_FALSE;
    depthStencil.depthBoundsTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttch = {};
    colorBlendAttch.blendEnable = blendEnable;
    colorBlendAttch.colorBlendOp = colorBlendAttch.alphaBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttch.srcColorBlendFactor = colorBlendAttch.srcAlphaBlendFactor = ConvertBlendFactorToVk(blendFuncSrc);
    colorBlendAttch.dstColorBlendFactor = colorBlendAttch.dstAlphaBlendFactor = ConvertBlendFactorToVk(blendFuncDst);

    colorBlendAttch.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &colorBlendAttch;

    VkPipelineDynamicStateCreateInfo dynamicInfo = {};
    dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicInfo.dynamicStateCount = dynamicState.isEnabled ? std::size(dynamicStates) : 0;
    dynamicInfo.pDynamicStates = dynamicStates;

    VkGraphicsPipelineCreateInfo plInfo = {};
    plInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    plInfo.stageCount = std::size(shaderStages);
    plInfo.pStages = shaderStages;
    plInfo.pVertexInputState = &vertexInputInfo;
    plInfo.pInputAssemblyState = &inputAssembly;
    plInfo.pViewportState = &viewportState;
    plInfo.pRasterizationState = &raster;
    plInfo.pMultisampleState = &multisampling;
    plInfo.pDepthStencilState = &depthStencil;
    plInfo.pColorBlendState = &colorBlendState;
    plInfo.pDynamicState = &dynamicInfo;
    plInfo.layout = pipelineLayout;
    plInfo.renderPass = renderPass;
    plInfo.subpass = 0;
    plInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline pipeline;

    VkResult r = vkCreateGraphicsPipelines(device, pipelineCache, 1, &plInfo, nullptr, &pipeline);
    VK_CHECKERROR(r);

    SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, pipeline, VK_OBJECT_TYPE_PIPELINE, "Rasterizer raster draw pipeline");

    return pipeline;
}

void RTGL1::RasterizerPipelines::BindPipelineIfNew(
    VkCommandBuffer cmd, VkPipeline &curPipeline,
    RgRasterizedGeometryStateFlags pipelineState, RgBlendFactor blendFuncSrc, RgBlendFactor blendFuncDst)
{
    VkPipeline p = GetPipeline(pipelineState, blendFuncSrc, blendFuncDst);

    if (p != curPipeline)
    {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, p);
        curPipeline = p;
    }
}
