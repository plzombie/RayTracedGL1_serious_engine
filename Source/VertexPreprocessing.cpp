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

#include "VertexPreprocessing.h"

#include <vector>
#include <cmath>
#include "Generated/ShaderCommonC.h"
#include "CmdLabel.h"

RTGL1::VertexPreprocessing::VertexPreprocessing(
    VkDevice _device,
    const std::shared_ptr<const GlobalUniform> &_uniform,
    const std::shared_ptr<const ASManager> &_asManager,
    const std::shared_ptr<const ShaderManager> &_shaderManager)
:
    device(_device)
{
    std::vector<VkDescriptorSetLayout> setLayouts =
    {
        _uniform->GetDescSetLayout(),
        _asManager->GetBuffersDescSetLayout()
    };

    CreatePipelineLayout(setLayouts.data(), setLayouts.size());
    CreatePipelines(_shaderManager.get());
}

RTGL1::VertexPreprocessing::~VertexPreprocessing()
{
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    DestroyPipelines();
}

void RTGL1::VertexPreprocessing::Preprocess(
    VkCommandBuffer cmd, uint32_t frameIndex, uint32_t preprocMode,
    const std::shared_ptr<const GlobalUniform> &uniform,
    const std::shared_ptr<ASManager> &asManager,
    const ShVertPreprocessing &push)
{
    CmdLabel label(cmd, "Vertex preprocessing");


    asManager->OnVertexPreprocessingBegin(cmd, frameIndex, preprocMode == VERT_PREPROC_MODE_ONLY_DYNAMIC);


    VkPipeline pl = 
        preprocMode == VERT_PREPROC_MODE_ALL ? pipelineAll :
        preprocMode == VERT_PREPROC_MODE_DYNAMIC_AND_MOVABLE ? pipelineDynamicAndMovable :
        pipelineOnlyDynamic;
   
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, pl);


    VkDescriptorSet sets[] =
    {
        uniform->GetDescSet(frameIndex),
        asManager->GetBuffersDescSet(frameIndex)
    };
    const uint32_t setCount = sizeof(sets) / sizeof(VkDescriptorSet);
    
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE,
                            pipelineLayout,
                            0, setCount, sets,
                            0, nullptr);


    vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(ShVertPreprocessing), &push);


    vkCmdDispatch(cmd, push.tlasInstanceCount, 1, 1);


    asManager->OnVertexPreprocessingFinish(cmd, frameIndex, preprocMode == VERT_PREPROC_MODE_ONLY_DYNAMIC);
}

void RTGL1::VertexPreprocessing::OnShaderReload(const ShaderManager *shaderManager)
{
    DestroyPipelines();
    CreatePipelines(shaderManager);
}

void RTGL1::VertexPreprocessing::CreatePipelineLayout(VkDescriptorSetLayout*pSetLayouts, uint32_t setLayoutCount)
{
    VkPushConstantRange pc = {};
    pc.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    pc.offset = 0;
    pc.size = sizeof(ShVertPreprocessing);

    VkPipelineLayoutCreateInfo plLayoutInfo = {};
    plLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    plLayoutInfo.setLayoutCount = setLayoutCount;
    plLayoutInfo.pSetLayouts = pSetLayouts;
    plLayoutInfo.pushConstantRangeCount = 1;
    plLayoutInfo.pPushConstantRanges = &pc;

    VkResult r = vkCreatePipelineLayout(device, &plLayoutInfo, nullptr, &pipelineLayout);
    VK_CHECKERROR(r);

    SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, pipelineLayout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, "Vertex preprocessing pipeline layout");
}

void RTGL1::VertexPreprocessing::CreatePipelines(const ShaderManager *shaderManager)
{
    VkResult r;

    uint32_t specInfoDataOnlyDynamic = 0;

    VkSpecializationMapEntry specEntry = {};
    specEntry.constantID = 0;
    specEntry.offset = 0;
    specEntry.size = sizeof(uint32_t);

    VkSpecializationInfo specInfo = {};
    specInfo.mapEntryCount = 1;
    specInfo.pMapEntries = &specEntry;
    specInfo.dataSize = sizeof(uint32_t);
    specInfo.pData = &specInfoDataOnlyDynamic;

    VkComputePipelineCreateInfo plInfo = {};
    plInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    plInfo.layout = pipelineLayout;
    plInfo.stage = shaderManager->GetStageInfo("CVertexPreprocess");
    plInfo.stage.pSpecializationInfo = &specInfo;
    
    {
        specInfoDataOnlyDynamic = VERT_PREPROC_MODE_ONLY_DYNAMIC;

        r = vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &plInfo, nullptr, &pipelineOnlyDynamic);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, pipelineOnlyDynamic, VK_OBJECT_TYPE_PIPELINE, "Vertex only dynamic preprocessing pipeline");
    }
    
    {
        specInfoDataOnlyDynamic = VERT_PREPROC_MODE_DYNAMIC_AND_MOVABLE;

        r = vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &plInfo, nullptr, &pipelineDynamicAndMovable);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, pipelineDynamicAndMovable, VK_OBJECT_TYPE_PIPELINE, "Vertex movable/dynamic preprocessing pipeline");
    }

    {
        specInfoDataOnlyDynamic = VERT_PREPROC_MODE_ALL;

        r = vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &plInfo, nullptr, &pipelineAll);
        VK_CHECKERROR(r);

        SET_DEBUG_NAME_FOR_NON_DISPATCHABLE_HANDLE(device, pipelineAll, VK_OBJECT_TYPE_PIPELINE, "Vertex static/movable/dynamic preprocessing pipeline");
    }
}

void RTGL1::VertexPreprocessing::DestroyPipelines()
{
    vkDestroyPipeline(device, pipelineOnlyDynamic, nullptr);
    vkDestroyPipeline(device, pipelineDynamicAndMovable, nullptr);
    vkDestroyPipeline(device, pipelineAll, nullptr);

    pipelineOnlyDynamic = VK_NULL_HANDLE;
    pipelineDynamicAndMovable = VK_NULL_HANDLE;
    pipelineAll = VK_NULL_HANDLE;
}
