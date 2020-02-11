#include "V8Binding.h"
#include "../NGFX/Public/ngfx.h"

#define V8_FUNC_BEGIN(Name) void V8##Name(const FunctionCallbackInfo<v8::Value>& args) {
#define V8_FUNC_END }
#define V8_FUNC_EXTDECL(Name) Local<External> pRet##Name = External::New(args.GetIsolate(), p##Name);
#define V8_FUNC_ARG_RET //args.GetReturnValue().Set

#define V8_INTERFACE_DEFINE_GLOBAL(ClassName) Global<ObjectTemplate> g##ClassName##Templ
#define V8_INTERFACE_BEGIN(ClassName) if (g##ClassName##Templ.IsEmpty()) { Local<ObjectTemplate> _ClassName##Templ = ObjectTemplate::New(args.GetIsolate());
#define V8_INTERFACE_FIELD_COUNT(Count) _ClassName##Templ->SetInternalFieldCount(Count);
#define V8_INTERFACE_METHOD(Name, ClassName) _ClassName##Templ->Set(String::NewFromUtf8(args.GetIsolate(), #Name).ToLocalChecked(), FunctionTemplate::New(args.GetIsolate(), V8##Name))
#define V8_INTERFACE_END(ClassName) g##ClassName##Templ.Reset(args.GetIsolate(),  _ClassName##Templ); }

using namespace k3d;
using namespace ngfx;

namespace v8
{
	V8_INTERFACE_DEFINE_GLOBAL(App);
	V8_INTERFACE_DEFINE_GLOBAL(Factory);
	V8_INTERFACE_DEFINE_GLOBAL(Device);
	V8_INTERFACE_DEFINE_GLOBAL(CommandQueue);

	V8_FUNC_BEGIN(GetDevice)

	V8_FUNC_END

	V8_FUNC_BEGIN(GetDeviceCount)

	V8_FUNC_END

	V8_FUNC_BEGIN(CreateFactory)
		V8_INTERFACE_BEGIN(Factory);
		V8_INTERFACE_FIELD_COUNT(1)
		V8_INTERFACE_METHOD(GetDevice, Factory);
		V8_INTERFACE_METHOD(GetDeviceCount, Factory);
		V8_INTERFACE_END(Factory)

		//V8_FUNC_EXTDECL(factory)
		V8_FUNC_ARG_RET
	V8_FUNC_END

#if 0
    static void vk_commandQueue_commandBuffer(const FunctionCallbackInfo<v8::Value>& args)
    {

    }

    static void vk_device_getDesc(const FunctionCallbackInfo<v8::Value>& args)
    {
        Local<External> device = Local<External>::Cast(args.Holder()->GetInternalField(0));
        Device* ptr = (Device*)device->Value();
        //DeviceDesc desc = {};
	
        //ptr->GetDesc(&desc);
        //args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(), desc.vendorName));
    }

    static void vk_device_createPipelineLayout(const FunctionCallbackInfo<v8::Value>& args)
    {
    }

    static void vk_device_createCommandQueue(const FunctionCallbackInfo<v8::Value>& args)
    {
        Local<External> device = Local<External>::Cast(args.Holder()->GetInternalField(0));
        Device* ptr = (Device*)device->Value();
        CommandQueue* pCmdQueue = nullptr;
        //ptr->CreateCommandQueue(CommandQueueType::Copy, &pCmdQueue);

        if (gCommandQueueTempl.IsEmpty())
        {
            Local<ObjectTemplate> cmdQueueTempl = ObjectTemplate::New(args.GetIsolate());
            cmdQueueTempl->SetInternalFieldCount(1);
            cmdQueueTempl->Set(String::NewFromUtf8(args.GetIsolate(), "commandBuffer").ToLocalChecked(),
                FunctionTemplate::New(args.GetIsolate(), vk_commandQueue_commandBuffer));
            gCommandQueueTempl.Reset(args.GetIsolate(), cmdQueueTempl);
        }
        Local<ObjectTemplate> cmdQueueTempl = Local<ObjectTemplate>::New(args.GetIsolate(), gCommandQueueTempl);
        Local<Object> cmdQueue = cmdQueueTempl->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
        Local<External> cmdQueuePtr = External::New(args.GetIsolate(), pCmdQueue);
        cmdQueue->SetInternalField(0, cmdQueuePtr);
        args.GetReturnValue().Set(cmdQueue);
    }
    static void vk_factory_createSwapchain(const FunctionCallbackInfo<v8::Value>& args)
    {
        Local<External> factory = Local<External>::Cast(args.Holder()->GetInternalField(0));
        Factory* ptr = (Factory*)factory->Value();

		PresentLayerDesc swapchainDesc = {};
        auto desc = args[0]->ToObject();
        swapchainDesc.width = desc->Get(String::NewFromUtf8(args.GetIsolate(), "width"))->Uint32Value();
        swapchainDesc.height = desc->Get(String::NewFromUtf8(args.GetIsolate(), "height"))->Uint32Value();
        swapchainDesc.pixelFormat = (PixelFormat)desc->Get(String::NewFromUtf8(args.GetIsolate(), "format"))->Uint32Value();

        Local<External> pCmdQueue = Local<External>::Cast(args[1]->ToObject()->GetInternalField(0));
        CommandQueue* pQueue = (CommandQueue*)pCmdQueue->Value();

        Local<External> app = Local<External>::Cast(args[2]->ToObject()->GetInternalField(0));
        //App* pApp = (App*)app->Value();
        //void* hWnd = pApp->Handle();

        SwapChain * pSwapChain = nullptr;
        //ptr->CreateSwapchain(&swapchainDesc, pQueue, hWnd, &pSwapChain);
    }

    static void vk_factory_enumDevices(const FunctionCallbackInfo<v8::Value>& args)
    {
        Local<External> factory = Local<External>::Cast(args.Holder()->GetInternalField(0));
        Factory* ptr = (Factory*)factory->Value();
        uint32_t count = 0;
        ptr->EnumDevice(&count, nullptr);
        Device ** ppDevice = new Device*[count];
        ptr->EnumDevice(&count, ppDevice);
        auto deviceArray = Array::New(args.GetIsolate(), count);
        if (gDeviceTempl.IsEmpty())
        {
            Local<ObjectTemplate> deviceTempl = ObjectTemplate::New(args.GetIsolate());
            deviceTempl->SetInternalFieldCount(1);
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "getDesc"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createCommandQueue"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_createCommandQueue));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createPipelineLayout"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_createPipelineLayout));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createRenderPass"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createRenderTarget"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createSampler"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createBuffer"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "createTexture"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            deviceTempl->Set(String::NewFromUtf8(args.GetIsolate(), "waitIdle"),
                FunctionTemplate::New(args.GetIsolate(), vk_device_getDesc));
            gDeviceTempl.Reset(args.GetIsolate(), deviceTempl);
        }
        Local<ObjectTemplate> deviceTempl = Local<ObjectTemplate>::New(args.GetIsolate(), gDeviceTempl);
        for (uint32_t i = 0; i < count; i++)
        {
            Local<Object> device =
                deviceTempl->NewInstance(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
            Local<External> devicePtr = External::New(args.GetIsolate(), ppDevice[i]);
            device->SetInternalField(0, devicePtr);
            deviceArray->Set(i, device);
        }
        args.GetReturnValue().Set(deviceArray);
    }

    static void vk_createFactory(const FunctionCallbackInfo<v8::Value>& args)
    {
        if (gFactoryTempl.IsEmpty())
        {
            //gFactoryTempl.Reset(args.GetIsolate(), factoryTempl);
        }
        Local<ObjectTemplate> factoryTempl = Local<ObjectTemplate>::New(args.GetIsolate(), gFactoryTempl);
        Local<Object> result = factoryTempl->NewInstance(args.GetIsolate()->GetCurrentContext())
            .ToLocalChecked();
        Factory* factory = nullptr;
        CreateFactory(&factory, false);
        Local<External> factory_ptr = External::New(args.GetIsolate(), factory);
        result->SetInternalField(0, factory_ptr);
        args.GetReturnValue().Set(result);
    }
#endif

	ScriptEngine::ScriptEngine()
	{
		initializeWithExtData();
		initialize();
		KLOG(Info,"V8Script", "---------------- loaded....");
	}

	ScriptEngine::~ScriptEngine()
	{
		destroy();
	}

	Local<Value> ScriptEngine::ExecuteScript(const char * scripts)
	{
		Isolate::Scope isolate_scope(getIsoloate());
		HandleScope handle_scope(getIsoloate());
		Local<Context> context = Local<Context>::New(getIsoloate(), context_);
		Context::Scope context_scope(context);
		Local<Value> result;
		TryCatch try_catch(getIsoloate());
		Local<Script> compiled_script;
		if (!Script::Compile(context, 
			String::NewFromUtf8(getIsoloate(), scripts).ToLocalChecked())
			.ToLocal(&compiled_script)) {
			String::Utf8Value error(getIsoloate(), try_catch.Exception());
			return result;
		}
		// Run the script!
		auto val = compiled_script->Run(context);
		if (!val.ToLocal(&result)) {
			String::Utf8Value error(getIsoloate(), try_catch.Exception());
			return result;
		}
		String::Utf8Value ret(getIsoloate(), result);
		KLOG(Info, "V8Script", "---- ExecuteScript %s", *ret);
		return result;
	}

	void ScriptEngine::initializeWithExtData() {
		//IAsset * natives_blob = AssetManager::Open("asset://Test/natives_blob.bin");
		//uint64 natives_blobs_length = natives_blob->GetLength();
		//StartupData ndata = { (const char*)natives_blob->GetBuffer(), (int)natives_blobs_length };
		//V8::SetNativesDataBlob(&ndata);

		//IAsset * snapshot_blob = AssetManager::Open("asset://Test/snapshot_blob.bin");
		//uint64 snapshot_blob_length = snapshot_blob->GetLength();
		//StartupData sdata = { (const char*)snapshot_blob->GetBuffer(), (int)snapshot_blob_length };
		//V8::SetSnapshotDataBlob(&sdata);

		auto platform = platform::NewDefaultPlatform();
		V8::InitializePlatform(platform.get());
		V8::Initialize();
	}


	static void GetCurrentDeviceInfo(const FunctionCallbackInfo<Value>& args) 
	{
		HandleScope scope(args.GetIsolate());
		args.GetReturnValue().Set(String::NewFromUtf8(args.GetIsolate(),"do you know ?", NewStringType::kNormal).ToLocalChecked());
	}

	void ScriptEngine::initialize() {
		Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
		auto isolate = Isolate::New(create_params);
		isolate_ = isolate;
		isolate_->Enter();
		HandleScope handleScope(isolate);
		global_template_ = ObjectTemplate::New(isolate);
		global_template_->Set(
			String::NewFromUtf8(isolate, "GetVkCurrentDevice").ToLocalChecked(),
			FunctionTemplate::New(isolate, GetCurrentDeviceInfo));
		Local<Context> context = Context::New(isolate, nullptr, global_template_);
		context_.Reset(isolate, context);
	}

	void ScriptEngine::destroy() {
		isolate_->Exit();
		isolate_->Dispose();
		V8::Dispose();
		V8::ShutdownPlatform();
	}

	V8Module::V8Module() : m_Inst(nullptr)
	{

	}

	V8Module::~V8Module()
	{
		
	}

	void V8Module::Start()
	{
		if (m_Inst == nullptr)
		{
			m_Inst = new ScriptEngine;
		}
	}

	void V8Module::Shutdown()
	{
		if (m_Inst) 
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}

	void V8Module::ExecuteScript(const char * script)
	{
		auto ret = m_Inst->ExecuteScript(script);
	}

	const char * V8Module::Name()
	{
		return "V8Script";
	}
}

#if _WIN32

#endif

//MODULE_IMPLEMENT(V8Script, v8::V8Module)
