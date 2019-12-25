#pragma once
#include "ngfx.h"

#ifdef BUILD_SHARED_LIB
#define NGFXU_API __declspec(dllexport)
#else
#define NGFXU_API __declspec(dllimport)
#endif

namespace ngfxu
{
    template <typename T>
    class NGFXU_API Handle {
    public:
        using Super = Handle<T>;

        Handle(T* obj) : ptr_(obj) {}

        Handle<T>& operator=(Handle<T> const& other) {
            ptr_ = (other.ptr_);
            return *this;
        }

    protected:
        ngfx::Ptr<T> ptr_;
    };

	class NGFXU_API Fence : public Handle<ngfx::Fence>
	{
	public:
		Fence(ngfx::Fence* fence) : Super(fence) {}
	};

	class NGFXU_API Drawable : public Handle<ngfx::Swapchain>
	{
	public:
		Drawable(ngfx::Swapchain* swapChain) : Super(swapChain) {}
	};

	class NGFXU_API RenderCommandEncoder : public Handle<ngfx::RenderEncoder>
	{
	public:
		RenderCommandEncoder(ngfx::RenderEncoder* renderEncoder) : Super(renderEncoder) {}

		void updateFence(Fence fence);
		void waitForFence(Fence fence);
		void presentDrawable(Drawable const& drawable);
		void endEncode();
	};

	class NGFXU_API ParallelRenderEncoder : public Handle<ngfx::ParallelEncoder>
	{
	public:
		ParallelRenderEncoder(ngfx::ParallelEncoder* parallelEncoder) : Super(parallelEncoder) {}

		RenderCommandEncoder subEncoder();
		void endEncode();
	};

	class NGFXU_API ComputeEncoder : public Handle<ngfx::ComputeEncoder>
	{
	public:
		ComputeEncoder(ngfx::ComputeEncoder* computeEncoder) : Super(computeEncoder) {}

		void updateFence(Fence fence);
		void waitForFence(Fence fence);
		void dispatch(int x, int y, int z);
		void endEncode();
	};

	class NGFXU_API CommandBuffer : public Handle<ngfx::CommandBuffer> {
	public:
		CommandBuffer(ngfx::CommandBuffer* cmdBuf) : Super(cmdBuf) {}

		RenderCommandEncoder newRenderEncoder(ngfx::RenderpassDesc const& rpDesc);
		ParallelRenderEncoder newParallelRenderEncoder(ngfx::RenderpassDesc const& rpDesc);
		ComputeEncoder newComputeEncoder();
		void commit();
		void release();
	};

    class NGFXU_API CommandQueue : public Handle<ngfx::CommandQueue> {
    public:
        CommandQueue(ngfx::CommandQueue* queue) : Super(queue) {}
		CommandBuffer obtainCommandBuffer();
    };

    class NGFXU_API RaytracingAccelerationStructure : public Handle<ngfx::RaytracingAS>
    {
    public:
        RaytracingAccelerationStructure(ngfx::RaytracingAS* as) : Super(as) {}
    };

    class NGFXU_API Buffer : public Handle<ngfx::Buffer> {
    public:
        Buffer(ngfx::Buffer* buffer) : Super(buffer) {}
    };

    class NGFXU_API Device : public Handle<ngfx::Device> {
    public:
        Device(ngfx::Device* device) : Super(device) {}
        CommandQueue newQueue() {
            ngfx::Result result;
            return CommandQueue(ptr_->newQueue(&result));
        }
        Buffer newBuffer(const ngfx::BufferDesc& desc, ngfx::StorageMode mode) {
            ngfx::Result result;
            return ptr_->newBuffer(&desc, mode, &result);
        }
        RaytracingAccelerationStructure newRaytracingAccelerationStructure(const ngfx::RaytracingASDesc& desc) {
            ngfx::Result result;
            return RaytracingAccelerationStructure(ptr_->newRaytracingAS(&desc, &result));
        }
		Fence newFence();
		void wait();
    };

    class NGFXU_API Factory : public Handle<ngfx::Factory> {
    public:
        Factory(ngfx::Factory* factory) : Super(factory) {
			factory->init();
		}
        Device getDevice(uint32_t id) { return Device(ptr_->getDevice(id)); }

		Drawable getDrawable();
    };
}