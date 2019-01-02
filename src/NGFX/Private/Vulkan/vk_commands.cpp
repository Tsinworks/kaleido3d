#include "vk_common.h"

namespace vulkan {
    GpuCommandBuffer::GpuCommandBuffer(GpuQueue * queue)
    {
    }
    GpuCommandBuffer::~GpuCommandBuffer()
    {
    }
    ngfx::RenderEncoder* GpuCommandBuffer::newRenderEncoder(ngfx::Result * result)
    {
        return nullptr;
    }

    ngfx::ComputeEncoder * GpuCommandBuffer::newComputeEncoder(ngfx::Result * result)
    {
        return nullptr;
    }

    ngfx::BlitEncoder * GpuCommandBuffer::newBlitEncoder(ngfx::Result * result)
    {
        return nullptr;
    }

    ngfx::ParallelEncoder * GpuCommandBuffer::newParallelRenderEncoder(ngfx::Result * result)
    {
        return nullptr;
    }

    ngfx::RaytraceEncoder * GpuCommandBuffer::newRaytraceEncoder(ngfx::Result * result)
    {
        return nullptr;
    }

    ngfx::Result GpuCommandBuffer::commit()
    {
        return ngfx::Result();
    }

}