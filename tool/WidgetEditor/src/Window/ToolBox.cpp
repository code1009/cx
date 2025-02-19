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
#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_WindowMessageHandler.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"





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
	cx::wui::WindowClass windowClass;


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
	_ControlWindow = std::make_unique<ControlWindow>(this);


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
}

//===========================================================================
LRESULT ToolBox::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	auto result = cx::wui::MessageMapWindowT<ToolBox, cx::wui::BaseWindow>::onWindowMessage(windowMessage);

	if (_ControlWindow)
	{
		_ControlWindow->getWindowMessageHandler()->onWindowMessage(
			windowMessage.hWnd,
			windowMessage.uMsg,
			windowMessage.wParam,
			windowMessage.lParam
		);
	}

	return result;
}


//===========================================================================
HWND ToolBox::createToolBox(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	_ParentWindowHandle = parentWindowHandle;


	//-----------------------------------------------------------------------
	int window_x;
	int window_y;
	int window_client_cx;
	int window_client_cy;


	window_x = 0;
	window_y = 0;
	window_client_cx = 300;
	window_client_cy = 400;


	//-----------------------------------------------------------------------
	RECT ParentRect;


	::GetWindowRect(parentWindowHandle, &ParentRect);



	//-----------------------------------------------------------------------
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpszClassName = ToolBox_WindowClassName;
	LPCWSTR   lpWindowName  = L"도구상자";
	DWORD     dwStyle       = cx::wui::ToolWindowStyle;
	DWORD     dwExStyle     = cx::wui::ToolWindowStyleEx;
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
		| WS_VSCROLL
		;

	dwExStyle = 0
		//| WS_EX_TOOLWINDOW
		//| WS_EX_WINDOWEDGE
		| WS_EX_CLIENTEDGE
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
	_WindowMessageMap.handle(WM_HSCROLL   ) = &ToolBox::onHScroll;
	_WindowMessageMap.handle(WM_VSCROLL   ) = &ToolBox::onVScroll;
	_WindowMessageMap.handle(WM_MOUSEWHEEL) = &ToolBox::onMouseWheel;
	_WindowMessageMap.handle(WM_KEYDOWN   ) = &ToolBox::onKeyDown;
	_WindowMessageMap.handle(WM_COMMAND   ) = &ToolBox::onCommand;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &ToolBox::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT     ) = &ToolBox::onPaint;
}

void ToolBox::onCreate(cx::wui::WindowMessage& windowMessage)
{
	defaultWindowProc(windowMessage);
}

void ToolBox::onClose(cx::wui::WindowMessage& windowMessage)
{
	UINT id;
	id = ::GetDlgCtrlID(*this);

	HWND parent;
	//parent = GetParent(*this);
	parent = _ParentWindowHandle;

	::PostMessageW(
		parent,
		WM_USER+0,
		static_cast<WPARAM>(id),
		reinterpret_cast<LPARAM>(this)
	);
}

void ToolBox::onSize(cx::wui::WindowMessage& windowMessage)
{
	RECT rect;
	GetClientRect(*this, &rect);

	UINT cx = static_cast<UINT>(rect.right - rect.left);
	UINT cy = static_cast<UINT>(rect.bottom - rect.top);

	if (_ControlWindow.get())
	{
		_ControlWindow->getViewport()->setWindowSize(cx, cy);
		_ControlWindow->getItemView()->recalcLayout();
	}
}

void ToolBox::onHScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };


	_ControlWindow->getViewport()->handleHScrollbar(wm.nSBCode());
	if (_ControlWindow.get())
	{
		_ControlWindow->getItemView()->recalcLayout();
	}
}

void ToolBox::onVScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_VSCROLL_WindowMessageCrack wm{ windowMessage };


	_ControlWindow->getViewport()->handleVScrollbar(wm.nSBCode());
	if (_ControlWindow.get())
	{
		_ControlWindow->getItemView()->recalcLayout();
	}
}

void ToolBox::onMouseWheel(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_MOUSEWHEEL_WindowMessageCrack wm{ windowMessage };

	UINT fwKeys = GET_KEYSTATE_WPARAM(windowMessage.wParam);
	bool scale = false;


	switch (fwKeys)
	{
	case MK_LBUTTON:
		break;

	case MK_RBUTTON:
		break;

	case MK_MBUTTON:
		break;

	case MK_XBUTTON1:
		break;

	case MK_XBUTTON2:
		break;

	case MK_CONTROL:
		scale = true;
		break;

	case MK_SHIFT:
		break;

	default:
		break;
	}


	if (scale)
	{
		if (wm.zDelta() > 0)
		{
			//_ControlWindow->getViewport()->zoom(true);
		}
		else
		{

			//_ControlWindow->getViewport()->zoom(false);
		}
	}
	else
	{
		if (wm.zDelta() > 0)
		{
			_ControlWindow->getViewport()->handleVScrollbar(SB_LINEUP);
		}
		else
		{
			_ControlWindow->getViewport()->handleVScrollbar(SB_LINEDOWN);
		}
	}

	if (_ControlWindow.get())
	{
		_ControlWindow->getItemView()->recalcLayout();
	}
}

void ToolBox::onKeyDown(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_KEYDOWN_WindowMessageCrack wm{ windowMessage };


	switch (wm.nChar())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void ToolBox::onCommand(cx::wui::WindowMessage& windowMessage)
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

void ToolBox::onMenuCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void ToolBox::onCtlCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void ToolBox::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void ToolBox::onPaint(cx::wui::WindowMessage& windowMessage)
{
	if(_ControlWindow)
	{
		_ControlWindow->render();
	}

	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

//===========================================================================
void ToolBox::onIdle(void)
{
}

//===========================================================================
void ToolBox::addIconBitmap(std::wstring name, std::wstring filePath)
{
	auto bitmap = cx::gw::makeBitmap(filePath);
	_ControlWindow->getItemView()->getBitmapList()->addBitmap(name, bitmap);
}

void ToolBox::addIconBitmap(std::wstring name, const void* pImageData, std::size_t imageSize)
{
	auto bitmap = cx::gw::makeBitmap(pImageData, imageSize);
	_ControlWindow->getItemView()->getBitmapList()->addBitmap(name, bitmap);
}

//===========================================================================
ToolBox::GroupItemSharedPtr ToolBox::addGroupItem(
	std::size_t id,
	ToolBox::GroupItemSharedPtr parentItem,
	std::wstring caption,
	std::wstring icon,
	ToolBox::ItemStyle style,
	cx::gw::coord_t size,
	std::wstring description
)
{
	ToolBox::GroupItemSharedPtr groupItem;

	groupItem = makeGroupItem(
		id, 
		caption,
		icon,
		style,
		size,
		description
	);
	_ControlWindow->getItemView()->addItem(parentItem, groupItem);

	_ControlWindow->getItemView()->recalcLayout();

	return groupItem;
}

ToolBox::SubItemSharedPtr ToolBox::addSubItem(
	std::size_t id,
	ToolBox::GroupItemSharedPtr parentItem,
	std::wstring caption,
	std::wstring icon,
	ToolBox::ItemStyle style,
	cx::gw::coord_t size,
	std::wstring description
)
{
	ToolBox::SubItemSharedPtr subItem;

	subItem = makeSubItem(
		id,
		caption,
		icon,
		style,
		size,
		description
	);
	_ControlWindow->getItemView()->addItem(parentItem, subItem);

	_ControlWindow->getItemView()->recalcLayout();

	return subItem;
}

//===========================================================================
void ToolBox::releaseWindowMouseCapture(void)
{
	_ControlWindow->getWindowMessageHandler()->releaseWindowMouseCapture();
	
	//todo
	//_ControlWindow->getWindowMessageHandler()->_
	//_ControlWindow->getWindowMessageHandler()->onMouseLButtonUp()
}




