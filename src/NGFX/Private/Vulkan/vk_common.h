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
		explicit iptr(T * pObj) : ptr_(pObj) {}
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

    class LayerEnumerator
    {
        using LayerMap = ngfx::HashMap<std::string, VkLayerProperties>;
        using ExtensionMap = ngfx::HashMap<std::string, VkExtensionProperties>;
    public:
        LayerEnumerator() = default;
        ~LayerEnumerator() = default;

        void init();

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
        ngfx::Swapchain *           newSwapchain(void * handle, void * reserved) override;
        int                         numDevices() override;
        ngfx::Device *              getDevice(ngfx::uint32 id) override;

    private:
        ngfx::Vec<VkPhysicalDevice> enumPhysicalDevices();

        VkInstance                  instance_;
        bool                        debug_enable_;
        ngfx_LogCallback            log_call_;
        ngfx::Vec<iptr<GpuDevice>>  devices_;
	};

	class GpuDevice : public ngfx::Device, public VolkDeviceTable
	{
	public:
		GpuDevice(VkPhysicalDevice device);
		~GpuDevice() override;

        void                        set_label(const char * label) override;
        const char*                 label() const override;
        ngfx::DeviceType            getType() const override;
		ngfx::CommandQueue *		newQueue() override;
		ngfx::Shader *				newShader() override;
		ngfx::Renderpass *			newRenderpass(const ngfx::RenderpassDesc * desc, ngfx::Result * result) override;
		ngfx::ComputePipeline *		newComputePipeline(const ngfx::ComputePipelineDesc * desc, ngfx::Result * result) override;
		ngfx::RaytracePipeline *	newRaytracePipeline(const ngfx::RaytracePipelineDesc * desc, ngfx::Result * result) override;
		ngfx::Texture *				newTexture(const ngfx::TextureDesc * desc, ngfx::StorageMode mode, ngfx::Result * result) override;
		ngfx::Buffer *				newBuffer(const ngfx::BufferDesc * desc, ngfx::StorageMode mode, ngfx::Result * result) override;
		ngfx::RaytracingAS *		newRaytracingAS(const ngfx::RaytracingASDesc * rtDesc, ngfx::Result * result) override;
		ngfx::Sampler *				newSampler(const ngfx::SamplerDesc * desc, ngfx::Result * result) override;
		ngfx::Fence *				newFence() override;
		ngfx::Result				wait() override;

        friend class                GpuCommandBuffer;

	private:
        void                        preCreateDevice();
        void                        createDevice();
        void                        postCreateDevice();

        ngfx::Vec<const char*>      device_extensions_;
        VkPhysicalDevice            physical_device_ = VK_NULL_HANDLE;
		VkDevice					device_ = VK_NULL_HANDLE;
		VkPhysicalDeviceProperties	properties_ = {};
		VkPhysicalDeviceFeatures	features_ = {};
        QueuesInfo                  queues_info_ = {};
        ngfx::DeviceType            device_type_ = ngfx::DeviceType::Integrate;
        bool                        is_mobile_gpu_ = false;
        bool                        support_raytracing_ = false;
        VkPhysicalDeviceRayTracingPropertiesNV ray_tracing_props_ = { };
	};

    class GpuQueue : public ngfx::CommandQueue
	{
	public:
        GpuQueue();
        ~GpuQueue() override;

		ngfx::CommandBuffer*        newCommandBuffer() override;

    private:
        VkQueue                     queue_;
        iptr<GpuDevice>             device_;
	};

	class GpuCommandBuffer : public ngfx::CommandBuffer
	{
	public:
        ngfx::RenderEncoder *       newRenderEncoder(ngfx::Result * result) override;
        ngfx::ComputeEncoder *      newComputeEncoder(ngfx::Result * result) override;
        ngfx::Result                newBlitEncoder() override;
        ngfx::Result                newParallelRenderEncoder() override;
        ngfx::RaytraceEncoder *     newRaytraceEncoder(ngfx::Result * result) override;
        ngfx::Result                commit() override;
    private:
        VkCommandBuffer             command_buffer_;
        GpuQueue*                   queue_;
	};

    class GpuBuffer : public ngfx::Buffer
    {
    public:
        ~GpuBuffer() override;
        ngfx::BufferView*           newView(ngfx::Result * result) override;
    private:
        VkBuffer                    buffer_;
        VkBufferCreateInfo          create_info_;
        ngfx::BufferDesc            desc_;
        iptr<GpuDevice>             device_;
    };

    class GpuTexture : public ngfx::Texture
    {
    public:
        ~GpuTexture() override;
        ngfx::TextureView*          newView(ngfx::Result * result) override;
    private:
        VkImage                     texture_;
        VkImageCreateInfo           create_info_;
        ngfx::TextureDesc           desc_;
        iptr<GpuDevice>             device_;
    };
}
extern "C" 
{
    NGFX_EXPORT ngfx::Factory* CreateFactory(bool debug_layer_enable, ngfx_LogCallback log_call);
}