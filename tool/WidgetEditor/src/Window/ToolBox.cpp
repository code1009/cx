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
class ToolBox::Item
{
private:
	std::uint32_t _ID{ 0 };
	std::uint32_t _Depth{ 0 };
	std::wstring _Caption{ };
	std::wstring _Description{ };
	// todo: icon

private:
	ToolBox::coord_t _cx{ 0 };
	ToolBox::coord_t _cy{ 0 };
	ToolBox::coord_t _x{ 0 };
	ToolBox::coord_t _y{ 0 };

public:
	Item(void) = default;
	virtual ~Item(void) = default;

public:
	Item(const Item&) = delete;
	Item& operator=(const Item&) = delete;

	Item(Item&&) = delete;
	Item& operator=(Item&&) = delete;

public:
	std::uint32_t getID(void) const;
	void setID(std::uint32_t id);

	std::uint32_t getDepth(void) const;
	void setDepth(std::uint32_t depth);

	std::wstring getCaption(void) const;
	std::wstring getDescription(void) const;
	void setCaption(const std::wstring& caption);
	void setDescription(const std::wstring& description);

	ToolBox::coord_t getCX(void) const;
	ToolBox::coord_t getCY(void) const;
	ToolBox::coord_t getX(void) const;
	ToolBox::coord_t getY(void) const;
	void setCX(ToolBox::coord_t cx);
	void setCY(ToolBox::coord_t cy);
	void setX(ToolBox::coord_t x);
	void setY(ToolBox::coord_t y);

public:
	virtual ToolBox::coord_t calcHeight(void);
	virtual ToolBox::coord_t calcWidth(void);
};

//===========================================================================
std::uint32_t    ToolBox::Item::getID(void) const       { return _ID; }
void             ToolBox::Item::setID(std::uint32_t id) { _ID = id; }

std::uint32_t    ToolBox::Item::getDepth(void) const          { return _Depth; }
void             ToolBox::Item::setDepth(std::uint32_t depth) { _Depth = depth; }

std::wstring     ToolBox::Item::getCaption(void) const                          { return _Caption; }
std::wstring     ToolBox::Item::getDescription(void) const                      { return _Description; }
void             ToolBox::Item::setCaption(const std::wstring& caption)         { _Caption = caption; }
void             ToolBox::Item::setDescription(const std::wstring& description) { _Description = description; }

ToolBox::coord_t ToolBox::Item::getCX(void) const          { return _cx; }
ToolBox::coord_t ToolBox::Item::getCY(void) const          { return _cy; }
void             ToolBox::Item::setCX(ToolBox::coord_t cx) { _cx = cx; }
void             ToolBox::Item::setCY(ToolBox::coord_t cy) { _cy = cy; }

ToolBox::coord_t ToolBox::Item::getX(void) const         { return _x; }
ToolBox::coord_t ToolBox::Item::getY(void) const         { return _y; }
void             ToolBox::Item::setX(ToolBox::coord_t x) { _x = x; }
void             ToolBox::Item::setY(ToolBox::coord_t y) { _y = y; }

//===========================================================================
ToolBox::coord_t ToolBox::Item::calcHeight(void) { return _cy; }
ToolBox::coord_t ToolBox::Item::calcWidth(void)  { return _cx; }





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::GroupItem : public ToolBox::Item
{
private:
	bool _CollapseSubItems { false };
	ToolBox::ItemHandles _SubItems;

public:
	GroupItem(void) = default;
	virtual ~GroupItem(void) = default;

public:
	virtual ToolBox::coord_t calcHeight(void) override;
	virtual ToolBox::coord_t calcWidth(void) override;

public:
	ToolBox::ItemHandles& getSubItems(void);
	void CollapseSubItems(bool collapse);
};

//===========================================================================
ToolBox::coord_t ToolBox::GroupItem::calcHeight(void)
{
	ToolBox::coord_t height = getCY();


	if (!_CollapseSubItems)
	{
		for (auto& item : _SubItems)
		{
			height += item->calcHeight();
		}
	}

	return height;
}

ToolBox::coord_t ToolBox::GroupItem::calcWidth(void)
{
	ToolBox::coord_t width = getCX();


	if (!_CollapseSubItems)
	{
		for (auto& item : _SubItems)
		{
			width += item->calcWidth();
		}
	}

	return width;
}

ToolBox::ItemHandles& ToolBox::GroupItem::getSubItems(void)
{
	return _SubItems;
}

void ToolBox::GroupItem::CollapseSubItems(bool collapse)
{
	_CollapseSubItems = collapse;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::SubItem : public ToolBox::Item
{
public:
	SubItem(void) = default;
	virtual ~SubItem(void) = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ControlWindow : public cx::gw::Window
{
private:
	HWND _WindowHandle{ nullptr };
	ToolBox::ItemHandles _Items;

private:
	ID2D1SolidColorBrush* _Brush{ nullptr };

public:
	explicit ControlWindow(HWND hwnd);
	virtual ~ControlWindow(void) = default;

public:
	ControlWindow(const ControlWindow&) = delete;
	ControlWindow& operator=(const ControlWindow&) = delete;

	ControlWindow(ControlWindow&&) = delete;
	ControlWindow& operator=(ControlWindow&&) = delete;

	//-----------------------------------------------------------------------
	// cx::gw::Window
public:
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;

	//-----------------------------------------------------------------------
public:
	ToolBox::ItemHandles& getItems(void);

	//-----------------------------------------------------------------------
public:
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemDrawing
{
public:
	IDWriteTextFormat* _pCaptionTextFormat{ nullptr };
	ID2D1SolidColorBrush* _pCaptionTextBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrameFillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrameLineBrush{ nullptr };

public:
	ItemDrawing(void) = default;
	virtual ~ItemDrawing(void) = default;

public:
	virtual bool createDeviceResources(ToolBox::ControlWindow* window);
	virtual void destroyDeviceResources(void);

public:
	virtual void draw(ToolBox::ControlWindow* window, ToolBox::Item* item);
	virtual void drawFrame(ToolBox::ControlWindow* window, ToolBox::Item* item);
	virtual void drawCaption(ToolBox::ControlWindow* window, ToolBox::Item* item);
};

bool ToolBox::ItemDrawing::createDeviceResources(ToolBox::ControlWindow* window)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	if (!_pFrameFillBrush)
	{
		hr = window->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.75f, 0.75f, 0.75f, 0.25f),
			&_pFrameFillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFrameLineBrush)
	{
		hr = window->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f),
			&_pFrameLineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pCaptionTextBrush)
	{
		hr = window->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f),
			&_pCaptionTextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pCaptionTextFormat)
	{
		hr = window->getDWriteFactory()->CreateTextFormat(
			//L"Arial",
			L"돋움",
			//L"FixedSys",
			nullptr,
			DWRITE_FONT_WEIGHT_ULTRA_BLACK, // DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,
			//L"en-us",
			L"ko-kr",
			&_pCaptionTextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		//_pCaptionTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // hcenter
		//_pCaptionTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter
	}

	return true;
}

void ToolBox::ItemDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFrameFillBrush)
	{
		_pFrameFillBrush->Release();
		_pFrameFillBrush = nullptr;
	}
	if (_pFrameLineBrush)
	{
		_pFrameLineBrush->Release();
		_pFrameLineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pCaptionTextBrush)
	{
		_pCaptionTextBrush->Release();
		_pCaptionTextBrush = nullptr;
	}
	if (_pCaptionTextFormat)
	{
		_pCaptionTextFormat->Release();
		_pCaptionTextFormat = nullptr;
	}
}

void ToolBox::ItemDrawing::draw(ToolBox::ControlWindow* window, ToolBox::Item* item)
{
	drawFrame(window, item);
	drawCaption(window, item);
}

void ToolBox::ItemDrawing::drawFrame(ToolBox::ControlWindow* window, ToolBox::Item* item)
{
	D2D1_RECT_F rect;


	rect.left = item->getX();
	rect.right = item->getX() + item->getCX();
	rect.top = item->getY();
	rect.bottom = item->getY() + item->getCY();

	window->getDRenderTarget()->FillRectangle(&rect, _pFrameFillBrush);
	window->getDRenderTarget()->DrawRectangle(&rect, _pFrameLineBrush);
}

void ToolBox::ItemDrawing::drawCaption(ToolBox::ControlWindow* window, ToolBox::Item* item)
{
	D2D1_RECT_F rect;


	rect.left = item->getX();
	rect.right = item->getX() + item->getCX();
	rect.top = item->getY();
	rect.bottom = item->getY() + item->getCY();


	window->getDRenderTarget()->DrawTextW(
		item->getCaption().c_str(),
		static_cast<UINT32>(item->getCaption().length()),
		_pCaptionTextFormat,
		rect,
		_pCaptionTextBrush
	);
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::GroupItemDrawing : public ToolBox::ItemDrawing
{
public:
	GroupItemDrawing(void) = default;
	virtual ~GroupItemDrawing(void) = default;

public:
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::SubItemDrawing : public ToolBox::ItemDrawing
{
public:
	SubItemDrawing(void) = default;
	virtual ~SubItemDrawing(void) = default;
};




/////////////////////////////////////////////////////////////////////////////
//
// class ToolBox::ControlWindow
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ControlWindow::ControlWindow(HWND hwnd) :
	cx::gw::Window(hwnd, false),
	_WindowHandle(hwnd)
{
	getViewport()->setWindowSize(0, 0);
	getViewport()->setDocumentSize(0, 0);
	getViewport()->enableScrollbar(true);
}

//===========================================================================
bool ToolBox::ControlWindow::createDeviceResources(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	rv = cx::gw::Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;


	if (!_Brush)
	{
		hr = getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 1.0f, 0.5f),
			&_Brush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::ControlWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_Brush)
	{
		_Brush->Release();
		_Brush = nullptr;
	}


	//-----------------------------------------------------------------------
	cx::gw::Window::destroyDeviceResources();
}

void ToolBox::ControlWindow::draw(void)
{
	//-----------------------------------------------------------------------
	cx::gw::Window::draw();


	//-----------------------------------------------------------------------
	D2D1_ROUNDED_RECT rrect;


	rrect.rect.left = 100.0f;
	rrect.rect.top = 100.0f;
	rrect.rect.right = 200.0f;
	rrect.rect.bottom = 200.0f;
	rrect.radiusX = 10.0f;
	rrect.radiusY = 10.0f;

	getDRenderTarget()->FillRoundedRectangle(&rrect, _Brush);
}

ToolBox::ItemHandles& ToolBox::ControlWindow::getItems(void)
{
	return _Items;
}





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
	_ControlWindow = std::make_unique<ControlWindow>(*this);


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
	_ParentWindowHandle = parentWindowHandle;


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
	}
}

void ToolBox::onHScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };


	_ControlWindow->getViewport()->handleHScrollbar(wm.nSBCode());
}

void ToolBox::onVScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_VSCROLL_WindowMessageCrack wm{ windowMessage };


	_ControlWindow->getViewport()->handleVScrollbar(wm.nSBCode());
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




