#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainBox : 
	public cx::wui::MessageMapWindowT<MainBox, cx::wui::ModalTemplateBaseDialog>
{
public:
	MainBox();

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
};
