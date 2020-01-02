#include "base/test_gui.h"
#include <Windows.h>

class ComputeEncoderApp : public test::App
{
private:
	ngfxu::PresentLayer		layer_;
	ngfxu::CommandQueue		queue_;
	ngfxu::ComputePipeline	compute_pipeline_;
public:
	ComputeEncoderApp(k3d::String const& appName) : test::App(appName, 1600, 1080)
	{
	}

	// for android
	virtual void InitWindow(void *param) override
	{
		k3d::App::InitWindow(param);
	}

	// for other platform
	virtual bool OnInit() override
	{
		device_ = factory().getDevice(0);
		ngfx::PresentLayerDesc desc;
		desc.format = ngfx::PixelFormat::RG11B10Float;
		desc.width = 1600;
		desc.height = 1080;
		desc.colorSpace = ngfx::ColorSpace::SRGBNonLinear;
		desc.hdrDisplay = true;
		desc.maxImages = 3;
		desc.winHandle = HostWindow()->GetHandle();
		desc.extraData = GetModuleHandle(NULL);
		layer_ = factory().newPresentLayer(desc, device_);
		device_.compileShaderLibrary();
		queue_ = device_.newQueue();
		compute_pipeline_ = device_.newComputePipeline(ngfx::ComputePipelineDesc());
		return k3d::App::OnInit();
	}

	virtual void OnProcess(k3d::Message& message) override
	{
		ngfxu::Drawable drawable = layer_.nextDrawable();
		ngfxu::Texture texture = drawable.texture();
		ngfxu::CommandBuffer cmdBuf = queue_.obtainCommandBuffer();
		ngfxu::ComputeEncoder computeEncoder = cmdBuf.newComputeEncoder();
		computeEncoder.setPipeline(compute_pipeline_);
		computeEncoder.dispatch(32, 32, 1);
		computeEncoder.endEncode();
		cmdBuf.commit();
	}
};


K3D_APP_MAIN(ComputeEncoderApp);