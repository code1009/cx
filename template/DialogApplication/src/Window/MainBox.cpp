/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <commdlg.h>

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>
#include <common/fs_std_wstring.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "MainBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MainBox::MainBox()
{
	setTemplateName(IDD_MAIN);
	registerWindowMessageMap();
}

//===========================================================================
MainBox::~MainBox()
{
}

//===========================================================================
void MainBox::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_INITDIALOG) = &MainBox::onInitDialog;
	_WindowMessageMap.handle(WM_COMMAND   ) = &MainBox::onCommand;
}

void MainBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
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

