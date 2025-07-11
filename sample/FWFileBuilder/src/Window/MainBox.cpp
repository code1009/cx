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
#include "../FWFileBuilder.hpp"
#include "../Config.hpp"

//===========================================================================
#include "MainBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static const wchar_t _FileDialog_himage_Filter[] { L"himage File(*.himage)\0*.himage\0All Files(*.*)\0*.*\0"  };
static const wchar_t _FileDialog_bin_Filter   [] { L"bin File(*.bin)\0*.bin\0All Files(*.*)\0*.*\0"  };





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MainBox::MainBox()
{
	_FWFileBuilder = std::make_unique<FWFileBuilder>();

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
	std::wstring filePath1 = getConfig()._BL_FilePath   ;
	std::wstring filePath2 = getConfig()._SA_FilePath   ;
	std::wstring filePath3 = getConfig()._MA_FilePath   ;
	std::wstring filePath4 = getConfig()._Image_FilePath;
	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), filePath1);
	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT2), filePath2);
	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT3), filePath3);

	
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
void MainBox::onButton1(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton1";
	
	
	//-----------------------------------------------------------------------
	std::wstring filePath1 = getConfig()._BL_FilePath   ;


	//-----------------------------------------------------------------------
	bool rv;

	_FileDialog_himage_InitialDir = cx::wfs::get_directory_of_current_process();
	cx::wui::OpenFileDialog openFileDialog(
		filePath1, L"",
		_FileDialog_bin_Filter, 
		_FileDialog_bin_DefExt, 
		_FileDialog_bin_InitialDir, 
		_FileDialog_bin_Title
	);
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
	std::wstring filePath2 = getConfig()._SA_FilePath   ;


	//-----------------------------------------------------------------------
	bool rv;

	_FileDialog_himage_InitialDir = cx::wfs::get_directory_of_current_process();
	cx::wui::OpenFileDialog openFileDialog(
		filePath2, L"",
		_FileDialog_himage_Filter, 
		_FileDialog_himage_DefExt, 
		_FileDialog_himage_InitialDir, 
		_FileDialog_himage_Title
	);
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
	std::wstring filePath3 = getConfig()._MA_FilePath;


	//-----------------------------------------------------------------------
	bool rv;

	_FileDialog_himage_InitialDir = cx::wfs::get_directory_of_current_process();
	cx::wui::OpenFileDialog openFileDialog(
		filePath3, L"",
		_FileDialog_himage_Filter, 
		_FileDialog_himage_DefExt, 
		_FileDialog_himage_InitialDir, 
		_FileDialog_himage_Title
	);
	std::wstring filePath;
	rv = openFileDialog.doModal(*this, filePath);
	OutputDebugStringW(L"\n");
	if (false == rv)
	{
		return;
	}

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT3), filePath);
}

//===========================================================================
void MainBox::onButton4(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"MainBox::onButton4";


	//-----------------------------------------------------------------------
	std::wstring filePath4 = getConfig()._Image_FilePath;


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
	_FileDialog_himage_InitialDir = cx::wfs::get_directory_of_current_process();
	cx::wui::SaveFileDialog saveFileDialog(
		filePath4, L"",
		_FileDialog_bin_Filter, 
		_FileDialog_bin_DefExt, 
		_FileDialog_bin_InitialDir, 
		_FileDialog_bin_Title
	);
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

