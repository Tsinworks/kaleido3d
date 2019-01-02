#include "vk_common.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

namespace vulkan {
    GpuBuffer::GpuBuffer(const ngfx::BufferDesc & desc, GpuDevice * device)
    {
    }
    GpuBuffer::~GpuBuffer()
    {
    }

    ngfx::BufferView* GpuBuffer::newView(ngfx::Result * result)
    {
        return nullptr;
    }

    GpuTexture::GpuTexture(const ngfx::TextureDesc & desc, GpuDevice * device)
    {
    }

    GpuTexture::~GpuTexture()
    {
    }
    
    GpuSampler::GpuSampler(const ngfx::SamplerDesc & desc, GpuDevice * device)
        : desc_(desc)
        , device_(device)
    {

    }

    GpuSampler::~GpuSampler()
    {

    }

    void GpuSampler::setLabel(const char * label)
    {
    }

    const char * GpuSampler::label() const
    {
        return nullptr;
    }

    ngfx::TextureView* GpuTexture::newView(ngfx::Result * result)
    {
        return nullptr;
    }
    
    ngfx::Texture* GpuDevice::newTexture(const ngfx::TextureDesc * desc, ngfx::StorageMode mode, ngfx::Result * result)
    {
        return nullptr;
    }

    ngfx::Buffer* GpuDevice::newBuffer(const ngfx::BufferDesc * desc, ngfx::StorageMode mode, ngfx::Result * result)
    {
        return nullptr;
    }
    
    ngfx::Sampler* GpuDevice::newSampler(const ngfx::SamplerDesc * desc, ngfx::Result * result) {
        GpuSampler* sampler = new GpuSampler(*desc, this);
        if (sampler->isValid()) {
            return sampler;
        } else {
            delete sampler;
            return nullptr;
        }
    }
    GpuPipelineBase::GpuPipelineBase(GpuDevice * device)
        : device_(device)
    {
    }
    GpuPipelineBase::~GpuPipelineBase()
    {
    }

}