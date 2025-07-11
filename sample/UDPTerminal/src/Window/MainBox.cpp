/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>
#include <common/fs_std_wstring.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "../WebUI/WebUI.hpp"
#include "../Model/Model.hpp"

//===========================================================================
#include "MainBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr UINT MainBox_Timer_ID = 10;
constexpr UINT MainBox_Timer_Time = 100;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MainBox::MainBox()
{
	setTemplateName(IDD_MAIN);
	registerWindowMessageMap();
}

//===========================================================================
MainBox::~MainBox()
{
}

//===========================================================================
void MainBox::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_INITDIALOG) = &MainBox::onInitDialog;
	_WindowMessageMap.handle(WM_CLOSE)      = &MainBox::onClose;
	_WindowMessageMap.handle(WM_SIZE)       = &MainBox::onSize;
	_WindowMessageMap.handle(WM_TIMER)      = &MainBox::onTimer;
	_WindowMessageMap.handle(WM_COMMAND)    = &MainBox::onCommand;

	_WindowMessageMap.handle(WM_USER+0) = &MainBox::onUser0;
	_WindowMessageMap.handle(WM_USER+1) = &MainBox::onUser1;
}

void MainBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	//HICON hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_MAIN));
	//HICON hSmallIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_SMALL));
	//SendMessage(*this, WM_SETICON, ICON_SMALL, (LPARAM)hSmallIcon);
	//SendMessage(*this, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	setIcon(cx::wui::loadIcon(IDI_MAIN), TRUE);
	setIcon(cx::wui::loadIcon(IDI_SMALL), FALSE);


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
	RECT rect;
	GetClientRect(getWindowHandle(), &rect);
	_WebUIManager->moveWindow(getWindowHandle(), rect);


	//-----------------------------------------------------------------------
	getModel()->setWebUIManager(_WebUIManager);
	_TimerID = SetTimer(*this, MainBox_Timer_ID, MainBox_Timer_Time, nullptr);


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void MainBox::onClose(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	getModel()->disconnect();
	getModel()->setWebUIManager(nullptr);
	KillTimer(*this, _TimerID);


	//-----------------------------------------------------------------------
	_WebUIManager->deleteAndDestroyAllWindows();
	_WebUIManager.reset();


	//-----------------------------------------------------------------------
	::EndDialog(*this, IDCANCEL);
	windowMessage.setResult(TRUE);
}

void MainBox::onSize(cx::wui::WindowMessage& windowMessage)
{
	RECT rect;
	GetClientRect(getWindowHandle(), &rect);
	_WebUIManager->moveWindow(getWindowHandle(), rect);
	windowMessage.setResult(TRUE);
}

void MainBox::onTimer(cx::wui::WindowMessage& windowMessage)
{
	//CX_RUNTIME_LOG(cxLInfo) << L"MainBox::onTimer()";
	_WebUIManager->getMessageService()->processWebMessageQueue();

	windowMessage.setResult(TRUE);
}

void MainBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDOK:
		::EndDialog(*this, IDOK);
		windowMessage.setResult(TRUE);
		break;

	case IDCANCEL:
		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
		break;

	default:
		windowMessage.setResult(FALSE);
		break;
	}
}

void MainBox::onUser0(cx::wui::WindowMessage& windowMessage)
{
	_WebUIManager->deleteWindow((HWND)windowMessage.wParam);
	windowMessage.setResult(TRUE);
}

void MainBox::onUser1(cx::wui::WindowMessage& windowMessage)
{
	windowMessage.setResult(TRUE);
}
