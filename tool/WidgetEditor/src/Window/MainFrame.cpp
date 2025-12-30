/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/gw/gw.hpp>
#include <cx/runtime/runtime.hpp>

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
	createToolBox();
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
	if (_View.get())
	{
		_View->onKeyDown(windowMessage);
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

	
	_View->onMenuCommand(windowMessage);
	if (1 == windowMessage.lResult)
	{
		windowMessage.lResult = 0;
		return;
	}
	
	switch (wm.nID())
	{
	case IDM_HELP_ABOUT:
		onAppAbout(windowMessage);
		break;

	case IDM_FILE_EXIT:
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

//===========================================================================
static unsigned char _bitmap_item_png[712] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x01,
	0x83, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6F,
	0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x28, 0x91, 0x7D, 0x91, 0x3D, 0x48,
	0xC3, 0x40, 0x1C, 0xC5, 0x5F, 0x53, 0x4B, 0x45, 0x2A, 0x22, 0x56, 0x10,
	0x71, 0xC8, 0x50, 0x9D, 0xEC, 0xA2, 0x22, 0x8E, 0xA5, 0x8A, 0x45, 0xB0,
	0x50, 0xDA, 0x0A, 0xAD, 0x3A, 0x98, 0x5C, 0xFA, 0x05, 0x4D, 0x1A, 0x92,
	0x14, 0x17, 0x47, 0xC1, 0xB5, 0xE0, 0xE0, 0xC7, 0x62, 0xD5, 0xC1, 0xC5,
	0x59, 0x57, 0x07, 0x57, 0x41, 0x10, 0xFC, 0x00, 0x71, 0x17, 0x9C, 0x14,
	0x5D, 0xA4, 0xC4, 0xFF, 0x25, 0x85, 0x16, 0x31, 0x1E, 0x1C, 0xF7, 0xE3,
	0xDD, 0xBD, 0xC7, 0xDD, 0x3B, 0x40, 0x68, 0x56, 0x99, 0x6A, 0xF6, 0xC4,
	0x00, 0x55, 0xB3, 0x8C, 0x74, 0x22, 0x2E, 0xE6, 0xF2, 0xAB, 0x62, 0xF0,
	0x15, 0x02, 0x06, 0x11, 0xC0, 0x30, 0xFC, 0x12, 0x33, 0xF5, 0x64, 0x66,
	0x31, 0x0B, 0xCF, 0xF1, 0x75, 0x0F, 0x1F, 0x5F, 0xEF, 0xA2, 0x3C, 0xCB,
	0xFB, 0xDC, 0x9F, 0xA3, 0x5F, 0x29, 0x98, 0x0C, 0xF0, 0x89, 0xC4, 0x31,
	0xA6, 0x1B, 0x16, 0xF1, 0x06, 0xF1, 0xEC, 0xA6, 0xA5, 0x73, 0xDE, 0x27,
	0x0E, 0xB3, 0xB2, 0xA4, 0x10, 0x9F, 0x13, 0x4F, 0x1A, 0x74, 0x41, 0xE2,
	0x47, 0xAE, 0xCB, 0x2E, 0xBF, 0x71, 0x2E, 0x39, 0x2C, 0xF0, 0xCC, 0xB0,
	0x91, 0x4D, 0xCF, 0x13, 0x87, 0x89, 0xC5, 0x52, 0x17, 0xCB, 0x5D, 0xCC,
	0xCA, 0x86, 0x4A, 0x3C, 0x43, 0x1C, 0x51, 0x54, 0x8D, 0xF2, 0x85, 0x9C,
	0xCB, 0x0A, 0xE7, 0x2D, 0xCE, 0x6A, 0xB5, 0xCE, 0xDA, 0xF7, 0xE4, 0x2F,
	0x0C, 0x15, 0xB4, 0x95, 0x0C, 0xD7, 0x69, 0x8E, 0x21, 0x81, 0x25, 0x24,
	0x91, 0x82, 0x08, 0x19, 0x75, 0x54, 0x50, 0x85, 0x85, 0x28, 0xAD, 0x1A,
	0x29, 0x26, 0xD2, 0xB4, 0x1F, 0xF7, 0xF0, 0x8F, 0x3A, 0xFE, 0x14, 0xB9,
	0x64, 0x72, 0x55, 0xC0, 0xC8, 0xB1, 0x80, 0x1A, 0x54, 0x48, 0x8E, 0x1F,
	0xFC, 0x0F, 0x7E, 0x77, 0x6B, 0x16, 0xA7, 0xA7, 0xDC, 0xA4, 0x50, 0x1C,
	0x08, 0xBC, 0xD8, 0xF6, 0xC7, 0x38, 0x10, 0xDC, 0x05, 0x5A, 0x0D, 0xDB,
	0xFE, 0x3E, 0xB6, 0xED, 0xD6, 0x09, 0xE0, 0x7F, 0x06, 0xAE, 0xB4, 0x8E,
	0xBF, 0xD6, 0x04, 0xE6, 0x3E, 0x49, 0x6F, 0x74, 0xB4, 0xC8, 0x11, 0x30,
	0xB0, 0x0D, 0x5C, 0x5C, 0x77, 0x34, 0x79, 0x0F, 0xB8, 0xDC, 0x01, 0x46,
	0x9E, 0x74, 0xC9, 0x90, 0x1C, 0xC9, 0x4F, 0x53, 0x28, 0x16, 0x81, 0xF7,
	0x33, 0xFA, 0xA6, 0x3C, 0x30, 0x74, 0x0B, 0xF4, 0xAD, 0xB9, 0xBD, 0xB5,
	0xF7, 0x71, 0xFA, 0x00, 0x64, 0xA9, 0xAB, 0xE5, 0x1B, 0xE0, 0xE0, 0x10,
	0x98, 0x28, 0x51, 0xF6, 0xBA, 0xC7, 0xBB, 0x7B, 0xBB, 0x7B, 0xFB, 0xF7,
	0x4C, 0xBB, 0xBF, 0x1F, 0x38, 0x67, 0x72, 0x8F, 0xEB, 0x43, 0xBA, 0x5D,
	0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xF8, 0x00, 0xFF,
	0x00, 0x35, 0x69, 0x2C, 0x50, 0x2D, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48,
	0x59, 0x73, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x0E, 0xC0, 0x01, 0x6A,
	0xD6, 0x89, 0x09, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45, 0x07,
	0xE9, 0x02, 0x11, 0x05, 0x1B, 0x34, 0xE1, 0xB3, 0x7E, 0xB4, 0x00, 0x00,
	0x00, 0xC6, 0x49, 0x44, 0x41, 0x54, 0x38, 0xCB, 0xB5, 0xD3, 0xB1, 0x4A,
	0x03, 0x51, 0x10, 0x05, 0xD0, 0xB3, 0xC1, 0x22, 0x7F, 0x12, 0x44, 0x2C,
	0x82, 0xA6, 0x49, 0xE1, 0x47, 0xA4, 0x14, 0x82, 0x5A, 0x8A, 0xFE, 0x90,
	0xBD, 0x9D, 0x95, 0x58, 0xA7, 0x11, 0x91, 0x2C, 0x84, 0x80, 0x55, 0x52,
	0x04, 0xBF, 0xC2, 0x22, 0x26, 0x2C, 0xB1, 0x19, 0xE5, 0xF1, 0x10, 0x61,
	0x37, 0xEB, 0xC0, 0x14, 0x77, 0x86, 0x79, 0x73, 0xE7, 0xCE, 0xBC, 0x02,
	0x53, 0x9C, 0x68, 0x66, 0xF3, 0x02, 0x5B, 0x6C, 0xB0, 0xA8, 0x59, 0x7C,
	0x88, 0xAE, 0x78, 0x60, 0xD6, 0xA0, 0xFB, 0x0C, 0xD5, 0x41, 0x16, 0x3C,
	0xC6, 0x19, 0x26, 0x58, 0x62, 0x88, 0x7E, 0x92, 0xAF, 0x70, 0x8F, 0x8F,
	0xB4, 0x28, 0x65, 0x70, 0x83, 0x1D, 0x2E, 0x02, 0xBF, 0x05, 0x4E, 0xFD,
	0xEA, 0x2F, 0x06, 0x13, 0x5C, 0xE2, 0x25, 0xF0, 0x39, 0x4E, 0x93, 0xFC,
	0x1A, 0x8F, 0xF9, 0x2C, 0x7B, 0x69, 0xD0, 0xC9, 0x82, 0x63, 0xAC, 0x30,
	0x0A, 0x7C, 0x17, 0xF8, 0xDB, 0x17, 0xA1, 0xCB, 0x8F, 0x75, 0x6A, 0x76,
	0x2D, 0x42, 0x87, 0xF6, 0x46, 0xC8, 0x45, 0xEC, 0x61, 0x80, 0x57, 0xBC,
	0xE3, 0x28, 0x5B, 0xE3, 0x27, 0x9E, 0x42, 0xCC, 0xFF, 0x59, 0xE3, 0x33,
	0x6E, 0x51, 0x06, 0xBE, 0xFE, 0xE5, 0x90, 0x1E, 0x5A, 0xD5, 0xA0, 0x95,
	0xCF, 0x54, 0x06, 0xB5, 0x26, 0x5E, 0x7E, 0x01, 0xA4, 0x2C, 0x53, 0xAB,
	0xA7, 0xD7, 0xB9, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44,
	0xAE, 0x42, 0x60, 0x82
};

void MainFrame::createToolBox(void)
{
	//-----------------------------------------------------------------------
	_ToolBox = std::make_unique<ToolBox>(*this);


	//-----------------------------------------------------------------------
	_ToolBox->addIconBitmap(L"item.png", _bitmap_item_png, sizeof(_bitmap_item_png));


	//-----------------------------------------------------------------------
	ToolBox::GroupItemSharedPtr root;
	ToolBox::GroupItemSharedPtr parent;
	ToolBox::SubItemSharedPtr   item;
	std::size_t id=0;

	//ToolBox::ItemStyle itemStyle = ToolBox::ItemStyle::Flat;
	//std::wstring       itemIcon  = L"item.png";
	ToolBox::ItemStyle itemStyle = ToolBox::ItemStyle::Button;
	std::wstring       itemIcon;

	//-----------------------------------------------------------------------
	parent = _ToolBox->addGroupItem(++id                   , nullptr, L"도형", itemIcon, itemStyle);
	item   = _ToolBox->addSubItem  (++id                   , parent , L"글자");
	item   = _ToolBox->addSubItem  (++id                   , parent , L"선");
	item   = _ToolBox->addSubItem  (++id                   , parent , L"네모");
	item   = _ToolBox->addSubItem  (++id                   , parent , L"원");
	//-----------------------------------------------------------------------
	root   = _ToolBox->addGroupItem(++id                   , nullptr, L"명령", itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	parent = root;
	//-----------------------------------------------------------------------
	//parent = _ToolBox->addGroupItem(++id                   , root   , L"편집");
	//item   = _ToolBox->addSubItem  (IDM_EDIT_DELETE        , parent , L"삭제", itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	//parent = _ToolBox->addGroupItem(++id                   , root   , L"실행");
	//item   = _ToolBox->addSubItem  (IDM_EDIT_UNDO          , parent , L"실행 취소", itemIcon, itemStyle);
	//item   = _ToolBox->addSubItem  (IDM_EDIT_REDO          , parent , L"다시 실행", itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	//parent = _ToolBox->addGroupItem(++id                   , root   , L"복사 및 붙여넣기");
	//item   = _ToolBox->addSubItem  (IDM_EDIT_CUT           , parent , L"잘라내기" , itemIcon, itemStyle);
	//item   = _ToolBox->addSubItem  (IDM_EDIT_COPY          , parent , L"복사"     , itemIcon, itemStyle);
	//item   = _ToolBox->addSubItem  (IDM_EDIT_PASTE         , parent , L"붙여넣기" , itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	//parent = _ToolBox->addGroupItem(++id                   , root   , L"선택");
	item   = _ToolBox->addSubItem  (IDM_EDIT_SELECT_ALL    , parent , L"모두 선택"     , itemIcon, itemStyle);
	item   = _ToolBox->addSubItem  (IDM_EDIT_DESELECT_ALL  , parent , L"모두 선택 해제", itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	//parent = _ToolBox->addGroupItem(++id                   , root   , L"순서");
	//item   = _ToolBox->addSubItem  (++id                   , parent , L"앞으로 가져오기"   , itemIcon, itemStyle);
	//item   = _ToolBox->addSubItem  (++id                   , parent , L"뒤로 보내기"       , itemIcon, itemStyle);
	item   = _ToolBox->addSubItem  (IDM_EDIT_BRING_TO_TOP  , parent , L"맨 앞으로 가져오기", itemIcon, itemStyle);
	item   = _ToolBox->addSubItem  (IDM_EDIT_SEND_TO_BOTTOM, parent , L"맨 뒤로 보내기  "  , itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	//parent = _ToolBox->addGroupItem(++id                   , root   , L"맞춤");
	item   = _ToolBox->addSubItem  (IDM_EDIT_SNAP_TO_GRID  , parent , L"격자에 맞춤", itemIcon, itemStyle);
	//-----------------------------------------------------------------------
	//item   = _ToolBox->addSubItem  (IDM_HELP_ABOUT         , nullptr, L"정보", L"item.png");


	//-----------------------------------------------------------------------
	_ToolBox->setGroupItemMouseClickedHandler  (std::bind(&MainFrame::onToolBoxGroupItemMouseClicked  , this, std::placeholders::_1));
	_ToolBox->setGroupItemMouseClickedHandler  (std::bind(&MainFrame::onToolBoxGroupItemMouseDbClicked, this, std::placeholders::_1));
	_ToolBox->setGroupItemMouseDbClickedHandler(std::bind(&MainFrame::onToolBoxGroupItemMouseDbClicked, this, std::placeholders::_1));
	_ToolBox->setGroupItemMouseDraggingHandler (std::bind(&MainFrame::onToolBoxGroupItemMouseDragging , this, std::placeholders::_1));
	_ToolBox->setSubItemMouseClickedHandler    (std::bind(&MainFrame::onToolBoxSubItemMouseClicked    , this, std::placeholders::_1));
	_ToolBox->setSubItemMouseDbClickedHandler  (std::bind(&MainFrame::onToolBoxSubItemMouseDbClicked  , this, std::placeholders::_1));
	_ToolBox->setSubItemMouseDraggingHandler   (std::bind(&MainFrame::onToolBoxSubItemMouseDragging   , this, std::placeholders::_1));


	//-----------------------------------------------------------------------
	_ToolBox->redraw();
}

void MainFrame::onToolBoxGroupItemMouseClicked(ToolBox::EventParam* param)
{
}

void MainFrame::onToolBoxGroupItemMouseDbClicked(ToolBox::EventParam* param)
{
}

void MainFrame::onToolBoxGroupItemMouseDragging(ToolBox::EventParam* param)
{
}

void MainFrame::onToolBoxSubItemMouseClicked(ToolBox::EventParam* param)
{
	_View->executeMenuCommand(static_cast<std::uint32_t>(param->_Id));
}

void MainFrame::onToolBoxSubItemMouseDbClicked(ToolBox::EventParam* param)
{
}

void MainFrame::onToolBoxSubItemMouseDragging(ToolBox::EventParam* param)
{
	switch (param->_Id)
	{
	case 2:
	case 3:
	case 4:
	case 5:
		_ToolBox->releaseMouseCapture();
		param->_DragDrop = true;
		doToolBoxDragDrop(param->_Id - 2);
		break;

	default:
		break;
	}
}

void MainFrame::doToolBoxDragDrop(std::size_t id)
{
	switch (id)
	{
	case 0: _View->doDragDrop(L"Shape.Text.Design"); break;
	case 1: _View->doDragDrop(L"Shape.Line.Design"); break;
	case 2: _View->doDragDrop(L"Shape.Rectangle.Design"); break;
	case 3: _View->doDragDrop(L"Shape.Ellipse.Design"); break;

	default:
		break;
	}
}
