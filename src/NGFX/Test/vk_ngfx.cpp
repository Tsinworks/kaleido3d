#include "Public/ngfx_shell.h"
#include "CoreMinimal.h"

static void log_print(int level, const char* msg)
{

}

typedef ngfx::Factory* (*fnCreate)(bool debug, decltype(log_print) call);
/*
 * Metal memoryBarrierWithResources
 */
int main(int argc, char**argv) {
    k3d::os::LibraryLoader loader("Kaleido3D.NGFX.dll");
    fnCreate create = (fnCreate)loader.ResolveSymbol("CreateFactory");
    ngfxu::Factory factory(create(true, log_print));

	ngfxu::Drawable presentDrawable = factory.getDrawable();

	ngfxu::Device device = factory.getDevice(0);
	ngfxu::Fence fence = device.newFence();
	ngfxu::CommandQueue queue = device.newQueue();
	ngfxu::CommandQueue computeQueue = device.newQueue();
	ngfxu::Buffer buffer = device.newBuffer(ngfx::BufferDesc{ngfx::BufferUsage::VertexBuffer, 256, 0}, ngfx::StorageMode::Auto);
    ngfxu::RaytracingAccelerationStructure as = 
        device.newRaytracingAccelerationStructure(ngfx::RaytracingASDesc());
    //auto texture = device->newTexture(nullptr, ngfx::StorageMode::Auto);
	ngfxu::CommandBuffer cmdBuf = queue.obtainCommandBuffer();
	ngfxu::CommandBuffer computeCmdBuf = computeQueue.obtainCommandBuffer();
	ngfxu::ComputeEncoder computeEncoder = computeCmdBuf.newComputeEncoder();
	// render pass describes the in out resource used in this render pass
	ngfxu::RenderCommandEncoder renderCmd =	cmdBuf.newRenderEncoder(ngfx::RenderpassDesc());
	
	//renderCmd.setRenderPipelineState();
	//renderCmd.setBindGroup();
	//renderCmd.draw()
	ngfxu::ParallelRenderEncoder parallelRenderCmd = cmdBuf.newParallelRenderEncoder(ngfx::RenderpassDesc());
	
	ngfxu::RenderCommandEncoder subRenderCmd0 = parallelRenderCmd.subEncoder();
	ngfxu::RenderCommandEncoder subRenderCmd1 = parallelRenderCmd.subEncoder();
	ngfxu::RenderCommandEncoder subRenderCmd2 = parallelRenderCmd.subEncoder();
	ngfxu::RenderCommandEncoder subRenderCmd3 = parallelRenderCmd.subEncoder();

	subRenderCmd0.endEncode();
	subRenderCmd1.endEncode();
	subRenderCmd2.endEncode();
	subRenderCmd3.endEncode();

	parallelRenderCmd.endEncode();
	renderCmd.waitForFence(fence); // wait for fence
	// present drawable
	renderCmd.presentDrawable(presentDrawable);
	renderCmd.endEncode();

	computeEncoder.dispatch(64, 64, 1);
	computeEncoder.updateFence(fence); // signal the fence
	computeEncoder.endEncode();
	computeCmdBuf.commit();
	device.wait();
	// execution order 
	// renderCmd -> 
	// parallelRenderCmd -> 
	// subRenderCmd0 -> subRenderCmd1 -> subRenderCmd2 -> subRenderCmd3
	cmdBuf.commit();
	device.wait();

    return 0;
}