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
	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), getConfig()._TemplateFilePath);
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
	std::wstring filePath = getConfig()._TemplateFilePath;

	std::wstring         _FileDialog_DefExt     { L"VcTemplate.xml" };
	std::wstring         _FileDialog_InitialDir { L"C:\\" };
	std::wstring         _FileDialog_Title      { L"" };
	static const wchar_t _FileDialog_Filter[]   { L"VcTemplate.xml File(*.VcTemplate.xml)\0*.VcTemplate.xml\0All Files(*.*)\0*.*\0"  };

	_FileDialog_InitialDir = cx::wfs::get_directory_of_current_process();
	cx::wui::OpenFileDialog openFileDialog(
		filePath, L"",
		_FileDialog_Filter,
		_FileDialog_DefExt,
		_FileDialog_InitialDir,
		_FileDialog_Title
	);

	if (openFileDialog.doModal(*this, filePath) == false)
	{
		return;
	}

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT1), filePath);
}

void MainBox::onButton2(cx::wui::WindowMessage& windowMessage)
{
	std::wstring directory;
	
	cx::wui::FolderDialog folderDialog(L"Target Directory");
	if (folderDialog.doModal(*this, directory) == false)
	{
		return;
	}

	cx::wui::setWindowText(::GetDlgItem(*this, IDC_EDIT2), directory);
}

//===========================================================================
void MainBox::onButton3(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	std::wstring edit1=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT1));
	std::wstring edit2=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT2));
	std::wstring edit3=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT3));
	std::wstring edit4=cx::wui::getWindowText(::GetDlgItem(*this, IDC_EDIT4));

	CX_RUNTIME_LOG(cxLDebug)  << std::endl
		<< L"edit1=" << edit1 << std::endl
		<< L"edit2=" << edit2 << std::endl
		<< L"edit3=" << edit3 << std::endl
		<< L"edit4=" << edit4;

	if(edit1.empty()) { MessageBox(*this, L"템플릿 파일 경로를 선택하십시오.", L"에러", MB_OK); return; }
	if(edit2.empty()) { MessageBox(*this, L"대상 디렉토리 경로를 선택하십시오.", L"에러", MB_OK); return; }
	if(edit3.empty()) { MessageBox(*this, L"솔루션 이름을 입력하세요.", L"에러", MB_OK); return; }
	if(edit4.empty()) { MessageBox(*this, L"프로젝트 이름을 입력하세요.", L"에러", MB_OK); return; }


	//-----------------------------------------------------------------------
	bool rv;
	rv = _VcFileGenerator->generate(edit1, edit2, edit3, edit4);
	if (false == rv)
	{
		MessageBox(*this, L"파일 생성 실패", L"에러", MB_OK);
		return;
	}

	MessageBox(*this, L"파일 생성 성공", L"완료", MB_OK);
}

