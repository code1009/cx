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
#include "MainBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment (lib, "Version.lib")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define IDC_ABOUTBOX_APP_VERSION 1001
#define IDC_ABOUTBOX_APP_NAME    1002
#define IDC_ABOUTBOX_APP_NOTE    1003





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MainBox::MainBox()
{
	setTemplateName(IDD_MAIN);
	registerWindowMessageMap();
}

//===========================================================================
void MainBox::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_INITDIALOG) = &MainBox::onInitDialog;
	_WindowMessageMap.handle(WM_COMMAND   ) = &MainBox::onCommand;
}

void MainBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void MainBox::onCommand(cx::wui::WindowMessage& windowMessage)
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
