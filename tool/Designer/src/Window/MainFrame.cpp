/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>
#include <cx/common/fs_std_wstring.hpp>
#include <cx/wui/wui.hpp>

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
	std::wstring appTitleString = cx::wui::loadString(IDS_APP_TITLE);


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createWindow(MainFrame_WindowClassName, nullptr, appTitleString.c_str());
	if (nullptr == hwnd)
	{
		throw std::runtime_error("MainFrame::MainFrame(): createWindow() failed");
	}


	//-----------------------------------------------------------------------
	_View = std::make_unique<View>(*this);
	_View->_Designer->modifiedHandler =
		[this]()
		{
			updateTitle();
		}
	;

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

	case IDM_FILE_NEW              : CX_RUNTIME_LOG(cxLInfo) << L"onFile_New              "; _View->_Designer->newFile();      break;
	case IDM_FILE_OPEN             : CX_RUNTIME_LOG(cxLInfo) << L"onFile_Open             "; _View->_Designer->openFile();     break;
	case IDM_FILE_SAVE             : CX_RUNTIME_LOG(cxLInfo) << L"onFile_Save             "; _View->_Designer->saveFile();     break;
	case IDM_FILE_SAVEAS           : CX_RUNTIME_LOG(cxLInfo) << L"onFile_SaveAs           "; _View->_Designer->saveFileAs();   break;
	case IDM_EDIT_UNDO             : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Undo             "; _View->_Designer->undo();         break;
	case IDM_EDIT_REDO             : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Redo             "; _View->_Designer->redo();         break;
	case IDM_EDIT_CUT              : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Cut              "; _View->_Designer->cut();          break;
	case IDM_EDIT_COPY             : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Copy             "; _View->_Designer->copy();         break;
	case IDM_EDIT_PASTE            : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Paste            "; _View->_Designer->paste();        break;
	case IDM_EDIT_SELECT_ALL       : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_SelectAll        "; _View->_Designer->selectAll();    break;
	case IDM_EDIT_DESELECT_ALL     : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_DeselectAll      "; _View->_Designer->deselectAll();  break;
	case IDM_EDIT_DELETE           : CX_RUNTIME_LOG(cxLInfo) << L"onEdit_Delete           "; _View->_Designer->erase();        break;

	case IDM_DESIGN_VIEW_COMMAND   : 
		onMenuCommand_DesignViewCommand();
		break;
		
	case IDM_DESIGN_VIEW_PROPERTY  : 
		onMenuCommand_DesignViewProperty();
		break;

	case IDM_DESIGN_ZOOMIN         : CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ZoomIn         "; _View->_Designer->zoomIn();            ; break;
	case IDM_DESIGN_ZOOMOUT        : CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ZoomOut        "; _View->_Designer->zoomOut();           ; break;
	case IDM_DESIGN_BRING_TO_FRONT : CX_RUNTIME_LOG(cxLInfo) << L"onDesign_BringToFront   "; _View->_Designer->bringToFront();      ; break;
	case IDM_DESIGN_SEND_TO_BACK   : CX_RUNTIME_LOG(cxLInfo) << L"onDesign_SendToBack     "; _View->_Designer->sendToBack();        ; break;
	case IDM_DESIGN_BRING_TO_TOP   : CX_RUNTIME_LOG(cxLInfo) << L"onDesign_BringToTop     "; _View->_Designer->bringToTop();        ; break;
	case IDM_DESIGN_SEND_TO_BOTTOM : CX_RUNTIME_LOG(cxLInfo) << L"onDesign_SendToBottom   "; _View->_Designer->sendToBottom();      ; break;
	case IDM_DESIGN_FILE_PROPERTIES: CX_RUNTIME_LOG(cxLInfo) << L"onDesign_FileProperties "; _View->_Designer->setViewProperties(); ; break;

	case IDM_DESIGN_SNAP_TO_GRID   :
		CX_RUNTIME_LOG(cxLInfo) << L"onDesign_SnapToGrid     ";
		toggleMenuItemChecked(wm.nID(), _MenuFlags.design_snapToGrid);
		_View->_Designer->snapToGrid (_MenuFlags.design_snapToGrid);
		break;

	case IDM_DESIGN_SHOW_GRID      : 
		CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ShowGrid       ";
		toggleMenuItemChecked(wm.nID(), _MenuFlags.design_showGrid);
		_View->_Designer->showGrid (_MenuFlags.design_showGrid);
		break;

	case IDM_DESIGN_SHOW_GRID_COORD: 
		CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ShowGridCoord  ";
		toggleMenuItemChecked(wm.nID(), _MenuFlags.design_showGridCoord);
		_View->_Designer->showGridCoord (_MenuFlags.design_showGridCoord);
		break;

	case IDM_DESIGN_SHOW_STATUS    : 
		CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ShowStatus     ";
		toggleMenuItemChecked(wm.nID(), _MenuFlags.design_showStatus);
		_View->_Designer->showStatus (_MenuFlags.design_showStatus);
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

void MainFrame::onMenuCommand_DesignViewCommand()
{
	CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ViewCommand    ";


	toggleMenuItemChecked(IDM_DESIGN_VIEW_COMMAND, _MenuFlags.design_viewCommand);


	if (_MenuFlags.design_viewCommand)
	{
		_CommandPanel = std::make_unique<CommandPanel>(*this, _View->_Designer.get());

	}
	else
	{
		if (_CommandPanel)
		{
			_CommandPanel->destroyWindow();
		}
		_CommandPanel.reset();
	}


	RECT rect;
	::GetClientRect(*this, &rect);

	UINT cx;
	UINT cy;
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	updateLayout(cx, cy);
}

void MainFrame::onMenuCommand_DesignViewProperty()
{
	CX_RUNTIME_LOG(cxLInfo) << L"onDesign_ViewProperty   ";

	toggleMenuItemChecked(IDM_DESIGN_VIEW_PROPERTY, _MenuFlags.design_viewProperty);

	
	if (_MenuFlags.design_viewProperty)
	{
		_PropertyPanel = std::make_unique<PropertyPanel>(*this, _View->_Designer.get());

	}
	else
	{
		if (_PropertyPanel)
		{
			_PropertyPanel->destroyWindow();
		}
		_PropertyPanel.reset();
	}


	RECT rect;
	::GetClientRect(*this, &rect);

	UINT cx;
	UINT cy;
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	updateLayout(cx, cy);
}

//===========================================================================
void MainFrame::onIdle(void)
{
	if (_View.get())
	{
//		_View->onIdle();
	}
}

//===========================================================================
void MainFrame::updateTitle(void)
{
	std::wstring appTitleString = cx::wui::loadString(IDS_APP_TITLE);
	std::wstring filePath = _View->_Designer->filePath();
	std::wstring fileName;
	std::wstring windowTitle;


	if ((!_View->_Designer->isModified()) && (filePath.empty()))
	{
		windowTitle = appTitleString;
	}
	else
	{
		if (filePath.empty())
		{
			fileName = L"제목 없음";
		}
		else
		{
			fileName = cx::wfs::get_file_name_of_file_path(filePath);
		}
		if (_View->_Designer->isModified())
		{
			fileName += L"*";
		}
		windowTitle = std::format(L"{} - {}", fileName, appTitleString);
	}

	::SetWindowTextW(*this, windowTitle.c_str());
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



