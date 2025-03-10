#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainBox : 
	public cx::wui::MessageMapWindowT<MainBox, cx::wui::ModalTemplateBaseDialog>
{
private:

private:
	std::unique_ptr<VcFileGenerator> _VcFileGenerator;

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
};
