#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class AboutBox : public cx::wui::MessageMapWindowT<AboutBox, cx::wui::ModalTemplateBaseDialog>
{
public:
	AboutBox()
	{
		setTemplateName(IDD_ABOUTBOX);

		registerWindowMessageMap();
	}

#if 0
	//-----------------------------------------------------------------------
	// cx::wui::Window
public:
	virtual LRESULT onMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override
	{
		cx::wui::WindowMessage windowMessage{ hwnd, umsg, wparam, lparam };


		switch (umsg)
		{
		case WM_INITDIALOG:
			onInitDialog(windowMessage);
			break;

		case WM_COMMAND:
			onCommand(windowMessage);
			break;

		default:
			break;
		}

		return windowMessage.lResult;
	}
#endif

	//-----------------------------------------------------------------------
	// AboutBox
public:
	void registerWindowMessageMap(void)
	{
		_WindowMessageMap.handle(WM_INITDIALOG) = &AboutBox::onInitDialog;
		_WindowMessageMap.handle(WM_DESTROY)    = &AboutBox::onDestroy;
		_WindowMessageMap.handle(WM_CLOSE)      = &AboutBox::onClose;
		_WindowMessageMap.handle(WM_COMMAND)    = &AboutBox::onCommand;
	}

	void onInitDialog(cx::wui::WindowMessage& windowMessage)
	{
		OutputDebugStringW(L"AboutBox::onInitDialog()\r\n");

		windowMessage.setResult(TRUE);
	}

	void onDestroy(cx::wui::WindowMessage& windowMessage)
	{
		OutputDebugStringW(L"AboutBox::onDestroy()\r\n");

		windowMessage.setResult(TRUE);
	}

	void onClose(cx::wui::WindowMessage& windowMessage)
	{
		OutputDebugStringW(L"AboutBox::onClose()\r\n");

		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
	}

	void onCommand(cx::wui::WindowMessage& windowMessage)
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
};
