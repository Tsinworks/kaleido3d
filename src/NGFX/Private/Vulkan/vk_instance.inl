#include "vk_common.h"
#include "vk_common.h"
namespace vulkan
{
	inline void GpuFactory::printLogStr(int level, const char * msg)
	{
		if (log_call_) {
			log_call_(level, msg);
		}
	}
	
	inline void GpuFactory::destroySurface(VkSurfaceKHR surface)
	{
		__DestroySurfaceKHR(instance_, surface, NGFXVK_ALLOCATOR);
	}

	inline bool GpuFactory::debuggable() const
	{
		return debug_enable_;
	}
}