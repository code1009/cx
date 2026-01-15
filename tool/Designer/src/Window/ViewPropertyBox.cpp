/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "ViewPropertyBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define IDC_EDIT_WIDTH        1001
#define IDC_EDIT_HEIGHT       1002
#define IDC_EDIT_BACKGROUND_COLOR     1003
#define IDC_BUTTON_COLOR_PICKER  1004





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ViewPropertyBox::ViewPropertyBox()
{
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void ViewPropertyBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);


	w.BEGIN_DIALOG(0, 0, 200, 140);
		w.DIALOG_CAPTION(L"문서 설정");
		w.DIALOG_STYLE  (DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		w.DIALOG_FONT   (9, L"Segoe UI");
	w.END_DIALOG();
	w.BEGIN_CONTROLS_MAP();
		w.CONTROL_LTEXT        (L"넓이:"          , IDC_STATIC               , 20,  25,  30,  15, SS_CENTERIMAGE, 0);
		w.CONTROL_EDITTEXT     (                    IDC_EDIT_WIDTH           , 55,  25,  80,  15, ES_NUMBER);
		w.CONTROL_LTEXT        (L"픽셀"           , IDC_STATIC               ,140,  25,  40,  15, SS_CENTERIMAGE, 0);
																			 
		w.CONTROL_LTEXT        (L"높이:"          , IDC_STATIC               , 20,  45,  30,  15, SS_CENTERIMAGE, 0);
		w.CONTROL_EDITTEXT     (                    IDC_EDIT_HEIGHT          , 55,  45,  80,  15, ES_NUMBER);
		w.CONTROL_LTEXT        (L"픽셀"           , IDC_STATIC               ,140,  45,  40,  15, SS_CENTERIMAGE, 0);
																			 
		w.CONTROL_LTEXT        (L"배경색:"        , IDC_STATIC               , 20,  70,  30,  15, SS_CENTERIMAGE, 0);
		w.CONTROL_EDITTEXT     (                    IDC_EDIT_BACKGROUND_COLOR, 55,  70,  80,  15, ES_UPPERCASE);
		w.CONTROL_PUSHBUTTON   (L"선택..."        , IDC_BUTTON_COLOR_PICKER  ,140,  70,  40,  15);
		w.CONTROL_LTEXT        (L"(예: #FFFFFF)"  , IDC_STATIC               , 55,  90,  80,  15, SS_CENTERIMAGE, 0);
																			 
		w.CONTROL_GROUPBOX     (L"속성"           , IDC_STATIC               , 10,  10, 180, 105);
																			 
		w.CONTROL_DEFPUSHBUTTON(L"확인"           , IDOK                     , 75, 120,  50,  15);
		w.CONTROL_PUSHBUTTON   (L"취소"           , IDCANCEL                 ,130, 120,  50,  15);
	w.END_CONTROLS_MAP();


	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void ViewPropertyBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &ViewPropertyBox::onInitDialog;
	_WindowMessageMap[WM_COMMAND   ] = &ViewPropertyBox::onCommand;
}

void ViewPropertyBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	cx::wui::centerWindow(getWindowHandle());


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void ViewPropertyBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDOK:
		::EndDialog(*this, IDOK);
		windowMessage.setResult(TRUE);
		break;

	case IDCANCEL:
		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
		break;

	default:
		windowMessage.setResult(FALSE);
		break;
	}
}
