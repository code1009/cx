/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>

//===========================================================================
#include "../res/resource.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_DEBUG)
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Debug/cx_static_library.lib")
#else
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Release/cx_static_library.lib")
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainFrame : public cx::wui::MessageMapWindowT<MainFrame, cx::wui::BaseWindow>
{
protected:
	LPCWSTR _WindowClassName = L"MainFrame";

public:
	MainFrame();

public:
	void registerWindowMessageMap(void);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose  (cx::wui::WindowMessage& windowMessage);
	void onPaint  (cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
};

MainFrame::MainFrame()
{
	cx::wui::WindowClass windowClass(_WindowClassName, IDC_MAIN, IDI_MAIN, IDI_SMALL);


	registerWindowMessageMap();

	HWND hwnd = createWindow(_WindowClassName);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("MainFrame::MainFrame(): createWindow() failed");
	}
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);
}

void MainFrame::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_DESTROY] = &MainFrame::onDestroy;
	_WindowMessageMap[WM_CLOSE  ] = &MainFrame::onClose;
	_WindowMessageMap[WM_PAINT  ] = &MainFrame::onPaint;
	_WindowMessageMap[WM_COMMAND] = &MainFrame::onCommand;
}

void MainFrame::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	::PostQuitMessage(0);
}

void MainFrame::onClose(cx::wui::WindowMessage& windowMessage)
{
	destroyWindow();
}

void MainFrame::onPaint(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::PaintDC paintDC{ *this };


	std::wstring text = L"Hello, World!";
	::TextOutW(paintDC, 10, 10, text.c_str(), static_cast<int>(text.length()));
}

void MainFrame::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDM_ABOUT: 
		::MessageBoxW(*this, L"IDM_ABOUT", L"MainFrame::onCommand()", MB_OK); 
		break;

	case IDM_EXIT:
		destroyWindow(); 
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	cx::wui::getAppModule()->setInstanceHandle(hInstance);
		
	MainFrame mainFrame;

	cx::wui::WindowMessageLoop windowMessageLoop;
	windowMessageLoop.run();

	return 0;
}
