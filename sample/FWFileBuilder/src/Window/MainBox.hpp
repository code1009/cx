#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainBox : 
	public cx::wui::MessageMapWindowT<MainBox, cx::wui::ModalTemplateBaseDialog>
{
private:
	std::wstring _FileDialog_himage_DefExt     { L"himage" };
	std::wstring _FileDialog_himage_InitialDir { L"C:\\" };
	std::wstring _FileDialog_himage_Title      { L"" };

	std::wstring _FileDialog_bin_DefExt     { L"bin" };
	std::wstring _FileDialog_bin_InitialDir { L"C:\\" };
	std::wstring _FileDialog_bin_Title      { L"" };

private:
	std::unique_ptr<FWFileBuilder> _FWFileBuilder;

public:
	MainBox();

public:
	virtual ~MainBox();

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);	
	void onButton1(cx::wui::WindowMessage& windowMessage);
	void onButton2(cx::wui::WindowMessage& windowMessage);
	void onButton3(cx::wui::WindowMessage& windowMessage);
	void onButton4(cx::wui::WindowMessage& windowMessage);
};
