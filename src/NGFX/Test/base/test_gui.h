#pragma once

#include "CoreMinimal.h"
#include "ngfx_shell.h"

namespace test
{
	class App : public k3d::App
	{
	public:
		App(k3d::String const& appName, k3d::U32 width = 2560, k3d::U32 height = 1660, const char* rhiDll = "Kaleido3D.NGFX.dll");
		virtual ~App() override;

		virtual void OnProcess(k3d::Message& message) override;

	protected:
		ngfxu::Factory& factory();

	private:
		k3d::os::LibraryLoader			loader_;
		k3d::SharedPtr<ngfxu::Factory>	factory_;
	};
}