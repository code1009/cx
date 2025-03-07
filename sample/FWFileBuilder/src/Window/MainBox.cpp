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
#include "../FWFileBuilder.hpp"

//===========================================================================
#include "MainBox.hpp"
#include "FileDialog.hpp"





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
	_FWFileBuilder->terminate();
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
	_FWFileBuilder->initialize();
	

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
	case IDC_BUTTON4:onButton4(windowMessage);break;

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
wchar_t _defaultFilter[] = { L"himage File(*.himage)\0*.himage\0All Files(*.*)\0*.*\0"  };
std::wstring _defaultDefExt    { L"himage" };
std::wstring _defaultInitialDir{ L"C:\\" };
//std::wstring _defaultTitle     { L"File Open" };
std::wstring _defaultTitle     { L"" };

void MainBox::onButton1(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton1";
	
	
	//-----------------------------------------------------------------------
	_defaultInitialDir = cx::wfs::get_directory_of_current_process();
	bool rv;		
	OpenFileDialog openFileDialog(L"", L"",_defaultFilter, _defaultDefExt, _defaultInitialDir, _defaultTitle);
	std::wstring filePath;
	rv = openFileDialog.doModal(*this, filePath);
	OutputDebugStringW(L"\n");
	if (false == rv)
	{
		return;
	}

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), filePath);
}

void MainBox::onButton2(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton2";

	//-----------------------------------------------------------------------
	_defaultInitialDir = cx::wfs::get_directory_of_current_process();
	bool rv;		
	OpenFileDialog openFileDialog(L"", L"",_defaultFilter, _defaultDefExt, _defaultInitialDir, _defaultTitle);
	std::wstring filePath;
	rv = openFileDialog.doModal(*this, filePath);
	OutputDebugStringW(L"\n");
	if (false == rv)
	{
		return;
	}

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT2), filePath);
}

void MainBox::onButton3(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton3";

	//-----------------------------------------------------------------------
	_defaultInitialDir = cx::wfs::get_directory_of_current_process();
	bool rv;		
	OpenFileDialog openFileDialog(L"", L"",_defaultFilter, _defaultDefExt, _defaultInitialDir, _defaultTitle);
	std::wstring filePath;
	rv = openFileDialog.doModal(*this, filePath);
	OutputDebugStringW(L"\n");
	if (false == rv)
	{
		return;
	}

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT3), filePath);
}

void MainBox::onButton4(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton4";


	//-----------------------------------------------------------------------
	std::wstring edit1=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT1));
	std::wstring edit2=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT2));
	std::wstring edit3=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT3));

	CX_RUNTIME_LOG(cxLDebug)
		<< L"edit1=" << edit1 << std::endl
		<< L"edit2=" << edit2 << std::endl
		<< L"edit3=" << edit3;

	if(edit1.empty()) { MessageBox(*this, L"파일을 선택하십시오.", L"에러", MB_OK); return; }
	if(edit2.empty()) { MessageBox(*this, L"파일을 선택하십시오.", L"에러", MB_OK); return; }
	if(edit3.empty()) { MessageBox(*this, L"파일을 선택하십시오.", L"에러", MB_OK); return; }


	//-----------------------------------------------------------------------
	bool rv;
	rv = _FWFileBuilder->makeImage(edit1, edit2, edit3);
	if (false == rv)
	{
		MessageBox(*this, L"이미지 생성 실패", L"에러", MB_OK);
		return;
	}


	//-----------------------------------------------------------------------
	_defaultInitialDir = cx::wfs::get_directory_of_current_process();
	SaveFileDialog saveFileDialog(L"", L"",_defaultFilter, _defaultDefExt, _defaultInitialDir, _defaultTitle);
	std::wstring filePath;
	rv = saveFileDialog.doModal(*this, filePath);
	OutputDebugStringW(L"\n");
	if (false == rv)
	{
		return;
	}


	//-----------------------------------------------------------------------
	rv = _FWFileBuilder->saveImageFile(filePath);
	if (false == rv)
	{
		MessageBox(*this, L"이미지 저장 실패", L"에러", MB_OK);
		return;
	}
}

