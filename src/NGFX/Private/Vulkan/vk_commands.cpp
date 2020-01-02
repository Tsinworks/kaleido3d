#include "vk_common.h"
#include "vk_commands.h"
/*
 Buffer views and texture views are created when encoding commands.
 When binding to fix slot,
 create the right resource view for slot, 
 then insert cooresponding resource barrier in need.
 */
namespace vulkan {
	CommandContext::CommandContext(CommandAllocator& allocator)
	: allocator_(allocator) {
	}

	CommandContext::~CommandContext()
	{
	}

	void CommandContext::Flush()
	{
		CommandType cmd_type = CommandType::Noop;
		while (NextCommandType(cmd_type))
		{
			switch (cmd_type)
			{
			case CommandType::Noop:
				break;
			case CommandType::SetPipeline:
				NextCommand<GfxCmdSetPipeline>();
				break;
			case CommandType::SetBindGroup:
				NextCommand<GfxCmdSetBindGroup>();
				break;
			case CommandType::SetIAVertexBuffer:
				NextCommand<GfxCmdSetIAVertexBuffer>();
				break;
			case CommandType::SetViewport:
				NextCommand<GfxCmdSetViewport>();
				break;
			case CommandType::SetViewports:
				NextCommand<GfxCmdSetViewports>();
				break;
			case CommandType::SetScissors:
				NextCommand<GfxCmdSetScissors>();
				break;
			case CommandType::SetStencilRef:
				NextCommand<GfxCmdSetStencilRef>();
				break;
				//case CommandType::SetBlendFactor:
				//	NextCommand<GfxCmdSetPipeline>();
				//	break;
			case CommandType::SetShadingRate:
				NextCommand<GfxCmdSetShadingRate>();
				break;
			case CommandType::DrawPrimitives:
				NextCommand<GfxCmdDrawPrimitives>();
				break;
			case CommandType::DrawIndexedPrimitives:
				NextCommand<GfxCmdDrawIndexedPrimitives>();
				break;
			case CommandType::DrawIndirect:
				NextCommand<GfxCmdDrawIndirect>();
				break;
			case CommandType::Present:
				NextCommand<GfxCmdPresent>();
				break;
			case CommandType::DispatchCompute:
				NextCommand<GfxCmdDispatchCompute>();
				break;
				/*case CommandType::DispatchRays:
					break;
				case CommandType::DispatchMesh:
					break;
				case CommandType::NumMaxCommands:
					break;*/
			default:
				break;
			}
		}
	}

	void CommandContext::Reset()
	{

	}

	bool CommandContext::NextCommandType(CommandType& cmd_type)
	{
		return false;
	}

	CommandAllocator::CommandAllocator() {}
	CommandAllocator::~CommandAllocator() {}

	void* CommandAllocator::allocate(size_t size, size_t alignment)
	{
		return nullptr;
	}

	void CommandAllocator::free(void* ptr)
	{
	}

	void CommandAllocator::trimBlocks()
	{
	}

	RenderEncoder::RenderEncoder(CommandContext& context)
		: context_(context)
	{
	}

	RenderEncoder::~RenderEncoder()
	{

	}

	void RenderEncoder::setPipeline(ngfx::Pipeline* pipeline)
	{

	}

	void RenderEncoder::setBindGroup(const ngfx::BindGroup* bindGroup)
	{

	}

	void RenderEncoder::drawPrimitives(ngfx::PrimitiveType primType, int vertexStart, int vertexCount, int instanceCount, int baseInstance)
	{
		auto& arg = context_.NewCmd<GfxCmdDrawPrimitives>();
		arg.primType = primType;
		arg.vertexStart = vertexStart;
		arg.vertexCount = vertexCount;
		arg.instanceCount = instanceCount;
		arg.baseInstance = baseInstance;
		// flush cmds?
	}

	void RenderEncoder::drawIndexedPrimitives(ngfx::PrimitiveType primType, 
		ngfx::IndexType indexType, int indexCount, const ngfx::Buffer* indexBuffer, int indexBufferOffset, 
		int vertexStart, int vertexCount, int instanceCount, int baseInstance)
	{
		auto& arg = context_.NewCmd<GfxCmdDrawIndexedPrimitives>();
		arg.primType = primType;
		arg.indexType = indexType;
		arg.indexCount = indexCount;
		arg.indexBuffer = indexBuffer;
		arg.indexBufferOffset = indexBufferOffset;
		arg.vertexStart = vertexStart;
		arg.vertexCount = vertexCount;
		arg.instanceCount = instanceCount;
		arg.baseInstance = baseInstance;
		// flush cmds?
	}

	void RenderEncoder::drawIndirect(ngfx::PrimitiveType primType, const ngfx::Buffer * buffer, uint64_t offset, uint32_t drawCount, uint32_t stride)
	{
		auto& arg = context_.NewCmd<GfxCmdDrawIndirect>();
		arg.primType = primType;
		arg.buffer = buffer;
		arg.offset = offset;
		arg.drawCount = drawCount;
		arg.stride = stride;
		// flush cmds?
	}

	void RenderEncoder::endEncode()
	{
		context_.Flush();
	}

	ComputeEncoder::ComputeEncoder(CommandContext& context)
		: context_(context)
	{

	}

	ComputeEncoder::~ComputeEncoder()
	{

	}

	void ComputeEncoder::setPipeline(ngfx::Pipeline* pipeline)
	{

	}

	void ComputeEncoder::setBindGroup(const ngfx::BindGroup* bindGroup)
	{

	}

	void ComputeEncoder::dispatch(int thread_group_x, int thread_group_y, int thread_group_z)
	{
		auto& arg = context_.NewCmd<GfxCmdDispatchCompute>();
		arg.threadGroupX = thread_group_x;
		arg.threadGroupY = thread_group_y;
		arg.threadGroupZ = thread_group_z;
	}

	void ComputeEncoder::endEncode()
	{

	}

    GpuCommandBuffer::GpuCommandBuffer(GpuQueue * queue)
    {
    }
    
	GpuCommandBuffer::~GpuCommandBuffer()
    {
    }

    ngfx::RenderEncoder* GpuCommandBuffer::newRenderEncoder(ngfx::Result* result)
    {
        return nullptr;
    }

    ngfx::ComputeEncoder* GpuCommandBuffer::newComputeEncoder(ngfx::Result* result)
    {
        return nullptr;
    }

    ngfx::BlitEncoder* GpuCommandBuffer::newBlitEncoder(ngfx::Result* result)
    {
        return nullptr;
    }

    ngfx::ParallelEncoder* GpuCommandBuffer::newParallelRenderEncoder(ngfx::Result* result)
    {
        return nullptr;
    }

    ngfx::RaytraceEncoder* GpuCommandBuffer::newRaytraceEncoder(ngfx::Result* result)
    {
        return nullptr;
    }

    ngfx::Result GpuCommandBuffer::commit()
    {
        return ngfx::Result();
    }
}