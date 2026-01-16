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
#include "UILayout.hpp"
#include "UIController.hpp"

#include "Catalog.hpp"
#include "Designer.hpp"

#include "View.hpp"
#include "CommandPanel.hpp"
#include "PropertyPanel.hpp"

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
	_CommandPanel = std::make_unique<CommandPanel>(*this, _View->_Designer.get());
	_PropertyPanel = std::make_unique<PropertyPanel>(*this, _View->_Designer.get());


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	::SetCursor(::LoadCursor(nullptr, IDC_ARROW));
}

//===========================================================================
void MainFrame::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_CREATE    ] = &MainFrame::onCreate;
	_WindowMessageMap[WM_DESTROY   ] = &MainFrame::onDestroy;
	_WindowMessageMap[WM_CLOSE     ] = &MainFrame::onClose;
	_WindowMessageMap[WM_SIZE      ] = &MainFrame::onSize;
	_WindowMessageMap[WM_ERASEBKGND] = &MainFrame::onEraseBkgnd;
	_WindowMessageMap[WM_KEYDOWN   ] = &MainFrame::onKeyDown;
	_WindowMessageMap[WM_COMMAND   ] = &MainFrame::onCommand;
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
	if (_CommandPanel)
	{
		_CommandPanel->destroyWindow();
	}
	_CommandPanel.reset();


	if (_PropertyPanel)
	{
		_PropertyPanel->destroyWindow();
	}
	_PropertyPanel.reset();


	if (_View)
	{
		_View->destroyWindow();
	}
	_View.reset();



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
	updateLayout(cx, cy);
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
	//case IDM_DESIGN_VIEW_COMMAND   : _View->_Designer->onDesign_ViewCommand   (); break;
	//case IDM_DESIGN_VIEW_PROPERTY  : _View->_Designer->onDesign_ViewProperty  (); break;
	case IDM_DESIGN_ZOOMIN         : _View->_Designer->onDesign_ZoomIn        (); break;
	case IDM_DESIGN_ZOOMOUT        : _View->_Designer->onDesign_ZoomOut       (); break;
	case IDM_DESIGN_BRING_TO_FRONT : _View->_Designer->onDesign_BringToFront  (); break;
	case IDM_DESIGN_SEND_TO_BACK   : _View->_Designer->onDesign_SendToBack    (); break;
	case IDM_DESIGN_BRING_TO_TOP   : _View->_Designer->onDesign_BringToTop    (); break;
	case IDM_DESIGN_SEND_TO_BOTTOM : _View->_Designer->onDesign_SendToBottom  (); break;
	case IDM_DESIGN_FILE_PROPERTIES: _View->_Designer->onDesign_FileProperties(); break;

	case IDM_DESIGN_SNAP_TO_GRID   :
		toggleMenuItemChecked(wm.nID(), _MenuCheck.SNAP_TO_GRID);
		_View->_Designer->onDesign_SnapToGrid    (); 
		break;

	case IDM_DESIGN_SHOW_GRID      : 
		toggleMenuItemChecked(wm.nID(), _MenuCheck.SHOW_GRID);
		_View->_Designer->onDesign_ShowGrid      ();
		break;

	case IDM_DESIGN_SHOW_GRID_COORD: 
		toggleMenuItemChecked(wm.nID(), _MenuCheck.SHOW_GRID_COORD);
		_View->_Designer->onDesign_ShowGridCoord ();
		break;

	case IDM_DESIGN_SHOW_STATUS    : 
		toggleMenuItemChecked(wm.nID(), _MenuCheck.SHOW_STATUS);
		_View->_Designer->onDesign_ShowStatus    ();
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
	case 0:
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

//===========================================================================
void MainFrame::toggleMenuItemChecked(UINT itemID, bool& checkFlag)
{
	if (checkFlag)
	{
		checkFlag = false;
		::CheckMenuItem(
			::GetMenu(*this),
			itemID,
			checkFlag ? MF_CHECKED : MF_UNCHECKED
		);
	}
	else
	{
		checkFlag = true;
		::CheckMenuItem(
			::GetMenu(*this),
			itemID,
			checkFlag ? MF_CHECKED : MF_UNCHECKED
		);
	}
}

void MainFrame::onIdle(void)
{
	if (_View.get())
	{
//		_View->onIdle();
	}
}

void MainFrame::updateLayout(std::uint32_t cx, std::uint32_t cy)
{
	UILayoutManager layoutManager;
	bool firstCol = true;

	auto layoutChangedHandler = [this](UILayout* layout)
		{
			HWND hwnd = reinterpret_cast<HWND>(layout->_Item._Data);
			if (hwnd != nullptr)
			{
				::MoveWindow(
					hwnd,
					static_cast<LONG>(layout->_Item._L),
					static_cast<LONG>(layout->_Item._T),
					static_cast<LONG>(layout->_Item._R - layout->_Item._L),
					static_cast<LONG>(layout->_Item._B - layout->_Item._T),
					TRUE
				);
			}
		};

	if (_CommandPanel)
	{
		UILayoutStyle uiLayoutStyle{ 200.0f, 0.0f, UILayoutAlignment::Fixed, UILayoutAlignment::Fill };

		layoutManager.add(
			firstCol,
			uiLayoutStyle,
			layoutChangedHandler,
			_CommandPanel.get()->getWindowHandle(),
			0
		);
		firstCol = false;
	}
	if (_View)
	{
		UILayoutStyle uiLayoutStyle{ 0.0f, 0.0f, UILayoutAlignment::Fill, UILayoutAlignment::Fill };

		layoutManager.add(
			firstCol,
			uiLayoutStyle,
			layoutChangedHandler,
			_View.get()->getWindowHandle(),
			0
		);
		firstCol = false;
	}
	if (_PropertyPanel)
	{
		UILayoutStyle uiLayoutStyle{ 300.0f, 0.0f, UILayoutAlignment::Fixed, UILayoutAlignment::Fill };

		layoutManager.add(
			firstCol,
			uiLayoutStyle,
			layoutChangedHandler,
			_PropertyPanel.get()->getWindowHandle(),
			0
		);
		firstCol = false;
	}

	layoutManager.recalcLayout(static_cast<UICoord>(cx), static_cast<UICoord>(cy));
}



