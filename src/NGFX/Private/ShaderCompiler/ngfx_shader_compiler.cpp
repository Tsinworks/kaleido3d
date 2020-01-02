#include "ngfx_shader_compiler.h"
#include "CoreMinimal.h"

using namespace k3d;

SharedPtr<os::LibraryLoader> gShCLibrary;

#ifndef USE_DXC_AS_DEFAULT_SHADER_COMPILER
#define USE_DXC_AS_DEFAULT_SHADER_COMPILER 1
#define SHADER_COMPILER_NAME "dxcompiler.dll"
//"libdxcompiler.so";
#endif

#if USE_DXC_AS_DEFAULT_SHADER_COMPILER
#include <dxc/Support/Global.h>
#include <dxc/Support/Unicode.h>
#include <dxc/Support/WinAdapter.h>
#include <dxc/Support/WinIncludes.h>
#include <dxc/dxcapi.h>
static DxcCreateInstanceProc sCreateInstanceProc = NULL;
#endif

namespace ngfx {

	void createShaderCompiler(ngfx::IShaderCompiler **ppCompiler, Result* result)
	{
		CComPtr<IDxcCompiler> compiler;
		CComPtr<IDxcLibrary> library;
		CComPtr<IDxcUtils> utils;
		//IDxcUtils
		//sCreateInstanceProc(CLSID_DxcLibrary, __uuidof(IDxcLibrary), reinterpret_cast<void**>(&m_library));
		sCreateInstanceProc(CLSID_DxcCompiler, __uuidof(IDxcCompiler), reinterpret_cast<void**>(&compiler));

		//CLSID_DxcCompiler;
		//CLSID_DxcLibrary;
	}

	void destroyShaderCompiler(ngfx::IShaderCompiler *pCompiler)
	{

	}

}

ngfx::Result ngfxCreateShaderCompiler(ngfx::IShaderCompiler **ppCompiler)
{
	return ngfx::Result::Ok;
}

ngfx::Result ngfxDestroyShaderCompiler(ngfx::IShaderCompiler *pCompiler)
{
	return ngfx::Result::Ok;
}

#if _WIN32
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	K3D_UNUSED(instance);
	BOOL result = TRUE;
	if (reason == DLL_PROCESS_DETACH)
	{
		if (reserved == 0)
		{
			gShCLibrary = MakeShared<os::LibraryLoader>(SHADER_COMPILER_NAME);
			sCreateInstanceProc = reinterpret_cast<DxcCreateInstanceProc>(gShCLibrary->ResolveSymbol("DxcCreateInstance"));
		}
		else
		{
			gShCLibrary.Reset();
		}
	}
	return result;
}
#endif