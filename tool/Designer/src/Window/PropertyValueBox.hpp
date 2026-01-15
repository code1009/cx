#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class PropertyValueBox : 
	public cx::wui::MessageMapWindowT<PropertyValueBox, cx::wui::ModalIndirectBaseDialog>
{
private:
	cx::wui::MemoryDialogTemplate _DialogTemplate;
	std::uint32_t _X; 
	std::uint32_t _Y; 
	std::uint32_t _CX; 
	std::uint32_t _CY;

public:
	explicit PropertyValueBox(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy);

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
