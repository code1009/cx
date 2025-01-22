/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "View.hpp"
#include "MainFrame.hpp"
#include "AboutBox.hpp"
#include "MyBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR MainFrame_WindowClassName = L"MainFrame";

//===========================================================================
constexpr UINT MainFrame_Timer_ID = 100;
constexpr UINT MainFrame_Timer_Time = 100;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MainFrame::MainFrame()
{
	//-----------------------------------------------------------------------
	wui::WindowClass windowClass(MainFrame_WindowClassName, IDC_MAIN,IDI_MAIN, IDI_SMALL);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createWindow(MainFrame_WindowClassName);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("MainFrame::MainFrame(): createWindow() failed");
	}


	//-----------------------------------------------------------------------
	_View = std::make_unique<View>(*this);


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);
}

//===========================================================================
void MainFrame::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE)     = &MainFrame::onCreate;
	_WindowMessageMap.handle(WM_DESTROY)    = &MainFrame::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE)      = &MainFrame::onClose;
	_WindowMessageMap.handle(WM_SIZE)       = &MainFrame::onSize;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &MainFrame::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT)      = &MainFrame::onPaint;
	_WindowMessageMap.handle(WM_COMMAND)    = &MainFrame::onCommand;
}

void MainFrame::onCreate(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MainFrame::onCreate()\r\n");

	//::MessageBoxW(*this, wm.lpCreateStruct()->lpszClass, L"MainFrame::onCreate()", MB_OK);
}

void MainFrame::onDestroy(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MainFrame::onDestroy()\r\n");

	::PostQuitMessage(0);
}

void MainFrame::onClose(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MainFrame::onClose()\r\n");

	destroyWindow();
}

void MainFrame::onSize(wui::WindowMessage& windowMessage)
{
	RECT rect;
	::GetClientRect(*this, &rect);

	UINT cx = static_cast<UINT>(rect.right - rect.left);
	UINT cy = static_cast<UINT>(rect.bottom - rect.top); 

	::MoveWindow(*_View, 0, 0, cx, cy, TRUE);
}

void MainFrame::onEraseBkgnd(wui::WindowMessage& windowMessage)
{
	wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(FALSE);
}

void MainFrame::onPaint(wui::WindowMessage& windowMessage)
{
	wui::PaintDC paintDC{ *this };
	

	::TextOutW(paintDC, 10, 10, L"Hello, Windows!", 15);
}

void MainFrame::onCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	if (wm.wndCtl() == nullptr)
	{
		onMenuCommand(windowMessage);
	}
	else
	{
		onCtlCommand(windowMessage);
	}
}

void MainFrame::onMenuCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDM_ABOUT:
		onAppAbout(windowMessage);
		break;

	case IDM_EXIT:
		destroyWindow();
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void MainFrame::onAppAbout(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MainFrame::onAppAbout()\r\n");

#if 0
	AboutBox aboutBox;
	aboutBox.doModal(*this);
#else
	MyBox myBox;
	myBox.doModal(*this);
#endif
}

void MainFrame::onCtlCommand(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MainFrame::onCtlCommand()\r\n");

	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDC_MAIN:
		::MessageBoxW(*this, L"wuiApplication", L"MainFrame::onCtlCommand()", MB_OK);
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

//===========================================================================
void MainFrame::onIdle(void)
{
	_View->onIdle();
}




