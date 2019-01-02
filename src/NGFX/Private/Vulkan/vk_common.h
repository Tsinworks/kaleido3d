/**
 * MIT License
 *
 * Copyright (c) 2019 Zhou Qin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <ngfx.h>

#include "volk.h"

VK_DEFINE_HANDLE(VmaAllocator)

#define NGFX_EXPORT __declspec(dllexport)

typedef void(*ngfx_LogCallback)(int level, const char* msg);
namespace vulkan {
	class GpuDevice;
	class GpuBuffer;
	class GpuTexture;

	template <class T>
	class iptr
	{
	public:
		iptr(T * pObj) : ptr_(pObj) {}
		iptr(iptr<T> const& Other) : ptr_(Other.ptr_) {
			if (ptr_) ptr_->retain_internal();
		}
		iptr() : ptr_(nullptr) {}
		~iptr() {
			if (ptr_) {
				ptr_->release_internal();
			    ptr_ = nullptr;
			}
		}
		T& operator*() const { return *ptr_; }
		T* operator->() const { return ptr_; }
		explicit operator bool() const { return ptr_ != nullptr; }
		void swap(iptr& Other) {
			T * const pValue = Other.ptr_;
			Other.ptr_ = ptr_;
			ptr_ = pValue;
		}
        iptr& operator=(const iptr& Other) {
			typedef iptr<T> ThisType;
			if (&Other != this) {
				ThisType(Other).swap(*this);
			}
			return *this;
		}
		T* get() const { return ptr_; }
		T** getAddressOf() { return &ptr_;}
	private:
		T* ptr_;
	};

    class Allocator
    {
    public:
        static void*   alloc(void* usr, size_t size, size_t align, VkSystemAllocationScope scope);
        static void*   realloc(void* usr, void* origin, size_t size, size_t align, VkSystemAllocationScope scope);
        static void    free(void* usr, void* ptr);

        static void    notifyAlloc(void* usr, size_t size, VkInternalAllocationType type, VkSystemAllocationScope scope);
        static void    notifyFree(void* usr, size_t size, VkInternalAllocationType type, VkSystemAllocationScope scope);

    public:
        Allocator();
        ~Allocator();

        void*   alloc(size_t size, size_t align, VkSystemAllocationScope scope);
        void*   realloc(void* origin, size_t size, size_t align, VkSystemAllocationScope scope);
        void    free(void* ptr);

        void    notifyAlloc(size_t size, VkInternalAllocationType type, VkSystemAllocationScope scope);
        void    notifyFree(size_t size, VkInternalAllocationType type, VkSystemAllocationScope scope);

    private:
    };

    class GpuAllocator
    {
        using MemProps = VkPhysicalDeviceMemoryProperties;
    public:
        GpuAllocator(GpuDevice* device);
        ~GpuAllocator();

    private:
        GpuDevice*                  device_;
        MemProps                    device_mem_props_;
    };

    class LayerEnumerator
    {
        using LayerMap = ngfx::HashMap<std::string, VkLayerProperties>;
        using ExtensionMap = ngfx::HashMap<std::string, VkExtensionProperties>;
    public:
        LayerEnumerator() = default;
        ~LayerEnumerator() = default;

        void                        init();

        bool                        hasLayer(std::string const& layer_name) const;
        bool                        hasExtension(std::string const& extension_name) const;

    private:
        LayerMap                    layer_props_;
        ExtensionMap                ext_props_;
    };

    extern Allocator                gAllocator;
    extern VkAllocationCallbacks    gAllocationCallbacks;
    extern LayerEnumerator          gLayerEnumerator;


    struct QueueInfo
    {
        int32_t queueFamilyIndex;
        VkQueue queue;
    };

    struct QueuesInfo
    {
        QueueInfo graphics;
        QueueInfo compute;
        QueueInfo transfer;
    };

	class GpuFactory : public ngfx::Factory
	{
	public:
        GpuFactory(VkInstance instance, bool enable_debug, ngfx_LogCallback log_call);
        ~GpuFactory() override;
        int                         numDevices() override;
        ngfx::Device *              getDevice(ngfx::uint32 id) override;
        void*                       newSurface(void* handle) override;
        bool                        debuggable() const;
    private:
        ngfx::Vec<VkPhysicalDevice> enumPhysicalDevices();

        VkInstance                  instance_;
        bool                        debug_enable_;
        ngfx_LogCallback            log_call_;
        ngfx::Vec<iptr<GpuDevice>>  devices_;
	};

	class GpuDevice : public ngfx::Device, public VolkDeviceTable
	{
        using RtProps = VkPhysicalDeviceRayTracingPropertiesNV;
        using HWProps = VkPhysicalDeviceProperties;
        using HWFeatures = VkPhysicalDeviceFeatures;
        using QueueProps = ngfx::Vec<VkQueueFamilyProperties>;
        using ExtensionMap = ngfx::HashMap<std::string, VkExtensionProperties>;

        struct DevicePropsScope
        {
            DevicePropsScope(VkPhysicalDevice device);
            ~DevicePropsScope();
            bool                    hasExtension(std::string const& ext) const;
            HWProps	                properties = {};
            HWFeatures	            features = {};
            QueuesInfo              queues_info = {};
        private:
            void                    init(VkPhysicalDevice device);
            int                     getQueueFamilyIndex(VkPhysicalDevice device, VkQueueFlags queueFlag);
            QueueProps              queue_props_;
            ExtensionMap            extensions_;
        };

	public:
		GpuDevice(VkPhysicalDevice device, GpuFactory* factory);
		~GpuDevice() override;

        void                        setLabel(const char * label) override;
        const char*                 label() const override;
        ngfx::DeviceType            getType() const override;
		ngfx::CommandQueue *		newQueue(ngfx::Result * result) override;
		ngfx::Shader *				newShader() override;
		ngfx::Renderpass *			newRenderpass(const ngfx::RenderpassDesc * desc, ngfx::Result * result) override;
		ngfx::ComputePipeline *		newComputePipeline(const ngfx::ComputePipelineDesc * desc, ngfx::Result * result) override;
		ngfx::RaytracePipeline *	newRaytracePipeline(const ngfx::RaytracePipelineDesc * desc, ngfx::Result * result) override;
		ngfx::Texture *				newTexture(const ngfx::TextureDesc * desc, ngfx::StorageMode mode, ngfx::Result * result) override;
		ngfx::Buffer *				newBuffer(const ngfx::BufferDesc * desc, ngfx::StorageMode mode, ngfx::Result * result) override;
		ngfx::RaytracingAS *		newRaytracingAS(const ngfx::RaytracingASDesc * rtDesc, ngfx::Result * result) override;
		ngfx::Sampler *				newSampler(const ngfx::SamplerDesc * desc, ngfx::Result * result) override;
		ngfx::Fence *				newFence(ngfx::Result * result) override;
        ngfx::Swapchain *           newSwapchain(const ngfx::SwapchainDesc * desc, const ngfx::Swapchain * old, void * surface, ngfx::Result * result) override;
		ngfx::Result				wait() override;

        bool                        isValid() const { return device_ != VK_NULL_HANDLE; }

        friend class                GpuCommandBuffer;
        friend class                GpuFactory;

    public:
        VkResult                    createAccelerationStructure(const VkAccelerationStructureCreateInfoNV* pCreateInfo, 
                                        const VkAllocationCallbacks* pAllocator, 
                                        VkAccelerationStructureNV* pAccelerationStructure);
        
        void                        destroyAccelerationStructure(VkAccelerationStructureNV accelerationStructure,
                                        const VkAllocationCallbacks * pAllocator);

        VkMemoryRequirements        getAccelerationStructureMemorySize(VkAccelerationStructureNV accel);
        VkResult                    createFence(const VkFenceCreateInfo& info, VkFence* pFence);
        void                        destroyFence(VkFence fence);

        struct SurfaceInfo
        {
            uint32_t present_family_index;
            uint32_t min_images;
            uint32_t max_images;
            ngfx::Vec<VkPresentModeKHR> present_modes;
            ngfx::Vec<VkSurfaceFormatKHR> formats;
        };

        bool                        querySurfaceInfo(VkSurfaceKHR surface, SurfaceInfo& info);

	private:
        void                        createDevice();
        void                        postCreateDevice();
        void                        initAllocator();
        void                        destroyAllocator();

        VkPhysicalDevice            physical_device_    = VK_NULL_HANDLE;
		VkDevice					device_             = VK_NULL_HANDLE;

        ngfx::DeviceType            device_type_        = ngfx::DeviceType::Integrate;
        bool                        is_mobile_gpu_      = false;
        bool                        support_raytracing_ = false;
        RtProps                     ray_tracing_props_  = {};

        VmaAllocator                memory_allocator_;

        iptr<GpuFactory>            factory_;
	};

    class GpuSwapchain : public ngfx::Swapchain {
    public:
        GpuSwapchain(GpuDevice* device);
        ~GpuSwapchain() override;

    private:
        VkSwapchainKHR              swapchain_;
        iptr<GpuDevice>             device_;
    };

    class GpuQueue : public ngfx::CommandQueue
	{
	public:
        GpuQueue(VkQueue queue, GpuDevice* device);
        ~GpuQueue() override;

		ngfx::CommandBuffer*        newCommandBuffer() override;

    private:
        VkQueue                     queue_;
        iptr<GpuDevice>             device_;
	};

    class GpuFence : public ngfx::Fence {
    public:
        GpuFence(GpuDevice* device);
        ~GpuFence() override;

        void                        setLabel(const char * label) override;
        const char*                 label() const override;
    private:
        VkFence                     fence_;
        iptr<GpuDevice>             device_;
    };

	class GpuCommandBuffer : public ngfx::CommandBuffer
	{
	public:
        GpuCommandBuffer(GpuQueue* queue);
        ~GpuCommandBuffer() override;

        ngfx::RenderEncoder *       newRenderEncoder(ngfx::Result * result) override;
        ngfx::ComputeEncoder *      newComputeEncoder(ngfx::Result * result) override;
        ngfx::BlitEncoder *         newBlitEncoder(ngfx::Result* result) override;
        ngfx::ParallelEncoder *     newParallelRenderEncoder(ngfx::Result* result) override;
        ngfx::RaytraceEncoder *     newRaytraceEncoder(ngfx::Result * result) override;
        ngfx::Result                commit() override;
    private:
        VkCommandBuffer             command_buffer_;
        iptr<GpuQueue>              queue_;
	};

    // Raytracing Commands support
    class GpuRaytracingEncoder : public ngfx::RaytraceEncoder
    {
    public:

        void buildAS(ngfx::RaytracingAS * src, ngfx::RaytracingAS * dest, ngfx::Buffer * scratch) override;
        void copyAS(ngfx::RaytracingAS * src, ngfx::RaytracingAS * dest, ngfx::AccelerationStructureCopyMode mode) override;
        void traceRay(ngfx::Buffer * rayGen, ngfx::BufferStride miss, ngfx::BufferStride hit, int width, int height) override;
    private:
        iptr<GpuCommandBuffer>      command_;
    };

    class ComputeEncoder : public ngfx::ComputeEncoder
    {
    public:

    };

    class RenderEncoder : public ngfx::RenderEncoder
    {
    public:

    };

    class GpuPipelineBase : public ngfx::Pipeline {
    public:
        GpuPipelineBase(GpuDevice* device);
        virtual ~GpuPipelineBase() override;

    protected:
        VkPipeline                  pipeline_       = VK_NULL_HANDLE;
        VkPipelineCache             pipeline_cache_ = VK_NULL_HANDLE;
        iptr<GpuDevice>             device_;
    };

    class GpuRaytracingPipeline : public GpuPipelineBase {
        using CreateInfo = VkRayTracingPipelineCreateInfoNV;
    public:
        GpuRaytracingPipeline(const ngfx::RaytracePipelineDesc* desc, GpuDevice* device);
        ~GpuRaytracingPipeline() override;

    private:
        CreateInfo                  create_info_    = {};
        ngfx::RaytracePipelineDesc  desc_           = {};
    };

    class GpuRenderPipeline : public GpuPipelineBase {
        using CreateInfo = VkGraphicsPipelineCreateInfo;
    public:

    private:
        CreateInfo                  create_info_    = {};
        ngfx::RenderPipelineDesc    desc_           = {};
    };

    class GpuComputePipeline : public GpuPipelineBase {
        using CreateInfo = VkComputePipelineCreateInfo;
    public:

    private:
        CreateInfo                  create_info_    = {};
        ngfx::ComputePipelineDesc   desc_           = {};
    };

    class GpuBuffer : public ngfx::Buffer
    {
    public:
        GpuBuffer(const ngfx::BufferDesc& desc, GpuDevice* device);
        ~GpuBuffer() override;
        ngfx::BufferView*           newView(ngfx::Result * result) override;
        bool                        isValid() const { return buffer_ != VK_NULL_HANDLE; }
    private:
        VkBuffer                    buffer_ = VK_NULL_HANDLE;
        VkBufferCreateInfo          create_info_;
        ngfx::BufferDesc            desc_;
        iptr<GpuDevice>             device_;
    };

    class GpuTexture : public ngfx::Texture
    {
    public:
        GpuTexture(const ngfx::TextureDesc& desc, GpuDevice* device);
        ~GpuTexture() override;
        ngfx::TextureView*          newView(ngfx::Result * result) override;
        bool                        isValid() const { return texture_ != VK_NULL_HANDLE; }

    private:
        VkImage                     texture_        = VK_NULL_HANDLE;
        VkImageCreateInfo           create_info_    = {};
        ngfx::TextureDesc           desc_           = {};
        iptr<GpuDevice>             device_;
    };

    class GpuSampler : public ngfx::Sampler
    {
    public:
        GpuSampler(const ngfx::SamplerDesc& desc, GpuDevice* device);
        ~GpuSampler() override;
        bool                        isValid() const { return sampler_ != VK_NULL_HANDLE; }
        void                        setLabel(const char* label);
        const char*                 label() const override;
    private:
        VkSampler                   sampler_        = VK_NULL_HANDLE;
        VkSamplerCreateInfo         create_info_    = {};
        ngfx::SamplerDesc           desc_           = {};
        iptr<GpuDevice>             device_;
    };

    class GpuRaytracingAccelerationStructure : public ngfx::RaytracingAS
    {
        using CreateInfo = VkAccelerationStructureCreateInfoNV;
    public:
        GpuRaytracingAccelerationStructure(const ngfx::RaytracingASDesc& desc, GpuDevice* device);
        ~GpuRaytracingAccelerationStructure() override;

        bool                        isValid() const { return acceleration_structure_ != VK_NULL_HANDLE; }
        void                        setLabel(const char* label);
        const char*                 label() const override;

    private:
        VkAccelerationStructureNV   acceleration_structure_ = VK_NULL_HANDLE;
        VkDeviceMemory              memory_                 = VK_NULL_HANDLE;
        ngfx::RaytracingASDesc      desc_                   = {};
        CreateInfo                  create_info_            = {};
        iptr<GpuDevice>             device_;
    };
}
extern "C" 
{
    NGFX_EXPORT ngfx::Factory* CreateFactory(bool debug_layer_enable, ngfx_LogCallback log_call);
}