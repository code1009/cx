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
#include "ToolBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR ToolBox_WindowClassName = L"ToolBox";
constexpr UINT    ToolBox_WindowID        = 0;

//===========================================================================
constexpr UINT ToolBox_Timer_ID   = 100;
constexpr UINT ToolBox_Timer_Time = 100;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ToolBox(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	wui::WindowClass windowClass;


	windowClass.registerWindowClass(
		ToolBox_WindowClassName,
		ToolBox_WindowID,
		IDI_MAIN, IDI_SMALL
	);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createToolBox(parentWindowHandle);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("ToolBox::ToolBox(): createToolBox() failed");
	}


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
}

//===========================================================================
HWND ToolBox::createToolBox(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	_parentWindowHandle = parentWindowHandle;


	//-----------------------------------------------------------------------
	int window_x;
	int window_y;
	int window_client_cx;
	int window_client_cy;


	window_x = 0;
	window_y = 0;
	window_client_cx = 160;
	window_client_cy = 400;


	//-----------------------------------------------------------------------
	RECT ParentRect;


	::GetWindowRect(parentWindowHandle, &ParentRect);



	//-----------------------------------------------------------------------
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpszClassName = ToolBox_WindowClassName;
	LPCWSTR   lpWindowName  = L"도구상자";
	DWORD     dwStyle       = wui::ToolWindowStyle;
	DWORD     dwExStyle     = wui::ToolWindowStyleEx;
	int       X             = 0;
	int       Y             = 0;
	int       nWidth        = 0;
	int       nHeight       = 0;
	HMENU     hMenu         = nullptr;
	HINSTANCE hInstance     = nullptr;
	LPVOID    lpParam       = this;


	dwStyle = 0
		| WS_OVERLAPPED
		| WS_CAPTION
		| WS_SIZEBOX
		| WS_SYSMENU
		| WS_CLIPCHILDREN
		| WS_CLIPSIBLINGS
		;

	dwExStyle = 0
		//| WS_EX_TOOLWINDOW
		//| WS_EX_WINDOWEDGE
		//| WS_EX_APPWINDOW
		;

	//-----------------------------------------------------------------------
	int cxEdge      = GetSystemMetrics(SM_CXEDGE);
	int cyEdge      = GetSystemMetrics(SM_CYEDGE);

	int cxSizeFrame = GetSystemMetrics(SM_CXSIZEFRAME);
	int cySizeFrame = GetSystemMetrics(SM_CYSIZEFRAME);

	int cyCaption   = GetSystemMetrics(SM_CYCAPTION);
	int cyMenu      = GetSystemMetrics(SM_CYMENU);

	int cxSpace     = 20;
	int cySpace     = 20;


	//-----------------------------------------------------------------------
	RECT rect = { 0, 0, window_client_cx, window_client_cy };


	// Adjust the window size to include the non-client area
	if (AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle))
	{
		X       = ParentRect.left + cxSpace + cxEdge + cxSizeFrame;
		Y       = ParentRect.top  + cySpace + cyEdge + cxSizeFrame + cyCaption + cyMenu;
		nWidth  = rect.right  - rect.left;
		nHeight = rect.bottom - rect.top;
	}
	else
	{
		X       = ParentRect.left + cxSpace + cxEdge + cxSizeFrame;
		Y       = ParentRect.top  + cySpace + cyEdge + cxSizeFrame + cyCaption + cyMenu;
		nWidth  = window_client_cx + 10;
		nHeight = window_client_cy + 10;
	}


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createWindow(
		lpszClassName,
		hWndParent,
		lpWindowName,
		dwStyle,
		dwExStyle,
		X, Y, nWidth, nHeight,
		hMenu,
		hInstance
	);

	return hwnd;
}

//===========================================================================
void ToolBox::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE    ) = &ToolBox::onCreate;
	_WindowMessageMap.handle(WM_CLOSE     ) = &ToolBox::onClose;
	_WindowMessageMap.handle(WM_SIZE      ) = &ToolBox::onSize;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &ToolBox::onEraseBkgnd;
	_WindowMessageMap.handle(WM_KEYDOWN   ) = &ToolBox::onKeyDown;
	_WindowMessageMap.handle(WM_COMMAND   ) = &ToolBox::onCommand;
}

void ToolBox::onCreate(wui::WindowMessage& windowMessage)
{
	defaultWindowProc(windowMessage);
}

void ToolBox::onClose(wui::WindowMessage& windowMessage)
{
	UINT id;
	id = ::GetDlgCtrlID(*this);

	HWND parent;
	//parent = GetParent(*this);
	parent = _parentWindowHandle;

	::PostMessageW(
		parent,
		WM_USER+0,
		static_cast<WPARAM>(id),
		reinterpret_cast<LPARAM>(this)
	);
}

void ToolBox::onSize(wui::WindowMessage& windowMessage)
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
}

void ToolBox::onEraseBkgnd(wui::WindowMessage& windowMessage)
{
	wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void ToolBox::onKeyDown(wui::WindowMessage& windowMessage)
{
	wui::WM_KEYDOWN_WindowMessageCrack wm{ windowMessage };


	switch (wm.nChar())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void ToolBox::onCommand(wui::WindowMessage& windowMessage)
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

void ToolBox::onMenuCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void ToolBox::onCtlCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

//===========================================================================
void ToolBox::onIdle(void)
{
}

//===========================================================================




