/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>


//===========================================================================
#include "Application.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	cx::runtime::memory_debug _memory_debug;
	_memory_debug.enable();
	_memory_debug.start_leak_check();


	cx::wui::getAppModule()->setInstanceHandle(hInstance);
	getApplication()->launch();
	

	//_memory_debug.test_leak_check();
	_memory_debug.end_leak_check();
	return 0;
}
