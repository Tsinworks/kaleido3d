#include "vk_common.h"
#include "vk_common.h"
#include "vk_common.h"
#include "vk_common.h"
#include "vk_common.h"
#include "vk_common.h"
namespace vulkan
{
	inline void GpuDevice::getPhysicalDeviceMemoryProperties(VkPhysicalDeviceMemoryProperties& memProps)
	{
		factory_->__GetPhysicalDeviceMemoryProperties(physical_device_, &memProps);
	}
	
	inline VkResult GpuDevice::allocateMemory(const VkMemoryAllocateInfo* pAllocateInfo, VkDeviceMemory* pMemory)
	{
		return this->__AllocateMemory(device_, pAllocateInfo, NGFXVK_ALLOCATOR, pMemory);
	}

	inline void GpuDevice::freeMemory(VkDeviceMemory memory)
	{
		this->__FreeMemory(device_, memory, NGFXVK_ALLOCATOR);
	}

	inline void* GpuDevice::mapMemory(VkDeviceMemory mem, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags flags)
	{
		void* ptr = nullptr;
		this->__MapMemory(device_, mem, offset, size, flags, &ptr);
		return ptr;
	}

	inline void GpuDevice::flushMappedMemory(uint32_t memoryRangeCount, const VkMappedMemoryRange * pMemoryRanges)
	{
		this->__FlushMappedMemoryRanges(device_, memoryRangeCount, pMemoryRanges);
	}

	inline void GpuDevice::invalidMappedMemory(uint32_t memoryRangeCount, const VkMappedMemoryRange * pMemoryRanges)
	{
		this->__InvalidateMappedMemoryRanges(device_, memoryRangeCount, pMemoryRanges);
	}

	inline void GpuDevice::unmapMemory(VkDeviceMemory mem)
	{
		this->__UnmapMemory(device_, mem);
	}

	inline VkResult GpuDevice::createBuffer(const VkBufferCreateInfo * pCreateInfo, VkBuffer * pBuffer)
	{
		return __CreateBuffer(device_, pCreateInfo, NGFXVK_ALLOCATOR, pBuffer);
	}

	inline void GpuDevice::destroyBuffer(VkBuffer buffer)
	{
		__DestroyBuffer(device_, buffer, NGFXVK_ALLOCATOR);
	}

	inline void GpuDevice::getBufferMemoryRequirements(VkBuffer buffer, VkMemoryRequirements* pMemoryRequirements)
	{
		this->__GetBufferMemoryRequirements(device_, buffer, pMemoryRequirements);
	}

	inline VkResult GpuDevice::createImage(const VkImageCreateInfo* pCreateInfo, VkImage* pImage)
	{
		return __CreateImage(device_, pCreateInfo, NGFXVK_ALLOCATOR, pImage);
	}

	inline void GpuDevice::destroyImage(VkImage image)
	{
		__DestroyImage(device_, image, NGFXVK_ALLOCATOR);
	}
	
	inline void GpuDevice::getImageMemoryRequirements(VkImage image, VkMemoryRequirements * pMemoryRequirements)
	{
		this->__GetImageMemoryRequirements(device_, image, pMemoryRequirements);
	}

	inline VkResult GpuDevice::createSampler(const VkSamplerCreateInfo * pCreateInfo, VkSampler * pSampler)
	{
		return VkResult();
	}

	inline void GpuDevice::destroySampler(VkSampler sampler)
	{
	}

	inline void GpuDevice::getAccelerationStructureMemoryRequirements(
		VkAccelerationStructureNV accelerationStructure,
		VkAccelerationStructureMemoryRequirementsTypeNV type,
		VkMemoryRequirements2KHR* pMemoryRequirements)
	{
		VkAccelerationStructureMemoryRequirementsInfoNV memoryRequirementsInfo{};
		memoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NV;
		memoryRequirementsInfo.type = type;
		memoryRequirementsInfo.accelerationStructure = accelerationStructure;
		if(__GetAccelerationStructureMemoryRequirementsNV)
			__GetAccelerationStructureMemoryRequirementsNV(device_, &memoryRequirementsInfo, pMemoryRequirements);
	}
	
	inline VkResult GpuDevice::createAccelerationStructure(
		const VkAccelerationStructureCreateInfoNV * pCreateInfo,
		const VkAllocationCallbacks * pAllocator,
		VkAccelerationStructureNV * pAccelerationStructure)
	{
		if (this->__CreateAccelerationStructureNV) {
			return this->__CreateAccelerationStructureNV(device_, pCreateInfo, pAllocator, pAccelerationStructure);
		}
		return VkResult::VK_ERROR_INITIALIZATION_FAILED;
	}

	inline void GpuDevice::destroyAccelerationStructure(VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks * pAllocator)
	{
		if (this->__DestroyAccelerationStructureNV) {
			this->__DestroyAccelerationStructureNV(device_, accelerationStructure, pAllocator);
		}
	}

	inline VkResult GpuDevice::createFence(const VkFenceCreateInfo & info, VkFence * pFence)
	{
		return this->__CreateFence(device_, &info, NGFXVK_ALLOCATOR, pFence);
	}

	inline void GpuDevice::destroyFence(VkFence fence)
	{
		this->__DestroyFence(device_, fence, NGFXVK_ALLOCATOR);
	}

	
}