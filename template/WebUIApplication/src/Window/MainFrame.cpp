/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "../WebUI/WebUI.hpp"

//===========================================================================
#include "MainFrame.hpp"
#include "AboutBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





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
	_WebUIManager = std::make_shared<app::WebUIManager>(getWindowHandle());


	//-----------------------------------------------------------------------
	app::registerWebUIWindowClass();


	//-----------------------------------------------------------------------
	std::shared_ptr<app::WebUIWindow> window;
	window = _WebUIManager->newChildWindow(
		getWindowHandle(), 
		_WebUIManager->getContentsURI(L"/page_home/page.html")
	);


	//-----------------------------------------------------------------------
	_WebUIManager->getMessageService()->setWindow(window->getWindowHandle());


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
}

//===========================================================================
void MainFrame::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE)     = &MainFrame::onCreate;
	_WindowMessageMap.handle(WM_DESTROY)    = &MainFrame::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE)      = &MainFrame::onClose;
	_WindowMessageMap.handle(WM_SIZE)       = &MainFrame::onSize;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &MainFrame::onEraseBkgnd;
	_WindowMessageMap.handle(WM_KEYDOWN)    = &MainFrame::onKeyDown;
	_WindowMessageMap.handle(WM_COMMAND)    = &MainFrame::onCommand;

	_WindowMessageMap.handle(WM_USER+0) = &MainFrame::onUser0;
	_WindowMessageMap.handle(WM_USER+1) = &MainFrame::onUser1;
}

void MainFrame::onCreate(cx::wui::WindowMessage& windowMessage)
{
}

void MainFrame::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	::PostQuitMessage(0);
}

void MainFrame::onClose(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	_WebUIManager->deleteAndDestroyAllWindows();
	_WebUIManager.reset();


	//-----------------------------------------------------------------------
	destroyWindow();
}

void MainFrame::onSize(cx::wui::WindowMessage& windowMessage)
{
	RECT rect;
	GetClientRect(getWindowHandle(), &rect);
	_WebUIManager->moveWindow(getWindowHandle(), rect);
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

void MainFrame::onUser0(cx::wui::WindowMessage& windowMessage)
{
	_WebUIManager->deleteWindow((HWND)windowMessage.wParam);
}

void MainFrame::onUser1(cx::wui::WindowMessage& windowMessage)
{
}

//===========================================================================
void MainFrame::onIdle(void)
{
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



