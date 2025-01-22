#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class AboutBox : 
	public wui::MessageMapWindowT<AboutBox, wui::ModalIndirectBaseDialog>
{
private:
	wui::MemoryDialogTemplate _DialogTemplate;

public:
	AboutBox();

public:
	void initializeDialogTemplate(void);

public:
	void registerWindowMessageMap(void);
	void onInitDialog(wui::WindowMessage& windowMessage);
	void onCommand(wui::WindowMessage& windowMessage);
};
