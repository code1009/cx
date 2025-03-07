#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainBox : 
	public cx::wui::MessageMapWindowT<MainBox, cx::wui::ModalTemplateBaseDialog>
{
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
};
