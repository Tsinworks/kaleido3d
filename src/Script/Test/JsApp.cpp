#include "JsApp.h"
#include "JsAppBinding.h"
#include "libplatform/libplatform.h"

#if _WIN32
#include <Windows.h>
#endif

using namespace v8;
using namespace std;

namespace JavaScript
{
	struct JsHolders
	{
		JsHolders()
		{
			auto& Dir = k3d::GetEnv().GetModuleDir();
			Dir += "\\";
			V8::InitializeExternalStartupData(Dir.CStr());
			platform = platform::NewDefaultPlatform();
			V8::InitializePlatform(platform.get());
			V8::Initialize();
			allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
			Isolate::CreateParams create_params;
			create_params.array_buffer_allocator = allocator;
			isolate = Isolate::New(create_params);;
		}

		~JsHolders()
		{
			isolate->Dispose();
			delete allocator;
			V8::Dispose();
			V8::ShutdownPlatform();
		}

		bool Execute(const k3d::String& str);

		unique_ptr<Platform>	platform;
		ArrayBuffer::Allocator* allocator;
		Isolate*				isolate;
		Global<Context>			context;
	};

	Application::Application() : k3d::App("JsApplication", 1024, 768), h_(new JsHolders)
	{
	}

	Application::Application(int argc, char** argv, k3d::String const & appName)
		: k3d::App(argc, argv, appName, 1024, 768), h_(new JsHolders)
	{
	}

	Application::~Application()
	{
		delete h_;
	}

	bool Application::OnInit()
	{
		//isolate_->Enter();
		//HandleScope handleScope(isolate_);
		//global_template_ = ObjectTemplate::New(isolate);
		//global_template_->Set(
		//	String::NewFromUtf8(isolate, "GetVkCurrentDevice").ToLocalChecked(),
		//	FunctionTemplate::New(isolate, GetCurrentDeviceInfo));
		//Local<Context> context = Context::New(isolate, nullptr, global_template_);
		//context_.Reset(isolate, context);
		return k3d::App::OnInit();
	}

	void Application::ExecuteScript(k3d::String const& str)
	{
		h_->Execute(str);
		//KLOG(Info, "V8Script", "---- ExecuteScript %s", *ret);
	}

	void Application::InitializeJsEnvironment()
	{

	}

	void Application::OnProcess(k3d::Message& message)
	{
		// convert message to Json
	}

	bool JsHolders::Execute(const k3d::String& str)
	{
		Isolate::Scope isolate_scope(isolate);
		HandleScope handle_scope(isolate);
		Local<Context> localContext = Local<Context>::New(isolate, context);
		Context::Scope context_scope(localContext);
		Local<Value> result;
		TryCatch try_catch(isolate);
		Local<Script> compiled_script;
		if (!Script::Compile(localContext,
			String::NewFromUtf8(isolate, str.CStr()).ToLocalChecked())
			.ToLocal(&compiled_script)) {
			String::Utf8Value error(isolate, try_catch.Exception());
			//return result;
		}
		// Run the script!
		auto val = compiled_script->Run(localContext);
		if (!val.ToLocal(&result)) {
			String::Utf8Value error(isolate, try_catch.Exception());
			//return result;
		}
		String::Utf8Value ret(isolate, result);
		return false;
	}
}