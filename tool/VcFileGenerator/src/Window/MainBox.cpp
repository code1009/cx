/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <commdlg.h>

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>
#include <component/fs_std_wstring.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "../VcFileGenerator/VcFileGenerator.hpp"
#include "../VcFileGenerator/Config.hpp"

//===========================================================================
#include "MainBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MainBox::MainBox()
{
	_VcFileGenerator = std::make_unique<VcFileGenerator>();

	setTemplateName(IDD_MAIN);
	registerWindowMessageMap();
}

//===========================================================================
MainBox::~MainBox()
{
	_VcFileGenerator->terminate();
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
	_VcFileGenerator->initialize();


	//-----------------------------------------------------------------------
	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), getConfig()._Source_Directory);
	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT2), L"");

	
	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void MainBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDC_BUTTON1:onButton1(windowMessage);break;
	case IDC_BUTTON2:onButton2(windowMessage);break;
	case IDC_BUTTON3:onButton3(windowMessage);break;

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

//===========================================================================
void MainBox::onButton1(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton1";
	
	
	//-----------------------------------------------------------------------
	std::wstring sourceDirectory = getConfig()._Source_Directory;

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), sourceDirectory);
}

void MainBox::onButton2(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton2";
	

	//-----------------------------------------------------------------------
	std::wstring targetDirectory;

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), targetDirectory);
}

//===========================================================================
void MainBox::onButton3(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton3";


	//-----------------------------------------------------------------------
	std::wstring sourceDirectory=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT1));
	std::wstring targetDirectory=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT2));

	CX_RUNTIME_LOG(cxLDebug)
		<< L"edit1=" << sourceDirectory << std::endl
		<< L"edit2=" << targetDirectory;

	if(sourceDirectory.empty()) { MessageBox(*this, L"경로를 선택하십시오.", L"에러", MB_OK); return; }
	if(targetDirectory.empty()) { MessageBox(*this, L"경로를 선택하십시오.", L"에러", MB_OK); return; }


	//-----------------------------------------------------------------------
	bool rv;
	rv = true; // _VcFileGenerator->generate(sourceDirectory, targetDirectory);
	if (false == rv)
	{
		MessageBox(*this, L"파일 생성 실패", L"에러", MB_OK);
		return;
	}

	MessageBox(*this, L"파일 생성 성공", L"완료", MB_OK);
}

