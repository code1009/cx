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
	std::wstring _Caption{ };
	std::wstring _Description{ };
	// todo: icon

private:
	std::size_t _cx{ 0 };
	std::size_t _cy{ 0 };

private:
	std::uint32_t _Depth{ 0 };

private:
	std::size_t _x{ 0 };
	std::size_t _y{ 0 };

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

	std::size_t getCX(void) const;
	std::size_t getCY(void) const;
	std::size_t getX(void) const;
	std::size_t getY(void) const;
	void setCX(std::size_t cx);
	void setCY(std::size_t cy);
	void setX(std::size_t x);
	void setY(std::size_t y);

public:
	virtual std::size_t calcHeight(void);
	virtual std::size_t calcWidth(void);
};

//===========================================================================
std::uint32_t ToolBox::Item::getID(void) const { return _ID; }
void ToolBox::Item::setID(std::uint32_t id) { _ID = id; }

std::uint32_t ToolBox::Item::getDepth(void) const { return _Depth; }
void ToolBox::Item::setDepth(std::uint32_t depth) { _Depth = depth; }

std::wstring ToolBox::Item::getCaption(void) const { return _Caption; }
std::wstring ToolBox::Item::getDescription(void) const { return _Description; }
void ToolBox::Item::setCaption(const std::wstring& caption) { _Caption = caption; }
void ToolBox::Item::setDescription(const std::wstring& description) { _Description = description; }

std::size_t ToolBox::Item::getCX(void) const { return _cx; }
std::size_t ToolBox::Item::getCY(void) const { return _cy; }
void ToolBox::Item::setCX(std::size_t cx) { _cx = cx; }
void ToolBox::Item::setCY(std::size_t cy) { _cy = cy; }

std::size_t ToolBox::Item::getX(void) const { return _x; }
std::size_t ToolBox::Item::getY(void) const { return _y; }
void ToolBox::Item::setX(std::size_t x) { _x = x; }
void ToolBox::Item::setY(std::size_t y) { _y = y; }

//===========================================================================
std::size_t ToolBox::Item::calcHeight(void) { return _cy; }
std::size_t ToolBox::Item::calcWidth(void) { return _cx; }





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::GroupItem : public ToolBox::Item
{
private:
	bool _CollapseSubItems { false };
	ItemHandles _SubItems;

public:
	GroupItem(void) = default;
	virtual ~GroupItem(void) = default;

public:
	virtual std::size_t calcHeight(void) override;
	virtual std::size_t calcWidth(void) override;

public:
	ItemHandles& getSubItems(void);
	void CollapseSubItems(bool collapse);
};

//===========================================================================
std::size_t ToolBox::GroupItem::calcHeight(void)
{
	std::size_t height = getCY();


	if (!_CollapseSubItems)
	{
		for (auto& item : _SubItems)
		{
			height += item->calcHeight();
		}
	}

	return height;
}

std::size_t ToolBox::GroupItem::calcWidth(void)
{
	std::size_t width = getCX();


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
class ToolBox::ListWindow
{
private:
	ItemHandles _Items;

public:
	ListWindow(void) = default;
	virtual ~ListWindow(void) = default;

public:
	ListWindow(const ListWindow&) = delete;
	ListWindow& operator=(const ListWindow&) = delete;

	ListWindow(ListWindow&&) = delete;
	ListWindow& operator=(ListWindow&&) = delete;

public:
	ItemHandles& getItems(void);
};

//===========================================================================
ToolBox::ItemHandles& ToolBox::ListWindow::getItems(void)
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


	//-----------------------------------------------------------------------
	_ListWindow = std::make_unique<ListWindow>();
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




