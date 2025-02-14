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
#include "ToolBox.hpp"
#include "MainFrame.hpp"
#include "AboutBox.hpp"





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
	cx::wui::WindowClass windowClass;


	windowClass.registerWindowClass(
		MainFrame_WindowClassName,
		IDC_MAIN,
		IDI_MAIN, IDI_SMALL
	);


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


	//-----------------------------------------------------------------------
	::SetCursor(::LoadCursor(nullptr, IDC_ARROW));


	//-----------------------------------------------------------------------
	_ToolBox = std::make_unique<ToolBox>(*this);
}

//===========================================================================
void MainFrame::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE    ) = &MainFrame::onCreate;
	_WindowMessageMap.handle(WM_DESTROY   ) = &MainFrame::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE     ) = &MainFrame::onClose;
	_WindowMessageMap.handle(WM_SIZE      ) = &MainFrame::onSize;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &MainFrame::onEraseBkgnd;
	_WindowMessageMap.handle(WM_KEYDOWN   ) = &MainFrame::onKeyDown;
	_WindowMessageMap.handle(WM_COMMAND   ) = &MainFrame::onCommand;
	_WindowMessageMap.handle(WM_NOTIFY    ) = &MainFrame::onNotify;
	_WindowMessageMap.handle(WM_USER      ) = &MainFrame::onUser0;
}

void MainFrame::onCreate(cx::wui::WindowMessage& windowMessage)
{
}

void MainFrame::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	if (_ToolBox)
	{
		//_ToolBox->destroyWindow();
		_ToolBox.reset();
	}

	::PostQuitMessage(0);
}

void MainFrame::onClose(cx::wui::WindowMessage& windowMessage)
{
	destroyWindow();
}

void MainFrame::onSize(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	RECT rect;


	::GetClientRect(*this, &rect);


	//-----------------------------------------------------------------------
	UINT cx;
	UINT cy;


	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	//-----------------------------------------------------------------------
	if (_View.get())
	{
		::MoveWindow(*_View, 0, 0, cx, cy, TRUE);
	}
}

void MainFrame::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void MainFrame::onKeyDown(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_KEYDOWN_WindowMessageCrack wm{ windowMessage };


	switch (wm.nChar())
	{
	case VK_F7:
	case VK_F8:
		if (_View.get())
		{
			_View->onKeyDown(windowMessage);
		}
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void MainFrame::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	if (wm.wndCtl() == nullptr)
	{
		onMenuCommand(windowMessage);
	}
	else
	{
		onCtlCommand(windowMessage);
	}
}

void MainFrame::onMenuCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


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

void MainFrame::onAppAbout(cx::wui::WindowMessage& windowMessage)
{
	AboutBox aboutBox;


	aboutBox.doModal(*this);
}

void MainFrame::onCtlCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDC_MAIN:
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void MainFrame::onNotify(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_NOTIFY_WindowMessageCrack wm{ windowMessage };
}

void MainFrame::onUser0(cx::wui::WindowMessage& windowMessage)
{
	auto child = reinterpret_cast<cx::wui::BaseWindow*>(windowMessage.lParam);

	
	if (child)
	{
		if (_ToolBox.get() == child)
		{
			_ToolBox->destroyWindow();
			_ToolBox.reset();
		}
	}
}

//===========================================================================
void MainFrame::onIdle(void)
{
	_View->onIdle();
}




