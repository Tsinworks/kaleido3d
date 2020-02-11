#pragma once
#include "CoreMinimal.h"
#include "v8.h"

namespace JavaScript
{
	struct JsHolders;

	class Application : public k3d::App
	{
	public:
		Application();
		Application(int argc, char** argv, k3d::String const & appName);
		virtual			~Application();

		virtual bool	OnInit();

		// App::OnProcess
		virtual void	OnProcess(k3d::Message& message) override;

		// Load essential JsBindins
		virtual void	InitializeJsEnvironment();

		virtual void	ExecuteScript(k3d::String const& str);

	private:
		JsHolders*		h_;
	};
}