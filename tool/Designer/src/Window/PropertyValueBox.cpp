/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "PropertyValueBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define IDC_ABOUTBOX_APP_VERSION 1001
#define IDC_ABOUTBOX_APP_NAME    1002
#define IDC_ABOUTBOX_APP_NOTE    1003





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
PropertyValueBox::PropertyValueBox()
{
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void PropertyValueBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);


	w.BEGIN_DIALOG(0, 0, 260, 150);
		w.DIALOG_CAPTION(L"정보");
		w.DIALOG_STYLE  (DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		w.DIALOG_FONT   (9, L"Segoe UI");
	w.END_DIALOG();
	w.BEGIN_CONTROLS_MAP();
		w.CONTROL_ICON         (MAKEINTRESOURCEW(IDI_APP_ICON)             , IDC_STATIC              , 10,  20,  20,  20);
		w.CONTROL_LTEXT        (L"어플리케이션"                            , IDC_ABOUTBOX_APP_NAME   , 35,  20, 210,  10);
		w.CONTROL_LTEXT        (L"버전 0.0.0.0"                            , IDC_ABOUTBOX_APP_VERSION, 35,  30, 210,  10);
		w.CONTROL_LTEXT        (L"Copyright (p) 2025 code1009."            , IDC_STATIC              , 35,  50, 210,  10);
		w.CONTROL_LTEXT        (L"All Rights Reserved."                    , IDC_STATIC              , 35,  60, 210,  10);
		w.CONTROL_LTEXT        (L""                                        , IDC_ABOUTBOX_APP_NOTE   , 35,  80, 210,  35);
		w.CONTROL_DEFPUSHBUTTON(L"확인"                                    , IDOK                    ,190, 130,  65,  15);
		w.CONTROL_GROUPBOX     (L""                                        , IDC_STATIC              ,  5,   5, 250, 120);
	w.END_CONTROLS_MAP();


	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void PropertyValueBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &PropertyValueBox::onInitDialog;
	_WindowMessageMap[WM_COMMAND   ] = &PropertyValueBox::onCommand;
	_WindowMessageMap[WM_NCACTIVATE] = &PropertyValueBox::onNcActivate;
	_WindowMessageMap[WM_ACTIVATE  ] = &PropertyValueBox::onActivate;
	_WindowMessageMap[WM_SETFOCUS] = &PropertyValueBox::onSetFocus;
	_WindowMessageMap[WM_KILLFOCUS] = &PropertyValueBox::onKillFocus;
	_WindowMessageMap[WM_USER+1] = &PropertyValueBox::onUser1;
}

LRESULT PropertyValueBox::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLTrace) <<
		L"PropertyValueBox::onWindowMessage(): uMsg=" << windowMessage.uMsg
		;
#endif
	return cx::wui::MessageMapWindowT<PropertyValueBox, cx::wui::ModalIndirectBaseDialog>::onWindowMessage(windowMessage);
}

void PropertyValueBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	cx::wui::centerWindow(getWindowHandle());


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void PropertyValueBox::onCommand(cx::wui::WindowMessage& windowMessage)
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

void PropertyValueBox::onNcActivate(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_NCACTIVATE_WindowMessageCrack wm{ windowMessage };

	BOOL active = wm.bActive();
	if (active)
	{
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_NCACTIVATE - active"
			;
	}
	else
	{
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_NCACTIVATE - no active"
			;

		//::EndDialog(*this, IDCANCEL);
		PostMessage(*this, WM_USER+1, 1009, 1);
	}
}

void PropertyValueBox::onActivate(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_ACTIVATE_WindowMessageCrack wm{ windowMessage };

	UINT state = wm.nState();
	switch (state)
	{
	case WA_ACTIVE:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_ACTIVATE - WA_ACTIVE"
			;
		break;

	case WA_CLICKACTIVE:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_ACTIVATE - WA_CLICKACTIVE"
			;
		break;

	case WA_INACTIVE:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_ACTIVATE - WA_INACTIVE"
			;
		break;

	default:
		break;
	}
}

void PropertyValueBox::onSetFocus(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_SETFOCUS_WindowMessageCrack wm{ windowMessage };

	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_SETFOCUS"
		;
}

void PropertyValueBox::onKillFocus(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_KILLFOCUS_WindowMessageCrack wm{ windowMessage };

	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_KILLFOCUS"
		;
}

void PropertyValueBox::onUser1(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_USER+1:"
		<< L" wParam=" << windowMessage.wParam
		<< L" lParam=" << windowMessage.lParam
		;

	if (windowMessage.wParam = 1009 && windowMessage.lParam == 1)
	{
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_USER+1 : EndDialog"
			;

		::EndDialog(*this, IDCANCEL);
	}
}
