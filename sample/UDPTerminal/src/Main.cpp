/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>
#include <runtime/LogFacility/LogFacility.hpp>

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
	cx::runtime::LogFacility_Initialize();

	CX_RUNTIME_LOG(cxLInfo)
		<< L"START" << std::endl;


	cx::wui::getAppModule()->setInstanceHandle(hInstance);

	
	getApplication()->launch();


	CX_RUNTIME_LOG(cxLInfo)
		<< L"END" << std::endl;
	
	cx::runtime::LogFacility_Cleanup();

	return 0;
}
