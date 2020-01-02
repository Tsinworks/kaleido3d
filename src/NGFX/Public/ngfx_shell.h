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

        Handle(T* obj = nullptr) : ptr_(obj) {}

        Handle<T>& operator=(Handle<T> const& other) {
            ptr_ = (other.ptr_);
            return *this;
        }

		T* iptr() { return ptr_.get(); }

    protected:
        ngfx::Ptr<T> ptr_;
    };

	class NGFXU_API Fence : public Handle<ngfx::Fence>
	{
	public:
		Fence(ngfx::Fence* fence) : Super(fence) {}
	};

	class NGFXU_API Texture : public Handle<ngfx::Texture>
	{
	public:
		Texture(ngfx::Texture* texture) : Super(texture) {}

	};

	class NGFXU_API Drawable : public Handle<ngfx::Drawable>
	{
	public:
		Drawable(ngfx::Drawable* drawable) : Super(drawable) {}

		Texture texture();
	};

	class NGFXU_API PresentLayer : public Handle<ngfx::PresentLayer>
	{
	public:
		PresentLayer(ngfx::PresentLayer* layer = nullptr) : Super(layer) {}

		Drawable nextDrawable();
	};

	class NGFXU_API RenderPipeline : public Handle<ngfx::RenderPipeline>
	{
	public:
		RenderPipeline(ngfx::RenderPipeline* pipeline = nullptr) : Super(pipeline) {}
	};

	class NGFXU_API ComputePipeline : public Handle<ngfx::ComputePipeline>
	{
	public:
		ComputePipeline(ngfx::ComputePipeline* pipeline = nullptr) : Super(pipeline) {}
	};

	class NGFXU_API RenderCommandEncoder : public Handle<ngfx::RenderEncoder>
	{
	public:
		RenderCommandEncoder(ngfx::RenderEncoder* renderEncoder) : Super(renderEncoder) {}
		void setPipeline(const RenderPipeline& render_pipeline);
		void setBindGroup();
		
		void drawPrimitive();
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
		void setPipeline(const ComputePipeline& pipeline);
		void setBindGroup();
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
        CommandQueue(ngfx::CommandQueue* queue = nullptr) : Super(queue) {}
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
        Device(ngfx::Device* device = nullptr) : Super(device) {}
        CommandQueue newQueue() {
            ngfx::Result result;
            return CommandQueue(ptr_->newQueue(&result));
        }
        Buffer newBuffer(const ngfx::BufferDesc& desc, ngfx::StorageMode mode) {
            ngfx::Result result;
            return ptr_->newBuffer(&desc, mode, &result);
        }

		RenderPipeline newRenderPipeline(const ngfx::RenderPipelineDesc& desc);
		ComputePipeline newComputePipeline(const ngfx::ComputePipelineDesc& desc);

        RaytracingAccelerationStructure newRaytracingAccelerationStructure(const ngfx::RaytracingASDesc& desc) {
            ngfx::Result result;
            return RaytracingAccelerationStructure(ptr_->newRaytracingAS(&desc, &result));
        }
		Fence newFence();

		void compileShaderSource();
		void compileShaderLibrary();

		void wait();
    };

    class NGFXU_API Factory : public Handle<ngfx::Factory> {
    public:
        Factory(ngfx::Factory* factory) : Super(factory) {
		}
        Device getDevice(uint32_t id) { return Device(ptr_->getDevice(id)); }

		PresentLayer newPresentLayer(ngfx::PresentLayerDesc const& desc, Device device);

		Drawable getDrawable();
    };

	ngfx::RenderpassDesc CreateRenderpassDesc();
}