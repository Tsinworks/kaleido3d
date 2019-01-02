#include "vk_common.h"

namespace vulkan {
    GpuSwapchain::GpuSwapchain(GpuDevice * device)
        : device_(device)
    {
    }
    GpuSwapchain::~GpuSwapchain()
    {
    }
    ngfx::Swapchain* GpuDevice::newSwapchain(
        const ngfx::SwapchainDesc * desc, 
        const ngfx::Swapchain * old, void * surface, 
        ngfx::Result * result)
    {
        VkSwapchainCreateInfoKHR create_info = {
            VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
/*
    const void*                      pNext;
    VkSwapchainCreateFlagsKHR        flags;
    VkSurfaceKHR                     surface;
    uint32_t                         minImageCount;
    VkFormat                         imageFormat;
    VkColorSpaceKHR                  imageColorSpace;
    VkExtent2D                       imageExtent;
    uint32_t                         imageArrayLayers;
    VkImageUsageFlags                imageUsage;
    VkSharingMode                    imageSharingMode;
    uint32_t                         queueFamilyIndexCount;
    const uint32_t*                  pQueueFamilyIndices;
    VkSurfaceTransformFlagBitsKHR    preTransform;
    VkCompositeAlphaFlagBitsKHR      compositeAlpha;
    VkPresentModeKHR                 presentMode;
    VkBool32                         clipped;
    VkSwapchainKHR                   oldSwapchain;
 */
        };

        return nullptr;
    }
}