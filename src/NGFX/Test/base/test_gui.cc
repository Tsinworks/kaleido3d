#include "test_gui.h"

namespace test
{

	static void log_print(int level, const char* msg)
	{

	}

	typedef ngfx::Factory* (*fnCreate)(bool debug, decltype(log_print) call);

	App::App(k3d::String const& appName, k3d::U32 width, k3d::U32 height, const char* rhiDll)
		: k3d::App(appName, width, height)
		, loader_(rhiDll)
	{
		using ngfxu::Factory;
		using namespace k3d;
		fnCreate create = (fnCreate)loader_.ResolveSymbol("CreateFactory");
		factory_ = MakeSharedMacro(Factory, create(true, log_print));
	}

	App::~App() {
		factory_.Reset();
	}

	void App::OnProcess(k3d::Message & message)
	{
	}

	ngfxu::Factory& App::factory()
	{
		return *factory_;
	}
}