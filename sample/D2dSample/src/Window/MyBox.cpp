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
#include "MyBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MyBox::MyBox()
{
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void MyBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);

#if 0
	w.BEGIN_DIALOG(10, 10, 200, 100);
	w.DIALOG_CAPTION(L"My Dialog");
	w.DIALOG_STYLE(WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	w.DIALOG_FONT(8, L"MS Shell Dlg");
	w.END_DIALOG();

	w.BEGIN_CONTROLS_MAP();
	w.CONTROL_PUSHBUTTON(L"OK", IDOK, 50, 70, 50, 14, WS_TABSTOP);
	w.CONTROL_PUSHBUTTON(L"Cancel", IDCANCEL, 110, 70, 50, 14, WS_TABSTOP);
	w.END_CONTROLS_MAP();
#endif

	w.BEGIN_DIALOG(0, 0, 640, 240);
		w.DIALOG_CAPTION(L"Report");
		//w.DIALOG_STYLE(DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		//w.DIALOG_STYLE(DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME);
		w.DIALOG_STYLE(WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
		w.DIALOG_FONT(12, L"FixedSys");
	w.END_DIALOG();

	w.BEGIN_CONTROLS_MAP();
		w.CONTROL_LTEXT(L"Data", 1001, 0, 0,640, 20, SS_CENTERIMAGE);
		w.CONTROL_EDITTEXT(1002, 0,20,640,220, WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_READONLY);
	w.END_CONTROLS_MAP();

	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void MyBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &MyBox::onInitDialog;
	_WindowMessageMap[WM_DESTROY   ] = &MyBox::onDestroy;
	_WindowMessageMap[WM_CLOSE     ] = &MyBox::onClose;
	_WindowMessageMap[WM_COMMAND   ] = &MyBox::onCommand;
}

void MyBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MyBox::onInitDialog()\r\n");

	std::wstring text = L"MyBox::onInitDialog()\r\n";
	cx::wui::setWindowText(::GetDlgItem(*this, 1002), text);

	windowMessage.setResult(TRUE);
}

void MyBox::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MyBox::onDestroy()\r\n");

	windowMessage.setResult(TRUE);
}

void MyBox::onClose(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"MyBox::onClose()\r\n");

	windowMessage.setResult(FALSE);
}

void MyBox::onCommand(cx::wui::WindowMessage& windowMessage)
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
