/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wrl.h>

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "WebUI.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//using namespace Microsoft::WRL;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR WebUI_WindowClassName = L"View";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void regsiterWebUIWindowClass(void)
{
	cx::wui::WindowClass windowClass;
	windowClass.registerWindowClass(
		WebUI_WindowClassName
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIWindow::WebUIWindow(WebUIManager* manager, std::wstring uri, HWND parentWindowHandle, bool popupWindowStyle) :
	_Manager{ manager },
	_PopupWindowStyle{ popupWindowStyle }
{
	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createWebUIWindow(
		parentWindowHandle,
		popupWindowStyle
	);
	if (!hwnd)
	{
		throw std::wstring(L"WebUIWindow::WebView(): createWindow() failed");
	}


	//-----------------------------------------------------------------------
	_View = std::make_unique<WebUIView>(this, uri);


	//-----------------------------------------------------------------------
	::ShowWindow(getWindowHandle(), SW_SHOW);
	::UpdateWindow(getWindowHandle());
}

//===========================================================================
WebUIWindow::~WebUIWindow()
{
}

//===========================================================================
HWND WebUIWindow::createWebUIWindow(HWND parentWindowHandle, bool popupWindowStyle)
{
	//-----------------------------------------------------------------------
	HWND hWnd;
	LPCWSTR lpszClassName = WebUI_WindowClassName;
	LPCWSTR lpWindowName = L"WebUIWindow";
	HWND hWndParent = parentWindowHandle;
	DWORD dwStyle;
	DWORD dwExStyle;
	RECT rect;


	//-----------------------------------------------------------------------
	if (parentWindowHandle)
	{
		GetClientRect(parentWindowHandle, &rect);
	}
	else
	{
		rect.left = 0;
		rect.top = 0;
		rect.right = 800;
		rect.bottom = 600;
	}


	//-----------------------------------------------------------------------
	if (popupWindowStyle)
	{
		dwStyle = cx::wui::FrameWindowStyle;
		dwExStyle = cx::wui::FrameWindowStyleEx;
	}
	else
	{
		dwStyle = WS_CHILD | WS_VISIBLE; // | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		dwExStyle = 0; // WS_EX_CLIENTEDGE;
	}

	//dwStyle |= WS_VSCROLL;
	//dwStyle |= WS_HSCROLL;
	//dwExStyle |= WS_EX_CLIENTEDGE;


	//-----------------------------------------------------------------------
	hWnd = createWindow(
		lpszClassName,
		hWndParent,
		lpWindowName,
		dwStyle,
		dwExStyle
	);

	return hWnd;
}

//===========================================================================
void WebUIWindow::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE    ) = &WebUIWindow::onCreate;
	_WindowMessageMap.handle(WM_DESTROY   ) = &WebUIWindow::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE     ) = &WebUIWindow::onClose;
	_WindowMessageMap.handle(WM_SIZE      ) = &WebUIWindow::onSize;
	_WindowMessageMap.handle(WM_DPICHANGED) = &WebUIWindow::onDPIChanged;
	_WindowMessageMap.handle(WM_USER+1    ) = &WebUIWindow::onUser1;
}

void WebUIWindow::onCreate(cx::wui::WindowMessage& windowMessage)
{
	//SetWindowTextW(windowMessage.hWnd, L"WebUIWindow");
}

void WebUIWindow::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	_View.reset();

	_Manager->onDestroyWindow(getWindowHandle());
}

void WebUIWindow::onClose(cx::wui::WindowMessage& windowMessage)
{
	destroyWindow();
}

void WebUIWindow::onSize(cx::wui::WindowMessage& windowMessage)
{
	if (_View)
	{
		_View->resizeWebView();
	}
}

void WebUIWindow::onDPIChanged(cx::wui::WindowMessage& windowMessage)
{
	RECT* pWindowRect = reinterpret_cast<RECT*>(windowMessage.lParam);
	cx::wui::setWindowPos(getWindowHandle(), HWND_TOP, *pWindowRect, SWP_SHOWWINDOW);
}

//===========================================================================
int WebUIWindow::getDPIAwareBound(int bound) const
{
	constexpr int DEFAULT_DPI = 96;
	return (bound * GetDpiForWindow(getWindowHandle()) / DEFAULT_DPI);
}

bool WebUIWindow::isPopupWindow(void)
{
	return _PopupWindowStyle;
}

void WebUIWindow::onUser1(cx::wui::WindowMessage& windowMessage)
{
	_View->onUser1(windowMessage);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




