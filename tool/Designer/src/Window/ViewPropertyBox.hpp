#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ViewPropertyBox : 
	public cx::wui::MessageMapWindowT<ViewPropertyBox, cx::wui::ModalIndirectBaseDialog>
{
private:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

public:
	std::uint32_t _CX{ 1920 };
	std::uint32_t _CY{ 1080 };
	std::uint8_t _ColorR8{ 255 };
	std::uint8_t _ColorG8{ 255 };
	std::uint8_t _ColorB8{ 255 };

public:
	ViewPropertyBox();

public:
	void initializeDialogTemplate(void);

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);

public:
	void pickColor(void);
	bool updateData(void);
};
