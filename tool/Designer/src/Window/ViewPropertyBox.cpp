/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>
#include <cx/common/to_std_wstring.hpp>
#include <cx/wui/wui.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "ViewPropertyBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define IDC_EDIT_CX              1001
#define IDC_EDIT_CY              1002
#define IDC_EDIT_COLOR           1003
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
		w.CONTROL_EDITTEXT     (                    IDC_EDIT_CX              , 55,  25,  80,  15, ES_NUMBER);
		w.CONTROL_LTEXT        (L"픽셀"           , IDC_STATIC               ,140,  25,  40,  15, SS_CENTERIMAGE, 0);
																			 
		w.CONTROL_LTEXT        (L"높이:"          , IDC_STATIC               , 20,  45,  30,  15, SS_CENTERIMAGE, 0);
		w.CONTROL_EDITTEXT     (                    IDC_EDIT_CY              , 55,  45,  80,  15, ES_NUMBER);
		w.CONTROL_LTEXT        (L"픽셀"           , IDC_STATIC               ,140,  45,  40,  15, SS_CENTERIMAGE, 0);
																			 
		w.CONTROL_LTEXT        (L"배경색:"        , IDC_STATIC               , 20,  70,  30,  15, SS_CENTERIMAGE, 0);
		w.CONTROL_EDITTEXT     (                    IDC_EDIT_COLOR           , 55,  70,  80,  15, ES_UPPERCASE | ES_READONLY );
		w.CONTROL_PUSHBUTTON   (L"선택..."        , IDC_BUTTON_COLOR_PICKER  ,140,  70,  40,  15);
//		w.CONTROL_LTEXT        (L"(예: #FFFFFF)"  , IDC_STATIC               , 55,  90,  80,  15, SS_CENTERIMAGE, 0);
																			 
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
	std::wstring cxText = cx::to_std_wstring(_CX);
	std::wstring cyText = cx::to_std_wstring(_CY);
	std::wstring colorText = std::format(L"{:02X}{:02X}{:02X}", _ColorR8, _ColorG8, _ColorR8);

	::SetDlgItemTextW(*this, IDC_EDIT_CX, cxText.c_str());
	::SetDlgItemTextW(*this, IDC_EDIT_CY, cyText.c_str());
	::SetDlgItemTextW(*this, IDC_EDIT_COLOR, colorText.c_str());


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void ViewPropertyBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDC_BUTTON_COLOR_PICKER:
		pickColor();
		windowMessage.setResult(TRUE);
		break;

	case IDOK:
		updateData();
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

void ViewPropertyBox::pickColor(void)
{
	std::uint8_t r = _ColorR8;
	std::uint8_t g = _ColorG8;
	std::uint8_t b = _ColorB8;

	cx::wui::ColorPickerDialog dialog;
	if (IDOK == dialog.doModal(*this, r, g, b))
	{
		std::wstring colorText = std::format(L"{:02X}{:02X}{:02X}", r, g, b);
		::SetDlgItemTextW(*this, IDC_EDIT_COLOR, colorText.c_str());

		_ColorR8 = r;
		_ColorG8 = g;
		_ColorB8 = b;
	}
}

void ViewPropertyBox::updateData(void)
{
	std::uint32_t cx;
	std::uint32_t cy;


	BOOL translatedFlag;


	translatedFlag = FALSE;
	cx = GetDlgItemInt(*this, IDC_EDIT_CX, reinterpret_cast<BOOL*>(&translatedFlag), FALSE);
	if (translatedFlag)
	{
		_CX = cx;
	}

	translatedFlag = FALSE;
	cy = GetDlgItemInt(*this, IDC_EDIT_CY, reinterpret_cast<BOOL*>(&translatedFlag), FALSE);
	if (translatedFlag)
	{
		_CY = cy;
	}
}
