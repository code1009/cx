#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class PropertyValueBox : 
	public cx::wui::MessageMapWindowT<PropertyValueBox, cx::wui::ModalIndirectBaseDialog>
{
private:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

public:
	PropertyValueBox();

public:
	virtual LRESULT onWindowMessage(cx::wui::WindowMessage& windowMessage) override;

public:
	void initializeDialogTemplate(void);

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onNcActivate(cx::wui::WindowMessage& windowMessage);
	void onActivate(cx::wui::WindowMessage& windowMessage);	
	void onSetFocus(cx::wui::WindowMessage& windowMessage);
	void onKillFocus(cx::wui::WindowMessage& windowMessage);
	void onUser1(cx::wui::WindowMessage& windowMessage);
};
