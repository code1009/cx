/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>

//===========================================================================
#include "../res/resource.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainFrame : public wui::MessageMapWindowT<MainFrame, wui::BaseWindow>
{
protected:
	LPCWSTR _WindowClassName = L"MainFrame";

public:
	MainFrame();

public:
	void registerWindowMessageMap(void);
	void onDestroy(wui::WindowMessage& windowMessage);
	void onClose  (wui::WindowMessage& windowMessage);
	void onPaint  (wui::WindowMessage& windowMessage);
	void onCommand(wui::WindowMessage& windowMessage);
};

MainFrame::MainFrame()
{
	wui::WindowClass windowClass(_WindowClassName, IDC_MAIN, IDI_MAIN, IDI_SMALL);


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
	_WindowMessageMap.handle(WM_DESTROY) = &MainFrame::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE  ) = &MainFrame::onClose;
	_WindowMessageMap.handle(WM_PAINT  ) = &MainFrame::onPaint;
	_WindowMessageMap.handle(WM_COMMAND) = &MainFrame::onCommand;
}

void MainFrame::onDestroy(wui::WindowMessage& windowMessage)
{
	::PostQuitMessage(0);
}

void MainFrame::onClose(wui::WindowMessage& windowMessage)
{
	destroyWindow();
}

void MainFrame::onPaint(wui::WindowMessage& windowMessage)
{
	wui::PaintDC paintDC{ *this };


	std::wstring text = L"Hello, World!";
	::TextOutW(paintDC, 10, 10, text.c_str(), static_cast<int>(text.length()));
}

void MainFrame::onCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


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
	wui::getAppModule()->setInstanceHandle(hInstance);
		
	MainFrame mainFrame;

	wui::WindowMessageLoop windowMessageLoop;
	windowMessageLoop.run();

	return 0;
}
