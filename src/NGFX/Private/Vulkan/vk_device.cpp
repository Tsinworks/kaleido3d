#if _WIN32
#define VK_KHR_win32_surface 1
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif

#include "vk_common.h"
#include <unordered_map>

#define VULKAN_STANDARD_LAYER "VK_LAYER_LUNARG_standard_validation"

namespace vulkan {
    LayerEnumerator gLayerEnumerator;

	GpuDevice::GpuDevice(VkPhysicalDevice device, GpuFactory* factory)
        : physical_device_(device)
        , factory_(factory)
	{
	}

	GpuDevice::~GpuDevice()
	{
        destroyAllocator();
	}
    void GpuDevice::setLabel(const char * label)
    {
    }
    const char * GpuDevice::label() const
    {
        return nullptr;
    }
    ngfx::DeviceType GpuDevice::getType() const
    {
        return device_type_;
    }
	ngfx::CommandQueue* GpuDevice::newQueue(ngfx::Result * result)
	{
        VkQueue queue = VK_NULL_HANDLE;
        this->vkGetDeviceQueue(device_, 0, 0, &queue);
		return new GpuQueue(queue, this);
	}
    
    GpuFence::GpuFence(GpuDevice * device)
        : device_(device)
    {
        VkFenceCreateInfo create_info = {
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO
        };
        device_->createFence(create_info, &fence_);
    }

    GpuFence::~GpuFence()
    {
        device_->destroyFence(fence_);
        fence_ = VK_NULL_HANDLE;
    }

    void GpuFence::setLabel(const char * label)
    {
    }

    const char * GpuFence::label() const
    {
        return nullptr;
    }

    VkResult GpuDevice::createFence(const VkFenceCreateInfo & info, VkFence * pFence)
    {
        return this->vkCreateFence(device_, &info, &gAllocationCallbacks, pFence);
    }
    void GpuDevice::destroyFence(VkFence fence)
    {
        this->vkDestroyFence(device_, fence, &gAllocationCallbacks);
    }

    bool GpuDevice::querySurfaceInfo(VkSurfaceKHR surface, GpuDevice::SurfaceInfo& info)
    {
        //VkBool32 supportPresent;
        //vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_, 
        //    queues_info.graphics.queueFamilyIndex, 
        //    surface, 
        //    &supportPresent);
        uint32_t count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_, surface, &count, nullptr);
        info.formats.resize(count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device_, surface, &count, &info.formats[0]);
        count = 0;
        VkSurfaceCapabilitiesKHR surface_caps = {};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device_, surface, &surface_caps);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_, surface, &count, nullptr);
        info.present_modes.resize(count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device_, surface, &count, &info.present_modes[0]);
        return true;
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
	ngfx::Fence* GpuDevice::newFence(ngfx::Result * result)
	{
		return new GpuFence(this);
	}
	ngfx::Result GpuDevice::wait()
	{
		return ngfx::Result();
	}

    void GpuDevice::DevicePropsScope::init(VkPhysicalDevice device)
    {
        vkGetPhysicalDeviceFeatures(device, &features);
        vkGetPhysicalDeviceProperties(device, &properties);

        uint32_t queue_family_count;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        queue_props_.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, &queue_props_[0]);

        uint32_t num_dev_layers = 0;
        vkEnumerateDeviceLayerProperties(device, &num_dev_layers, nullptr);
        if (num_dev_layers > 0) {
            ngfx::Vec<VkLayerProperties> layer_props;
            layer_props.resize(num_dev_layers);
            vkEnumerateDeviceLayerProperties(device, &num_dev_layers, &layer_props[0]);

            // list device supported extensions
            uint32_t num_prop = 0;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &num_prop, nullptr);
            if (num_prop > 0) {
                ngfx::Vec<VkExtensionProperties> ext_props;
                ext_props.resize(num_prop);
                vkEnumerateDeviceExtensionProperties(device, nullptr, &num_prop, &ext_props[0]);
                for (auto& ext_prop : ext_props) {
                    extensions_.push({ ext_prop.extensionName, ext_prop });
                }
            }
        }

        queues_info.graphics.queueFamilyIndex = getQueueFamilyIndex(device, VK_QUEUE_GRAPHICS_BIT);
        queues_info.compute.queueFamilyIndex = getQueueFamilyIndex(device, VK_QUEUE_COMPUTE_BIT);
        queues_info.transfer.queueFamilyIndex = getQueueFamilyIndex(device, VK_QUEUE_TRANSFER_BIT);
    }

    bool GpuDevice::DevicePropsScope::hasExtension(std::string const & ext) const
    {
        return extensions_.contains(ext);
    }

    GpuDevice::DevicePropsScope::DevicePropsScope(VkPhysicalDevice device)
    {
        init(device);
    }

    GpuDevice::DevicePropsScope::~DevicePropsScope()
    {
        extensions_.clear();
        queue_props_.clear();
    }

    int GpuDevice::DevicePropsScope::getQueueFamilyIndex(VkPhysicalDevice device, VkQueueFlags queueFlag)
    {
        if (queueFlag & VK_QUEUE_COMPUTE_BIT)
        {
            for (uint32_t i = 0; i < queue_props_.num(); ++i)
            {
                if ((queue_props_[i].queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                    !(queue_props_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
                {
                    return i;
                }
            }
        }
        if (queueFlag & VK_QUEUE_TRANSFER_BIT)
        {
            for (uint32_t i = 0; i < queue_props_.num(); ++i)
            {
                if ((queue_props_[i].queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                    !(queue_props_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                    !(queue_props_[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
                {
                    return i;
                }
            }
        }
        for (uint32_t i = 0; i < queue_props_.num(); ++i)
        {
            if (queue_props_[i].queueFlags & queueFlag)
            {
                return i;
            }
        }
        return 0;
    }

    VkResult GpuDevice::createAccelerationStructure(
        const VkAccelerationStructureCreateInfoNV * pCreateInfo, 
        const VkAllocationCallbacks * pAllocator, 
        VkAccelerationStructureNV * pAccelerationStructure)
    {
        if (this->vkCreateAccelerationStructureNV) {
            return this->vkCreateAccelerationStructureNV(device_, pCreateInfo, pAllocator, pAccelerationStructure);
        }
        return VkResult::VK_ERROR_INITIALIZATION_FAILED;
    }

    void GpuDevice::destroyAccelerationStructure(VkAccelerationStructureNV accelerationStructure, const VkAllocationCallbacks * pAllocator)
    {
        if (this->vkDestroyAccelerationStructureNV) {
            this->vkDestroyAccelerationStructureNV(device_, accelerationStructure, pAllocator);
        }
    }


    void GpuDevice::createDevice()
    {
        DevicePropsScope props_scope(physical_device_);

        std::vector<const char*> required_extensions;
        if (vkGetPhysicalDeviceProperties2) {
            VkPhysicalDeviceDescriptorIndexingFeaturesEXT descriptorIndexing = { };
            descriptorIndexing.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
            VkPhysicalDeviceFeatures2 features2 = { };
            features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
            if (props_scope.hasExtension(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME)) {
                features2.pNext = &descriptorIndexing;
                vkGetPhysicalDeviceFeatures2(physical_device_, &features2);
                required_extensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);
            }
            if (props_scope.hasExtension(VK_NV_RAY_TRACING_EXTENSION_NAME)) {
                ray_tracing_props_.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PROPERTIES_NV;
                ray_tracing_props_.pNext = nullptr;
                ray_tracing_props_.maxRecursionDepth = 0;
                ray_tracing_props_.shaderGroupHandleSize = 0;
                VkPhysicalDeviceProperties2 props;
                props.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
                props.pNext = &ray_tracing_props_;
                props.properties = { };
                vkGetPhysicalDeviceProperties2(physical_device_, &props);
                support_raytracing_ = true;
                required_extensions.push_back(VK_NV_RAY_TRACING_EXTENSION_NAME);
            }
        }

        std::vector<VkDeviceQueueCreateInfo> queue_infos;
        float priority = 0.0f;
        VkDeviceQueueCreateInfo queue_info = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        queue_info.queueFamilyIndex = props_scope.queues_info.graphics.queueFamilyIndex;
        queue_info.queueCount = 1;
        queue_info.pQueuePriorities = &priority;
        queue_infos.push_back(queue_info);
        if (props_scope.queues_info.graphics.queueFamilyIndex != props_scope.queues_info.compute.queueFamilyIndex)
        {
            queue_info.queueFamilyIndex = props_scope.queues_info.compute.queueFamilyIndex;
            queue_infos.push_back(queue_info);
        }
        if (props_scope.queues_info.transfer.queueFamilyIndex != props_scope.queues_info.graphics.queueFamilyIndex &&
            props_scope.queues_info.transfer.queueFamilyIndex != props_scope.queues_info.compute.queueFamilyIndex)
        {
            queue_info.queueFamilyIndex = props_scope.queues_info.transfer.queueFamilyIndex;
            queue_infos.push_back(queue_info);
        }
        if (factory_->debuggable()) {

        }
        VkDeviceCreateInfo device_info = {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr, 0,
            queue_infos.size(), queue_infos.data(),// queueCreateInfoCount queueCreateInfo
            0, nullptr,// layerCount, layer
            required_extensions.size(), required_extensions.data(),// extensionCount, extensions
            &props_scope.features// features
        };
        VkResult ret = vkCreateDevice(physical_device_, &device_info, &gAllocationCallbacks, &device_);
        volkLoadDeviceTable(this, device_);
        // detect extensions
        postCreateDevice();
    }

    void GpuDevice::postCreateDevice()
    {
        initAllocator();
    }

    GpuQueue::GpuQueue(VkQueue queue, GpuDevice* device)
        : queue_(queue)
        , device_(device)
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
            auto device = new GpuDevice(physical_device, this);
            iptr<GpuDevice> ptr_device(device);
            device->createDevice();
            if (device->isValid()) {
                devices_.push(ptr_device);
            }
        }
    }

    GpuFactory::~GpuFactory()
    {
    }

    int GpuFactory::numDevices()
    {
        return devices_.num();
    }
    ngfx::Device * GpuFactory::getDevice(ngfx::uint32 id)
    {
        return devices_[id].get();
    }
    void* GpuFactory::newSurface(void * handle)
    {
        VkSurfaceKHR surface;
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
        //surfaceCreateInfo.hinstance = _windowInfo.WindowInstance;
        surfaceCreateInfo.hwnd = reinterpret_cast<HWND>(handle);
        VkResult code = vkCreateWin32SurfaceKHR(instance_, &surfaceCreateInfo, &gAllocationCallbacks, &surface);

        //PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
        //NVVK_RESOLVE_INSTANCE_FUNCTION_ADDRESS(_instance, vkGetPhysicalDeviceSurfaceSupportKHR);

        //VkBool32 supportPresent = VK_FALSE;
        //code = vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, _queuesInfo.Graphics.QueueFamilyIndex, _surface, &supportPresent);
        /*
        NVVK_CHECK_ERROR(code, L"vkGetPhysicalDeviceSurfaceSupportKHR");
        if (!supportPresent)
        {
            ExitError(L"Graphics queue does not support presenting");
        }
        */

       /* 
       PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
        NVVK_RESOLVE_INSTANCE_FUNCTION_ADDRESS(_instance, vkGetPhysicalDeviceSurfaceFormatsKHR);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &formatCount, nullptr);
        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice, _surface, &formatCount, surfaceFormats.data());

        if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            _surfaceFormat.format = _settings.DesiredSurfaceFormat;
            _surfaceFormat.colorSpace = surfaceFormats[0].colorSpace;
        }
        else
        {
            bool found = false;
            for (auto& surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == _settings.DesiredSurfaceFormat)
                {
                    _surfaceFormat = surfaceFormat;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                _surfaceFormat = surfaceFormats[0];
            }
        }*/
        return (void*)surface;
    }
    bool GpuFactory::debuggable() const
    {
        return debug_enable_;
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
    bool LayerEnumerator::hasLayer(std::string const & layer_name) const
    {
        return layer_props_.contains(layer_name);
    }
    bool LayerEnumerator::hasExtension(std::string const & extension_name) const
    {
        return ext_props_.contains(extension_name);
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
    std::vector<const char*> required_layers;
    std::vector<const char*> required_extensions;
    if (debug_layer_enable) {
        if (vulkan::gLayerEnumerator.hasLayer(VULKAN_STANDARD_LAYER)) {
            required_layers.push_back(VULKAN_STANDARD_LAYER);
        }
        if (vulkan::gLayerEnumerator.hasExtension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME)) {
            required_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        }
    }
    instanceInfo.enabledLayerCount = required_layers.size();
    instanceInfo.ppEnabledLayerNames = required_layers.data();
    instanceInfo.enabledExtensionCount = required_extensions.size();
    instanceInfo.ppEnabledExtensionNames = required_extensions.data();

    VkResult result = vkCreateInstance(&instanceInfo, &vulkan::gAllocationCallbacks, &instance);
    return new vulkan::GpuFactory(instance, debug_layer_enable, log_call);
}