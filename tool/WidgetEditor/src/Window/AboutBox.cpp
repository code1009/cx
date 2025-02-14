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
#include "AboutBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// winver.h(Windows.h 포함)
// Api-ms-win-core-version-l1-1-0.dll
#pragma comment (lib, "Version.lib")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define IDC_ABOUTBOX_APP_VERSION 1001
#define IDC_ABOUTBOX_APP_NAME    1002
#define IDC_ABOUTBOX_APP_NOTE    1003





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
AboutBox::AboutBox()
{
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void AboutBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);

	/*
	IDD_ABOUTBOX DIALOGEX 0, 0, 260, 150
	STYLE DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU
	CAPTION "정보"
	FONT 9, "Segoe UI", 0, 0, 0x0
	BEGIN
		GROUPBOX        "",IDC_STATIC,5,5,250,120
		ICON            IDI_MAIN,IDC_STATIC,10,20,20,20
		LTEXT           "어플리케이션",IDC_ABOUTBOX_APP_NAME,35,20,210,10
		LTEXT           "버전 0.0.0.0",IDC_ABOUTBOX_APP_VERSION,35,30,210,10
		LTEXT           "Copyright (c) 2025 <회사 이름> Co., Ltd.",IDC_STATIC,35,50,210,10
		LTEXT           "All Rights Reserved.",IDC_STATIC,35,60,210,10
		LTEXT           "",IDC_ABOUTBOX_APP_NOTE,35,80,210,35
		DEFPUSHBUTTON   "확인",IDOK,190,130,65,15
	END
	*/

	w.BEGIN_DIALOG(0, 0, 260, 150);
		w.DIALOG_CAPTION(L"정보");
		w.DIALOG_STYLE  (DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		w.DIALOG_FONT   (9, L"Segoe UI");
	w.END_DIALOG();
	w.BEGIN_CONTROLS_MAP();
		w.CONTROL_ICON         (MAKEINTRESOURCEW(IDI_MAIN)                 , IDC_STATIC              , 10,  20,  20,  20);
		w.CONTROL_LTEXT        (L"어플리케이션"                            , IDC_ABOUTBOX_APP_NAME   , 35,  20, 210,  10);
		w.CONTROL_LTEXT        (L"버전 0.0.0.0"                            , IDC_ABOUTBOX_APP_VERSION, 35,  30, 210,  10);
		w.CONTROL_LTEXT        (L"Copyright (c) 2025 <회사 이름> Co., Ltd.", IDC_STATIC              , 35,  50, 210,  10);
		w.CONTROL_LTEXT        (L"All Rights Reserved."                    , IDC_STATIC              , 35,  60, 210,  10);
		w.CONTROL_LTEXT        (L""                                        , IDC_ABOUTBOX_APP_NOTE   , 35,  80, 210,  35);
		w.CONTROL_DEFPUSHBUTTON(L"확인"                                    , IDOK                    ,190, 130,  65,  15);
		w.CONTROL_GROUPBOX     (L""                                        , IDC_STATIC              ,  5,   5, 250, 120);
	w.END_CONTROLS_MAP();


	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void AboutBox::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_INITDIALOG) = &AboutBox::onInitDialog;
	_WindowMessageMap.handle(WM_COMMAND   ) = &AboutBox::onCommand;
}

void AboutBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	int v0;
	int v1;
	int v2;
	int v3;


	v0 = 0;
	v1 = 0;
	v2 = 0;
	v3 = 0;


	//-----------------------------------------------------------------------
	wchar_t szFileName[MAX_PATH] = { '\0' };


	GetModuleFileNameW(
		cx::wui::getAppModule()->getInstanceHandle(),
		szFileName,
		sizeof(szFileName) / sizeof(wchar_t)
	);


	//-----------------------------------------------------------------------
	DWORD dwHandle = 0;
	DWORD dwLen = 0;


	dwLen = GetFileVersionInfoSize(szFileName, &dwHandle);
	if (dwLen)
	{
		std::vector<BYTE> data(dwLen);


		if (GetFileVersionInfoW(szFileName, dwHandle, dwLen, &data[0]))
		{
			VS_FIXEDFILEINFO* pInfo;
			UINT uLen;


			if (VerQueryValueW(&data[0], L"\\", reinterpret_cast<LPVOID*>(&pInfo), &uLen))
			{
				v0 = pInfo->dwProductVersionMS >> 16;
				v1 = pInfo->dwProductVersionMS & 0xffff;
				v2 = pInfo->dwProductVersionLS >> 16;
				v3 = pInfo->dwProductVersionLS & 0xffff;
			}
		}
	}


	//-----------------------------------------------------------------------
	wchar_t app_version[256];


	swprintf_s(app_version, L"버젼 %d.%d.%d.%d",
		v0,
		v1,
		v2,
		v3
	);

	SetDlgItemTextW(*this, IDC_ABOUTBOX_APP_VERSION, app_version);


	//-----------------------------------------------------------------------
	wchar_t app_name[256];


	LoadStringW(
		cx::wui::getAppModule()->getInstanceHandle(),
		IDS_APP_TITLE,
		app_name,
		sizeof(app_name) / sizeof(wchar_t)
	);
	SetDlgItemTextW(*this, IDC_ABOUTBOX_APP_NAME, app_name);


	//-----------------------------------------------------------------------
	wchar_t app_note[] =
	{
	L"경고:"                                                                L"\r\n"
	L"이 컴퓨터 프로그램은 저작권법과 국제 협약의 보호를 받습니다."         L"\r\n"
	L"이 프로그램의 전부 또는 일부를 무단으로 복제, 배포하는 행위는"        L"\r\n"
	L"민사 및 형사법에 의해 엄격히 규제되어 있으며, 기소 사유가 됩니다."    L"\r\n"
	};

	SetDlgItemTextW(*this, IDC_ABOUTBOX_APP_NOTE, app_note);


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void AboutBox::onCommand(cx::wui::WindowMessage& windowMessage)
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
