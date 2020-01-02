#include "vk_common.h"

namespace vulkan
{
	GpuRenderPipeline::GpuRenderPipeline(GpuDevice* device, ngfx::RenderPipelineDesc const& desc)
		: GpuPipelineBase(device)
	{
		create_info_.stageCount;
		create_info_.pStages;
		create_info_.pVertexInputState;
		create_info_.pInputAssemblyState;
		create_info_.pTessellationState;
		create_info_.pViewportState;
		create_info_.pRasterizationState;
		create_info_.pMultisampleState;
		create_info_.pDepthStencilState;
		create_info_.pColorBlendState;
		create_info_.pDynamicState;
		create_info_.layout;
		create_info_.renderPass;
		create_info_.subpass;
		create_info_.basePipelineHandle;
		create_info_.basePipelineIndex;
	}

	GpuRenderPipeline::~GpuRenderPipeline()
	{
	}

	GpuComputePipeline::GpuComputePipeline(GpuDevice* device, ngfx::ComputePipelineDesc const& desc)
		: GpuPipelineBase(device)
	{
		create_info_.stage;
		create_info_.layout;
		create_info_.basePipelineIndex;
		create_info_.basePipelineHandle;
	}

	GpuComputePipeline::~GpuComputePipeline()
	{
	}
}