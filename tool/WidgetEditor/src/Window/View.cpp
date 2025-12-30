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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR View_WindowClassName = L"View";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
View::View(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	cx::wui::WindowClass windowClass;


	windowClass.registerWindowClass(
		View_WindowClassName
	);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createView(parentWindowHandle);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("View::View(): createView() failed");
	}


	//-----------------------------------------------------------------------
	_Window = std::make_unique<cx::gw::WidgetDesignerWindow>(*this);

	_Window->getStatusOverayPanel()->setVisible(true);
	_Window->getDocumentGrid()->setVisible(true);


	//-----------------------------------------------------------------------
	_Window->getWidgetDesignerModel()->getWidgetEventDragDropHandler()->registerDragDrop();


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);
}

//===========================================================================
View::~View()
{
}

//===========================================================================
LRESULT View::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	auto result = cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>::onWindowMessage(windowMessage);

	if (_Window)
	{
		_Window->getWidgetDesignerModel()->getWidgetEventWindowMessageHandler()->onWindowMessage(
			windowMessage.hWnd,
			windowMessage.uMsg,
			windowMessage.wParam,
			windowMessage.lParam
		);
	}

	return result;
}

//===========================================================================
HWND View::createView(HWND parentWindowHandle)
{
	LPCWSTR   lpszClassName = View_WindowClassName;
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpWindowName  = L"Window";
	DWORD     dwStyle       = cx::wui::ChildWindowStyle;
	DWORD     dwExStyle     = cx::wui::ChildWindowStyleEx;


	//dwStyle |= WS_VSCROLL;
	//dwStyle |= WS_HSCROLL;
	dwExStyle |= WS_EX_CLIENTEDGE;


	HWND hwnd;
	hwnd = createWindow(
		lpszClassName,
		hWndParent,
		lpWindowName,
		dwStyle,
		dwExStyle
	);

	return hwnd;
}

//===========================================================================
void View::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE    ) = &View::onCreate;
	_WindowMessageMap.handle(WM_DESTROY   ) = &View::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE     ) = &View::onClose;

	_WindowMessageMap.handle(WM_SIZE      ) = &View::onSize;
	_WindowMessageMap.handle(WM_HSCROLL   ) = &View::onHScroll;
	_WindowMessageMap.handle(WM_VSCROLL   ) = &View::onVScroll;

	_WindowMessageMap.handle(WM_MOUSEWHEEL) = &View::onMouseWheel;
	_WindowMessageMap.handle(WM_MOUSEMOVE ) = &View::onMouseMove;

	_WindowMessageMap.handle(WM_KEYDOWN   ) = &View::onKeyDown;

	_WindowMessageMap.handle(WM_ERASEBKGND) = &View::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT     ) = &View::onPaint;

	_WindowMessageMap.handle(WM_COMMAND   ) = &View::onCommand;
}

void View::onCreate(cx::wui::WindowMessage& windowMessage)
{
}

void View::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	// 성공해야 메모리릭이 없음!
	_Window->getWidgetDesignerModel()->getWidgetEventDragDropHandler()->unregisterDragDrop();
}

void View::onClose(cx::wui::WindowMessage& windowMessage)
{
}

void View::onSize(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	RECT rect;

	GetClientRect(*this, &rect);


	//-----------------------------------------------------------------------
	UINT cx;
	UINT cy;


	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	//-----------------------------------------------------------------------
	if (_Window)
	{
		_Window->getViewport()->setWindowSize(cx, cy);
	}
}

void View::onHScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };


	_Window->getViewport()->handleHScrollbar(wm.nSBCode());
}

void View::onVScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_VSCROLL_WindowMessageCrack wm{ windowMessage };


	_Window->getViewport()->handleVScrollbar(wm.nSBCode());
}

void View::onMouseWheel(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_MOUSEWHEEL_WindowMessageCrack wm{ windowMessage };

	UINT fwKeys = GET_KEYSTATE_WPARAM(windowMessage.wParam);
	bool scale  = false;


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
			viewZoomIn();
		}
		else
		{
			viewZoomOut();
		}
	}
	else
	{
		if (wm.zDelta() > 0)
		{
			viewVScrollUp();
		}
		else
		{
			viewVScrollDown();
		}
	}
}

void View::onMouseMove(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };
	
	POINT pt = wm.point();


	_Window->getStatusOverayPanel()->setMousePosition(pt.x, pt.y);
}

void View::onKeyDown(cx::wui::WindowMessage& windowMessage)
{
	//--------------------------------------------------------------------------
	cx::wui::WM_KEYDOWN_WindowMessageCrack wm{ windowMessage };


	//--------------------------------------------------------------------------
	UINT virtualKeyCode;
	int  repeatCount;
	int  oemScanCode;
	int  extendedKey;
	int  contextCode;
	int  reserved;
	int  previousKeyState;
	int  transitionState;


	//--------------------------------------------------------------------------
	virtualKeyCode   =  wm.nChar();
	repeatCount      =  wm.nRepCnt();
	oemScanCode      = (wm.nFlags() >> (16-16)) & 0xff;
	extendedKey      = (wm.nFlags() >> (24-16)) & 1;    // The value is 1 if it is an extended key; otherwise, it is 0.
	reserved         = (wm.nFlags() >> (25-16)) & 0xf;  // 25~28 Reserved; do not use.
	contextCode      = (wm.nFlags() >> (29-16)) & 1;    // The value is always 0 for a WM_KEYDOWN message.
	previousKeyState = (wm.nFlags() >> (30-16)) & 1;    // The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
	transitionState  = (wm.nFlags() >> (31-16)) & 1;    // The value is always 0 for a WM_KEYDOWN message.

	//--------------------------------------------------------------------------
	constexpr int VK_A = 0x41;
	constexpr int VK_B = 0x42;
	constexpr int VK_C = 0x43;
	constexpr int VK_D = 0x44;
	constexpr int VK_E = 0x45;
	constexpr int VK_F = 0x46;
	constexpr int VK_G = 0x47;
	constexpr int VK_H = 0x48;
	constexpr int VK_I = 0x49;
	constexpr int VK_J = 0x4A;
	constexpr int VK_K = 0x4B;
	constexpr int VK_L = 0x4C;
	constexpr int VK_M = 0x4D;
	constexpr int VK_N = 0x4E;
	constexpr int VK_O = 0x4F;
	constexpr int VK_P = 0x50;
	constexpr int VK_Q = 0x51;
	constexpr int VK_R = 0x52;
	constexpr int VK_S = 0x53;
	constexpr int VK_T = 0x54;
	constexpr int VK_U = 0x55;
	constexpr int VK_V = 0x56;
	constexpr int VK_W = 0x57;
	constexpr int VK_X = 0x58;
	constexpr int VK_Y = 0x59;
	constexpr int VK_Z = 0x5A;
	constexpr int VK_0 = 0x30;
	constexpr int VK_1 = 0x31;
	constexpr int VK_2 = 0x32;
	constexpr int VK_3 = 0x33;
	constexpr int VK_4 = 0x34;
	constexpr int VK_5 = 0x35;
	constexpr int VK_6 = 0x36;
	constexpr int VK_7 = 0x37;
	constexpr int VK_8 = 0x38;
	constexpr int VK_9 = 0x39;


	//--------------------------------------------------------------------------
	SHORT asyncKeyState;
	bool ctrlPressed;
	bool ctrlPrevPressed;	
	bool shiftPressed;
	bool shiftPrevPressed;


	asyncKeyState    = GetAsyncKeyState(VK_CONTROL);
	ctrlPressed      = (asyncKeyState & 0x8000) ? true : false;
	ctrlPrevPressed  = (asyncKeyState & 0x0001) ? true : false;

	asyncKeyState    = GetAsyncKeyState(VK_SHIFT);
	shiftPressed     = (asyncKeyState & 0x8000) ? true : false;
	shiftPrevPressed = (asyncKeyState & 0x0001) ? true : false;


	//--------------------------------------------------------------------------
	enum class ShutcutCommand
	{
		Unknown,
		
		EditUndo,
		EditRedo,
		
		EditCopy,
		EditCut,
		EditPaste,
		
		EditInsert,
		EditDelete,
		
		EditEnter,
		EditCancel,
		
		EditSelectAll,
		EditDeselectAll,
		
		EditMoveCursorLeft,
		EditMoveCursorRight,
		EditMoveCursorUp,
		EditMoveCursorDown,
		
		EditHomeCursorFirst,
		EditHomeCursorLast,
		
		ViewPageUp,
		ViewPageDown,
		ViewPageTop,
		ViewPageBottom,

		ViewFitScroll,

		ViewDocumentGrid,
		ViewStatusOverlay,
	};


	//--------------------------------------------------------------------------
	ShutcutCommand shutcutCommand = ShutcutCommand::Unknown;


	//--------------------------------------------------------------------------
	switch (virtualKeyCode)
	{
	case VK_INSERT: shutcutCommand = ShutcutCommand::EditInsert; break;
	case VK_DELETE: shutcutCommand = ShutcutCommand::EditDelete; break;
	}
	//--------------------------------------------------------------------------
	if (ctrlPressed)
	{
		switch (virtualKeyCode)
		{
		case VK_Z: shutcutCommand = ShutcutCommand::EditUndo ; break;
		case VK_Y: shutcutCommand = ShutcutCommand::EditRedo ; break;
		case VK_C: shutcutCommand = ShutcutCommand::EditCopy ; break;
		case VK_X: shutcutCommand = ShutcutCommand::EditCut  ; break;
		case VK_V: shutcutCommand = ShutcutCommand::EditPaste; break;
		}
	}
	switch (virtualKeyCode)
	{
	case VK_RETURN: shutcutCommand = ShutcutCommand::EditEnter; break;
	case VK_ESCAPE: shutcutCommand = ShutcutCommand::EditCancel; break;
	}
	if (ctrlPressed)
	{
		switch (virtualKeyCode)
		{
		case VK_A: shutcutCommand = ShutcutCommand::EditSelectAll; break;
		case VK_U: shutcutCommand = ShutcutCommand::EditDeselectAll; break;
		}
	}
	//--------------------------------------------------------------------------
	switch (virtualKeyCode)
	{
	case VK_LEFT : shutcutCommand = ShutcutCommand::EditMoveCursorLeft ; break;
	case VK_RIGHT: shutcutCommand = ShutcutCommand::EditMoveCursorRight; break;
	case VK_UP   : shutcutCommand = ShutcutCommand::EditMoveCursorUp   ; break;
	case VK_DOWN : shutcutCommand = ShutcutCommand::EditMoveCursorDown ; break;
	case VK_HOME : shutcutCommand = ShutcutCommand::EditHomeCursorFirst; break;
	case VK_END  : shutcutCommand = ShutcutCommand::EditHomeCursorLast ; break;
	}
	//--------------------------------------------------------------------------
	switch (virtualKeyCode)
	{
	case VK_PRIOR: shutcutCommand = ShutcutCommand::ViewPageUp  ; break;
	case VK_NEXT : shutcutCommand = ShutcutCommand::ViewPageDown; break;
	}
	if (ctrlPressed)
	{
		switch (virtualKeyCode)
		{
		case VK_PRIOR: shutcutCommand = ShutcutCommand::ViewPageTop ; break;
		case VK_NEXT : shutcutCommand = ShutcutCommand::ViewPageDown; break;
		}
	}
	switch (virtualKeyCode)
	{
	case VK_BACK: shutcutCommand = ShutcutCommand::ViewFitScroll; break;
	}
	switch (virtualKeyCode)
	{
	case VK_F7: shutcutCommand = ShutcutCommand::ViewDocumentGrid; break;
	case VK_F8: shutcutCommand = ShutcutCommand::ViewStatusOverlay; break;
	}


	//--------------------------------------------------------------------------
	switch(shutcutCommand)
	{
	case ShutcutCommand::EditUndo:
		editUndo();
		break;
	case ShutcutCommand::EditRedo:
		editRedo();
		break;

	case ShutcutCommand::EditCopy:
	case ShutcutCommand::EditCut:
	case ShutcutCommand::EditPaste:
		break;

	//case ShutcutCommand::EditInsert:
	//	break;
	case ShutcutCommand::EditDelete:
		editDelete();
		break;

	//case ShutcutCommand::EditEnter:
	//	break;
	//case ShutcutCommand::EditCancel:
	//	break;

	case ShutcutCommand::EditSelectAll:
		editSelectAll();
		break;
	case ShutcutCommand::EditDeselectAll:
		editDeselectAll();
		break;

	//case ShutcutCommand::EditMoveCursorLeft:
	//case ShutcutCommand::EditMoveCursorRight:
	//case ShutcutCommand::EditMoveCursorUp:
	//case ShutcutCommand::EditMoveCursorDown:
	//	break;

	//case ShutcutCommand::EditHomeCursorFirst:
	//case ShutcutCommand::EditHomeCursorLast:
	//	break;
	
	//case ShutcutCommand::ViewPageUp:
	//case ShutcutCommand::ViewPageDown:
	case ShutcutCommand::ViewPageTop:
		editBringToTop();
		break;
	case ShutcutCommand::ViewPageBottom:
		editSendToBottom();
		break;

	//case ShutcutCommand::ViewFitScroll:
	//	break;

	case ShutcutCommand::ViewDocumentGrid:
		viewDocumentGrid();
		break;
	case ShutcutCommand::ViewStatusOverlay:
		viewStatusOverlay();
		break;

	case ShutcutCommand::Unknown:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void View::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void View::onPaint(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	_Window->render();


	//-----------------------------------------------------------------------
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

void View::onCommand(cx::wui::WindowMessage& windowMessage)
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

void View::onMenuCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	bool rv;
	rv = executeMenuCommand(wm.nID());
	if (rv)
	{
		windowMessage.setResult(1);
	}
	else
	{
		windowMessage.setResult(0);
		//defaultWindowProc(windowMessage);
	}
}

void View::onCtlCommand(cx::wui::WindowMessage& windowMessage)
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

//===========================================================================
void View::onIdle(void)
{
	_Window->render();
}

//===========================================================================
void View::doDragDrop(std::wstring data)
{
	cx::gw::WidgetEventDragDropData dragDropData;

	dragDropData._String = data;
	_WidgetEventDragDropNotifier.setDragDropData(dragDropData);
	_WidgetEventDragDropNotifier.doDragDrop();
}

//===========================================================================
#if 0
IDC_MAIN MENU
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "&New",                        IDM_FILE_NEW
        MENUITEM SEPARATOR
        MENUITEM "&Open",                       IDM_FILE_OPEN
        MENUITEM "&Save",                       IDM_FILE_SAVE
        MENUITEM "&Save As",                    IDM_FILE_SAVEAS
        MENUITEM SEPARATOR
        MENUITEM "E&xit",                       IDM_FILE_EXIT
    END
    POPUP "&Edit"
    BEGIN
        MENUITEM "Undo\tCtrl+Z",                IDM_EDIT_UNDO, GRAYED
        MENUITEM "Redo\tCtrl+Y",                IDM_EDIT_REDO, GRAYED
        MENUITEM SEPARATOR
        MENUITEM "Cut\tCtrl+X",                 IDM_EDIT_CUT, GRAYED
        MENUITEM "Copy\tCtrl+C",                IDM_EDIT_COPY, GRAYED
        MENUITEM "Paste\tCtrl+V",               IDM_EDIT_PASTE, GRAYED
        MENUITEM "Delete\tDel",                 IDM_EDIT_DELETE, GRAYED
        MENUITEM SEPARATOR
        MENUITEM "Select All\tCtrl+A",          IDM_EDIT_SELECT_ALL, GRAYED
        MENUITEM "Deselect All\tCtrl+U",        IDM_EDIT_DESELECT_ALL, GRAYED
        MENUITEM SEPARATOR
        MENUITEM "Snap To Grid",                IDM_EDIT_SNAP_TO_GRID, GRAYED
        MENUITEM "Bring to Top\tCtrl+PgUp",     IDM_EDIT_BRING_TO_TOP, GRAYED
        MENUITEM "Send to Bottom\tCtrl+PgDn",   IDM_EDIT_SEND_TO_BOTTOM, GRAYED
    END
    POPUP "&View"
    BEGIN
        MENUITEM "&Grid",                       IDM_VIEW_DOCUMENTGRID, CHECKED
        MENUITEM "&Status",                     IDM_VIEW_STATUSOVERLAY, CHECKED
    END
    POPUP "&Help"
    BEGIN
        MENUITEM "&About",                      IDM_HELP_ABOUT
    END
END
#endif 

//===========================================================================
bool View::executeMenuCommand(std::uint32_t menuCommandId)
{
	switch (menuCommandId)
	{
	case IDM_FILE_NEW           : fileNew();           return true;;
	case IDM_FILE_OPEN          : fileOpen();          return true;;
	case IDM_FILE_SAVE          : fileSave();          return true;;
	case IDM_FILE_SAVEAS        : fileSaveAs();        return true;;
	case IDM_FILE_PRINT         : filePrint();         return true;;
	case IDM_EDIT_UNDO          : editUndo();          return true;;
	case IDM_EDIT_REDO          : editRedo();          return true;;
	case IDM_EDIT_CUT           : editCopy();          return true;;
	case IDM_EDIT_COPY          : editCut();           return true;;
	case IDM_EDIT_PASTE         : editPaste();         return true;;
	case IDM_EDIT_DELETE        : editDelete();        return true;;
	case IDM_EDIT_SELECT_ALL    : editSelectAll();     return true;;
	case IDM_EDIT_DESELECT_ALL  : editDeselectAll();   return true;;
	case IDM_EDIT_SNAP_TO_GRID  : editSnapToGrid();    return true;;
	case IDM_EDIT_BRING_TO_TOP  : editBringToTop();    return true;;
	case IDM_EDIT_SEND_TO_BOTTOM: editSendToBottom();  return true;;
	case IDM_VIEW_DOCUMENTGRID  : viewDocumentGrid();  return true;;
	case IDM_VIEW_STATUSOVERLAY : viewStatusOverlay(); return true;;
	default:
		break;
	}

	return false;
}

void View::fileNew(void)
{

}

void View::fileOpen(void)
{

}

void View::fileSave(void)
{

}

void View::fileSaveAs(void)
{

}

void View::filePrint(void)
{

}

void View::editUndo(void)
{
	_Window->getWidgetDesignerModel()->getWidgetDesigner()->undo();
}

void View::editRedo(void)
{
	_Window->getWidgetDesignerModel()->getWidgetDesigner()->redo();
}

void View::editCopy(void)
{

}

void View::editCut(void)
{

}

void View::editPaste(void)
{

}

void View::editDelete(void)
{
	_Window->getWidgetDesignerModel()->getWidgetDesigner()->deleteSelectedWidgets();
}

void View::editSelectAll(void)
{
	_Window->getWidgetDesignerModel()->getWidgetDesigner()->selectAllWidgets();
}

void View::editDeselectAll(void)
{
	_Window->getWidgetDesignerModel()->getWidgetDesigner()->deselectAllWidgets();
}

void View::editSnapToGrid(void)
{
	bool snapToGrid;
	snapToGrid = _Window->getWidgetDesignerModel()->getWidgetDesigner()->getSnapToGrid();
	_Window->getWidgetDesignerModel()->getWidgetDesigner()->setSnapToGrid(!snapToGrid);
}

void View::editBringToTop(void)
{

}

void View::editSendToBottom(void)
{

}

void View::viewZoomIn(void)
{
	_Window->getViewport()->zoom(true);
}

void View::viewZoomOut(void)
{
	_Window->getViewport()->zoom(false);
}

void View::viewVScrollUp(void)
{
	_Window->getViewport()->handleVScrollbar(SB_LINEUP);
}

void View::viewVScrollDown(void)
{
	_Window->getViewport()->handleVScrollbar(SB_LINEDOWN);
}

void View::viewDocumentGrid(void)
{
	bool visible;
	visible = _Window->getDocumentGrid()->getVisible();
	_Window->getDocumentGrid()->setVisible(!visible);
}

void View::viewStatusOverlay(void)
{
	bool visible;
	visible = _Window->getStatusOverayPanel()->getVisible();
	_Window->getStatusOverayPanel()->setVisible(!visible);
}
