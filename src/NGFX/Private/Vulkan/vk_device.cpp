#include "vk_common.h"
#include <unordered_map>

#define VULKAN_STANDARD_LAYER "VK_LAYER_LUNARG_standard_validation"

namespace vulkan {
    LayerEnumerator gLayerEnumerator;

	GpuDevice::GpuDevice(VkPhysicalDevice device)
        : physical_device_(device)
	{
        preCreateDevice();
        createDevice();
        volkLoadDeviceTable(this, device_);
        // detect extensions
        postCreateDevice();
	}
	GpuDevice::~GpuDevice()
	{
	}
    void GpuDevice::set_label(const char * label)
    {
    }
    const char * GpuDevice::label() const
    {
        return nullptr;
    }
    ngfx::DeviceType GpuDevice::getType() const
    {
        return ngfx::DeviceType();
    }
	ngfx::CommandQueue* GpuDevice::newQueue()
	{
		return nullptr;
	}
	ngfx::Shader* GpuDevice::newShader()
	{
		return nullptr;
	}
	ngfx::Renderpass* GpuDevice::newRenderpass(const ngfx::RenderpassDesc * desc, ngfx::Result * result)
	{
		return nullptr;
	}
	ngfx::ComputePipeline* GpuDevice::newComputePipeline(const ngfx::ComputePipelineDesc * desc, ngfx::Result * result)
	{
		return nullptr;
	}
	ngfx::RaytracePipeline* GpuDevice::newRaytracePipeline(const ngfx::RaytracePipelineDesc * desc, ngfx::Result * result)
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
	ngfx::RaytracingAS* GpuDevice::newRaytracingAS(const ngfx::RaytracingASDesc * rtDesc, ngfx::Result * result)
	{
		return nullptr;
	}
	ngfx::Sampler* GpuDevice::newSampler(const ngfx::SamplerDesc * desc, ngfx::Result * result)
	{
		return nullptr;
	}
	ngfx::Fence* GpuDevice::newFence()
	{
		return nullptr;
	}
	ngfx::Result GpuDevice::wait()
	{
		return ngfx::Result();
	}

    void GpuDevice::preCreateDevice()
    {
        vkGetPhysicalDeviceFeatures(physical_device_, &features_);
        vkGetPhysicalDeviceProperties(physical_device_, &properties_);

        uint32_t num_dev_layers = 0;
        vkEnumerateDeviceLayerProperties(physical_device_, &num_dev_layers, nullptr);
        if (num_dev_layers > 0) {
            ngfx::Vec<VkLayerProperties> layer_props;
            layer_props.resize(num_dev_layers);
            vkEnumerateDeviceLayerProperties(physical_device_, &num_dev_layers, &layer_props[0]);

            // list device supported extensions
            uint32_t num_prop = 0;
            vkEnumerateDeviceExtensionProperties(physical_device_, nullptr, &num_prop, nullptr);
            if (num_prop > 0) {
                ngfx::Vec<VkExtensionProperties> ext_props;
                ext_props.resize(num_prop);
                vkEnumerateDeviceExtensionProperties(physical_device_, nullptr, &num_prop, &ext_props[0]);
            }
        }

        auto getQueueFamilyIndex = [&](VkQueueFlags queueFlag) {
            uint32_t queueFamilyPropertyCount;
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queueFamilyPropertyCount, nullptr);
            std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(physical_device_, &queueFamilyPropertyCount, queueFamilyProperties.data());
            if (queueFlag & VK_QUEUE_COMPUTE_BIT)
            {
                for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
                {
                    if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                        !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
                    {
                        return i;
                    }
                }
            }
            if (queueFlag & VK_QUEUE_TRANSFER_BIT)
            {
                for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
                {
                    if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                        !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                        !(queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
                    {
                        return i;
                    }
                }
            }
            for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i)
            {
                if (queueFamilyProperties[i].queueFlags & queueFlag)
                {
                    return i;
                }
            }
            return 0u;
        };

        queues_info_.graphics.queueFamilyIndex = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        queues_info_.compute.queueFamilyIndex = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
        queues_info_.transfer.queueFamilyIndex = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
    }

    void GpuDevice::createDevice()
    {
        VkDeviceCreateInfo create_info = {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0
        };
        vkCreateDevice(physical_device_, &create_info, &gAllocationCallbacks, &device_);
    }

    void GpuDevice::postCreateDevice()
    {
    }

    GpuQueue::GpuQueue()
    {
    }
    GpuQueue::~GpuQueue()
    {
    }
    ngfx::CommandBuffer* GpuQueue::newCommandBuffer()
    {
        return nullptr;
    }

    GpuFactory::GpuFactory(VkInstance instance, bool debug_enable, ngfx_LogCallback log_call)
        : instance_(instance)
        , debug_enable_(debug_enable)
        , log_call_(log_call)
    {
        volkLoadInstance(instance_);
        for (auto& physical_device : enumPhysicalDevices()) {
            iptr<GpuDevice> device(new GpuDevice(physical_device));

            devices_.push(device);
        }
        
    }

    GpuFactory::~GpuFactory()
    {
    }

    ngfx::Swapchain * GpuFactory::newSwapchain(void * handle, void * reserved)
    {
        return nullptr;
    }
    int GpuFactory::numDevices()
    {
        return devices_.num();
    }
    ngfx::Device * GpuFactory::getDevice(ngfx::uint32 id)
    {
        return devices_[id].get();
    }
    ngfx::Vec<VkPhysicalDevice> GpuFactory::enumPhysicalDevices()
    {
        ngfx::Vec<VkPhysicalDevice> ret;
        uint32_t num_devs = 0;
        vkEnumeratePhysicalDevices(instance_, &num_devs, nullptr);
        if (num_devs > 0) {
            ret.resize(num_devs);
            vkEnumeratePhysicalDevices(instance_, &num_devs, &ret[0]);
        }
        return ret;
    }
    void LayerEnumerator::init()
    {
        // list layers
        uint32_t num_layers = 0;
        vkEnumerateInstanceLayerProperties(&num_layers, nullptr);
        ngfx::Vec<VkLayerProperties> layer_props;
        if (num_layers > 0) {
            layer_props.resize(num_layers);
            vkEnumerateInstanceLayerProperties(&num_layers, &layer_props[0]);
            for (auto& layer_prop : layer_props) {
                layer_props_.push({ layer_prop.layerName, layer_prop });
            }
        }

        // list default instance extensions
        uint32_t num_props = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &num_props, nullptr);
        ngfx::Vec<VkExtensionProperties> ext_props;
        if (num_props > 0) {
            ext_props.resize(num_props);
            vkEnumerateInstanceExtensionProperties(nullptr, &num_props, &ext_props[0]);
            for (auto& ext_prop : ext_props) {
                ext_props_.push({ ext_prop.extensionName, ext_prop });
            }
        }
    }
}

ngfx::Factory* CreateFactory(bool debug_layer_enable, ngfx_LogCallback log_call)
{
    volkInitialize();
    // find all layers and extensions
    vulkan::gLayerEnumerator.init();

    VkInstance instance = VK_NULL_HANDLE;
    VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, 
        "ngfx_vulkan", 1,
        "ngfx", 1,
        volkGetInstanceVersion()
    };
    VkInstanceCreateInfo instanceInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr,
        0, &appInfo
    };

    if (debug_layer_enable)
    {
        //instanceInfo.enabledLayerCount = RequiredLayers.size();
        //instanceInfo.ppEnabledLayerNames = RequiredLayers.data();
    }

    //instanceCreateInfo.enabledExtensionCount = RequiredInstanceExtensions.size();
    //instanceCreateInfo.ppEnabledExtensionNames = RequiredInstanceExtensions.data();

    VkResult result = vkCreateInstance(&instanceInfo, &vulkan::gAllocationCallbacks, &instance);
    return new vulkan::GpuFactory(instance, debug_layer_enable, log_call);
}