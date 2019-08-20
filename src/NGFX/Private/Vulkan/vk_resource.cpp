#include "vk_common.h"

namespace vulkan {
    GpuBuffer::GpuBuffer(const ngfx::BufferDesc& desc, VkBuffer buffer, GpuAllocator::MemoryItem inMemItem, GpuDevice* device)
		: buffer_(buffer)
		, mem_item_(inMemItem)
		, device_(device)
    {
    }
    GpuBuffer::~GpuBuffer()
    {
		if (buffer_)
		{
			device_->getAllocator().freeBuffer(buffer_, mem_item_);
			device_->destroyBuffer(buffer_);
			buffer_ = VK_NULL_HANDLE;
		}
    }

    ngfx::BufferView* GpuBuffer::newView(ngfx::Result * result)
    {
        return nullptr;
    }

	void* GpuBuffer::map(ngfx::uint64 offset, ngfx::uint64 size)
	{
		return nullptr;
	}

	void GpuBuffer::unmap(void * addr)
	{
		
	}

	void GpuBuffer::setLabel(const char * label)
	{
	}

	const char* GpuBuffer::label() const
	{
		return nullptr;
	}

    GpuTexture::GpuTexture(const ngfx::TextureDesc& desc, VkImage image, GpuAllocator::MemoryItem inMemItem, GpuDevice * device)
		: texture_(image)
		, mem_item_(inMemItem)
		, device_(device)
    {
    }

    GpuTexture::~GpuTexture()
    {
		if (texture_)
		{
			device_->getAllocator().freeImage(texture_, mem_item_);
			device_->destroyImage(texture_);
			texture_ = VK_NULL_HANDLE;
		}
    }
    
    GpuSampler::GpuSampler(const ngfx::SamplerDesc& desc, GpuDevice * device)
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

	ngfx::PixelFormat GpuTexture::format() const
	{
		return ngfx::PixelFormat();
	}

	ngfx::TextureView* GpuTexture::newView(ngfx::Result * result)
    {
        return nullptr;
    }
	void* GpuTexture::map(ngfx::uint64 offset, ngfx::uint64 size)
	{
		return nullptr;
	}
	void GpuTexture::unmap(void* addr)
	{
	}
	void GpuTexture::setLabel(const char * label)
	{

	}
	const char* GpuTexture::label() const
	{
		return nullptr;
	}

	static VkImageCreateInfo GetImageCreateInfo(ngfx::TextureDesc const& desc, ngfx::StorageMode mode);
    ngfx::Texture* GpuDevice::newTexture(const ngfx::TextureDesc * desc, ngfx::StorageMode mode, ngfx::Result * result)
    {
		VkImageCreateInfo info = GetImageCreateInfo(*desc, mode);
		VkImage image = VK_NULL_HANDLE;
		VkResult ret = this->createImage(&info, &image);
		if (ret == VK_SUCCESS)
		{
			GpuAllocator::MemoryItem item;
			mem_alloc_.allocateForImage(image, mode, item);
			this->__BindImageMemory(device_, image, item.mem, item.offset);
			return new GpuTexture(*desc, image, item, this);
		}
		return nullptr;
    }

	static VkBufferCreateInfo GetBufferCreateInfo(ngfx::BufferDesc const& desc, ngfx::StorageMode mode);
    ngfx::Buffer* GpuDevice::newBuffer(const ngfx::BufferDesc* desc, ngfx::StorageMode mode, ngfx::Result* result)
    {
		check(desc);
		check(desc->size > 0);
		VkBuffer buffer = VK_NULL_HANDLE;
		auto info = GetBufferCreateInfo(*desc, mode);
		VkResult ret = this->createBuffer(&info, &buffer);
		if (ret == VK_SUCCESS)
		{
			GpuAllocator::MemoryItem item;
			mem_alloc_.allocateForBuffer(buffer, mode, item);
			this->__BindBufferMemory(device_, buffer, item.mem, item.offset);
			return new GpuBuffer(*desc, buffer, item, this);
		}
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
	VkBufferCreateInfo GetBufferCreateInfo(ngfx::BufferDesc const& desc, ngfx::StorageMode mode)
	{
		VkBufferCreateInfo CreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		/*		
		VK_BUFFER_CREATE_SPARSE_BINDING_BIT = 0x00000001,
		VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT = 0x00000002,
		VK_BUFFER_CREATE_SPARSE_ALIASED_BIT = 0x00000004,
		VK_BUFFER_CREATE_PROTECTED_BIT = 0x00000008,
		VK_BUFFER_CREATE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
		*/
		CreateInfo.flags;
		CreateInfo.size = desc.size;
		/*
		// ngfx
		VertexBuffer = 1,
		IndexBuffer = 2,
		UniformBuffer = 4,
		UnorderedAccess = 8,
		AccelerationStructure = 16,
		// vulkan
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT = 0x00000001,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT = 0x00000002,
		VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT = 0x00000004,
		VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT = 0x00000008,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT = 0x00000010,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT = 0x00000020,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT = 0x00000040,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT = 0x00000080,
		VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT = 0x00000100,
		VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT = 0x00000800,
		VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT = 0x00001000,
		VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT = 0x00000200,
		VK_BUFFER_USAGE_RAY_TRACING_BIT_NV = 0x00000400,
		*/
		if ((uint8_t)desc.usages & (uint8_t)ngfx::BufferUsage::VertexBuffer)
		{
			CreateInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		}
		if ((uint8_t)desc.usages & (uint8_t)ngfx::BufferUsage::IndexBuffer)
		{
			CreateInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		}
		if ((uint8_t)desc.usages & (uint8_t)ngfx::BufferUsage::UniformBuffer)
		{
			CreateInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		}
		if ((uint8_t)desc.usages & (uint8_t)ngfx::BufferUsage::UnorderedAccess)
		{
			CreateInfo.usage |= (
				VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT |
				VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | 
				VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
				VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
				VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT |
				VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT
				);
		}
		if ((uint8_t)desc.usages & (uint8_t)ngfx::BufferUsage::AccelerationStructure)
		{
			CreateInfo.usage |= VK_BUFFER_USAGE_RAY_TRACING_BIT_NV;
		}
		CreateInfo.sharingMode; // Default to VK_SHARING_MODE_EXCLUSIVE
		desc.deviceMask;
		CreateInfo.queueFamilyIndexCount;
		CreateInfo.pQueueFamilyIndices;
		return CreateInfo;
	}
	VkImageCreateInfo GetImageCreateInfo(ngfx::TextureDesc const& desc, ngfx::StorageMode mode)
	{
		VkImageCreateInfo CreateInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		CreateInfo.flags;
		CreateInfo.imageType;
		CreateInfo.format;
		CreateInfo.extent;
		CreateInfo.mipLevels;
		CreateInfo.arrayLayers;
		CreateInfo.samples;
		CreateInfo.tiling;
		CreateInfo.usage;
		CreateInfo.sharingMode;
		CreateInfo.queueFamilyIndexCount;
		CreateInfo.pQueueFamilyIndices;
		return CreateInfo;
	}
}