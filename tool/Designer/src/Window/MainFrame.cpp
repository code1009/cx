/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include <cx/d2d/cx-d2d.hpp>
#include <cx/d2dw/cx-Widget.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "../WindowHandler/WindowHandler.hpp"

//===========================================================================
#include "Designer.hpp"

#include "View.hpp"
#include "CommandPanel.hpp"

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
		IDR_MAINFRAME,
		IDI_APP_ICON, IDI_APP_ICON_SMALL
	);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createWindow(MainFrame_WindowClassName, nullptr, L"Designer");
	if (nullptr == hwnd)
	{
		throw std::runtime_error("MainFrame::MainFrame(): createWindow() failed");
	}


	//-----------------------------------------------------------------------
	_View = std::make_unique<View>(*this);
	_CommandPanel = std::make_unique<CommandPanel>(*this, _View.get());


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
}

//===========================================================================
void MainFrame::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_CREATE]     = &MainFrame::onCreate;
	_WindowMessageMap[WM_DESTROY]    = &MainFrame::onDestroy;
	_WindowMessageMap[WM_CLOSE]      = &MainFrame::onClose;
	_WindowMessageMap[WM_SIZE]       = &MainFrame::onSize;
	_WindowMessageMap[WM_ERASEBKGND] = &MainFrame::onEraseBkgnd;
	_WindowMessageMap[WM_KEYDOWN]    = &MainFrame::onKeyDown;
	_WindowMessageMap[WM_COMMAND]    = &MainFrame::onCommand;
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
	destroyWindow();
}

void MainFrame::onSize(cx::wui::WindowMessage& windowMessage)
{
	updateLayout();
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
	case 0:
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
	case IDM_APP_ABOUT:
		onAppAbout(windowMessage);
		break;

	case IDM_APP_EXIT:
		destroyWindow();
		break;

	case IDM_FILE_NEW              : _View->_Designer->onFile_New             (); break;
	case IDM_FILE_OPEN             : _View->_Designer->onFile_Open            (); break;
	case IDM_FILE_SAVE             : _View->_Designer->onFile_Save            (); break;
	case IDM_FILE_SAVEAS           : _View->_Designer->onFile_SaveAs          (); break;
	case IDM_EDIT_UNDO             : _View->_Designer->onEdit_Undo            (); break;
	case IDM_EDIT_REDO             : _View->_Designer->onEdit_Redo            (); break;
	case IDM_EDIT_CUT              : _View->_Designer->onEdit_Cut             (); break;
	case IDM_EDIT_COPY             : _View->_Designer->onEdit_Copy            (); break;
	case IDM_EDIT_PASTE            : _View->_Designer->onEdit_Paste           (); break;
	case IDM_EDIT_SELECT_ALL       : _View->_Designer->onEdit_SelectAll       (); break;
	case IDM_EDIT_DESELECT_ALL     : _View->_Designer->onEdit_DeselectAll     (); break;
	case IDM_EDIT_DELETE           : _View->_Designer->onEdit_Delete          (); break;
	case IDM_DESIGN_VIEW_COMMAND   : _View->_Designer->onDesign_ViewCommand   (); break;
	case IDM_DESIGN_VIEW_PROPERTY  : _View->_Designer->onDesign_ViewProperty  (); break;
	case IDM_DESIGN_ZOOMIN         : _View->_Designer->onDesign_ZoomIn        (); break;
	case IDM_DESIGN_ZOOMOUT        : _View->_Designer->onDesign_ZoomOut       (); break;
	case IDM_DESIGN_BRING_TO_FRONT : _View->_Designer->onDesign_BringToFront  (); break;
	case IDM_DESIGN_SEND_TO_BACK   : _View->_Designer->onDesign_SendToBack    (); break;
	case IDM_DESIGN_BRING_TO_TOP   : _View->_Designer->onDesign_BringToTop    (); break;
	case IDM_DESIGN_SEND_TO_BOTTOM : _View->_Designer->onDesign_SendToBottom  (); break;
	case IDM_DESIGN_FILE_PROPERTIES: _View->_Designer->onDesign_FileProperties(); break;
	case IDM_DESIGN_SNAP_TO_GRID   : _View->_Designer->onDesign_SnapToGrid    (); break;
	case IDM_DESIGN_SHOW_GRID      : _View->_Designer->onDesign_ShowGrid      (); break;
	case IDM_DESIGN_SHOW_GRID_COORD: _View->_Designer->onDesign_ShowGridCoord (); break;
	case IDM_DESIGN_SHOW_STATUS    : _View->_Designer->onDesign_ShowStatus    (); break;

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
	case 0:
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

//===========================================================================
void MainFrame::onIdle(void)
{
	if (_View.get())
	{
//		_View->onIdle();
	}
}

void MainFrame::updateLayout(void)
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
	struct WindowLayoutPosition
	{
		std::uint32_t x{ 0 };
		std::uint32_t y{ 0 };
		std::uint32_t cx{ 0 };
		std::uint32_t cy{ 0 };
	};

	WindowLayoutPosition wlps[3];
	WindowLayoutPosition* wlp;

	
	bool hasCommandPanel = (_CommandPanel.get() != nullptr);
	bool hasPropertyPanel = (_PropertyPanel.get() != nullptr);


	//-----------------------------------------------------------------------
	if (hasCommandPanel && hasPropertyPanel)
	{
		// CommandPanel | View | PropertyPanel
		std::uint32_t viewWidth = (cx > _CommandPanel_Width + _PropertyPanel_Width)
			? cx - _CommandPanel_Width - _PropertyPanel_Width
			: 0;

		wlp = &wlps[1];
		*wlp = { 0, 0, _CommandPanel_Width, cy };

		wlp = &wlps[0];
		*wlp = { _CommandPanel_Width, 0, viewWidth, cy };

		wlp = &wlps[2];
		*wlp = { _CommandPanel_Width + viewWidth, 0, _PropertyPanel_Width, cy };
	}
	else if (hasCommandPanel)
	{
		// CommandPanel | View
		std::uint32_t viewWidth = (cx > _CommandPanel_Width)
			? cx - _CommandPanel_Width
			: 0;

		wlp = &wlps[1];
		*wlp = { 0, 0, _CommandPanel_Width, cy };

		wlp = &wlps[0];
		*wlp = { _CommandPanel_Width, 0, viewWidth, cy };
	}
	else if (hasPropertyPanel)
	{
		// View | PropertyPanel
		std::uint32_t viewWidth = (cx > _PropertyPanel_Width)
			? cx - _PropertyPanel_Width
			: 0;
		std::uint32_t propertyX = viewWidth;

		wlp = &wlps[0];
		*wlp = { 0, 0, viewWidth, cy };

		wlp = &wlps[2];
		*wlp = { propertyX, 0, _PropertyPanel_Width, cy };
	}
	else
	{
		// View only
		wlp = &wlps[0];
		*wlp = { 0, 0, cx, cy };
	}


	//-----------------------------------------------------------------------
	if (_CommandPanel.get())
	{
		wlp = &wlps[1];
		::MoveWindow(*_CommandPanel, wlp->x, wlp->y, wlp->cx, wlp->cy, TRUE);
	}
	if (_PropertyPanel.get())
	{
		wlp = &wlps[2];
		::MoveWindow(*_PropertyPanel, wlp->x, wlp->y, wlp->cx, wlp->cy, TRUE);
	}
	if (_View.get())
	{
		wlp = &wlps[0];
		::MoveWindow(*_View, wlp->x, wlp->y, wlp->cx, wlp->cy, TRUE);
	}
}



