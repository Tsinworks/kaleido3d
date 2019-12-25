#include "ngfx_shell.h"

namespace ngfxu
{
	Drawable Factory::getDrawable()
	{
		return Drawable(nullptr);
	}
	RenderCommandEncoder ParallelRenderEncoder::subEncoder()
	{
		return RenderCommandEncoder(ptr_->subRenderEncoder(nullptr));
	}
	void ParallelRenderEncoder::endEncode()
	{
	}
	RenderCommandEncoder CommandBuffer::newRenderEncoder(ngfx::RenderpassDesc const& rpDesc)
	{
		return RenderCommandEncoder(ptr_->newRenderEncoder(nullptr));
	}
	ParallelRenderEncoder CommandBuffer::newParallelRenderEncoder(ngfx::RenderpassDesc const & rpDesc)
	{
		return ParallelRenderEncoder(ptr_->newParallelRenderEncoder(nullptr));
	}
	ComputeEncoder CommandBuffer::newComputeEncoder()
	{
		return ComputeEncoder(ptr_->newComputeEncoder(nullptr));
	}
	void CommandBuffer::commit()
	{
	}
	void CommandBuffer::release()
	{
	}
	CommandBuffer CommandQueue::obtainCommandBuffer()
	{
		return CommandBuffer(ptr_->newCommandBuffer());
	}
	void RenderCommandEncoder::updateFence(Fence fence)
	{
	}
	void RenderCommandEncoder::waitForFence(Fence fence)
	{
	}
	void RenderCommandEncoder::presentDrawable(Drawable const & drawable)
	{
	}
	void RenderCommandEncoder::endEncode()
	{
	}
	void ComputeEncoder::updateFence(Fence fence)
	{
	}
	void ComputeEncoder::waitForFence(Fence fence)
	{
	}
	void ComputeEncoder::dispatch(int x, int y, int z)
	{
	}
	void ComputeEncoder::endEncode()
	{
	}
	Fence Device::newFence()
	{
		return Fence(ptr_->newFence(nullptr));
	}
	void Device::wait()
	{
	}
}