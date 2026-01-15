#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ViewPropertyBox : 
	public cx::wui::MessageMapWindowT<ViewPropertyBox, cx::wui::ModalIndirectBaseDialog>
{
private:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

public:
	ViewPropertyBox();

public:
	void initializeDialogTemplate(void);

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
};
