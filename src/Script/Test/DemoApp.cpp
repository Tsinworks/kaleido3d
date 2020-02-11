#include "JsApp.h"

#if _WIN32
#include <Windows.h>
#endif

class JsApp : public JavaScript::Application
{
public:
	JsApp(k3d::String const& Name) : JavaScript::Application() {}
	JsApp(int argc, char** argv, k3d::String const & appName) : JavaScript::Application(argc, argv, appName)
	{

	}
};

K3D_APP_MAIN_ARG(JsApp);