/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
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
	CX_RUNTIME_LOG(LInfo)
		<< L"=============================================================================" << std::endl
		<< L"START" << std::endl
		<< L"=============================================================================" << std::endl;


	cx::wui::getAppModule()->setInstanceHandle(hInstance);

	
	getApplication()->launch();


	CX_RUNTIME_LOG(LInfo)
		<< L"=============================================================================" << std::endl
		<< L"END" << std::endl
		<< L"=============================================================================" << std::endl;

	return 0;
}
