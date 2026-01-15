#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class InputValueBox : 
	public cx::wui::MessageMapWindowT<InputValueBox, cx::wui::ModalIndirectBaseDialog>
{
protected:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

protected:
	std::uint32_t _X{ 0 };
	std::uint32_t _Y{ 0 };
	std::uint32_t _CX{ 0 }; 
	std::uint32_t _CY{ 0 };

public:
	InputValueBox();

public:
	virtual LRESULT onWindowMessage(cx::wui::WindowMessage& windowMessage) override;

public:
	virtual void setupInputBox(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy);
	virtual void setPosition(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy);
	virtual void setWindowPosition(void);

public:
	virtual void initializeDialogTemplate(void);

public:
	virtual void registerWindowMessageMap(void);
	virtual void onInitDialog(cx::wui::WindowMessage& windowMessage);
	virtual void onCommand(cx::wui::WindowMessage& windowMessage);
	virtual void onNcActivate(cx::wui::WindowMessage& windowMessage);
	virtual void onActivate(cx::wui::WindowMessage& windowMessage);	
	virtual void onSetFocus(cx::wui::WindowMessage& windowMessage);
	virtual void onKillFocus(cx::wui::WindowMessage& windowMessage);
	virtual void onUser1(cx::wui::WindowMessage& windowMessage);
	virtual void onPaint(cx::wui::WindowMessage& windowMessage);

public:
	virtual void onEndDialog(INT_PTR result);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class InputStringBox : public InputValueBox
{
private:
	std::unique_ptr<cx::wui::Font> _Font;
	std::wstring _String;
	HWND _EditControlHandle{ nullptr };

public:
	InputStringBox();

public:
	virtual void setupInputStringBox(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy);

public:
	virtual void initializeDialogTemplate(void) override;

public:
	void setString(std::wstring const& value);
	std::wstring getString(void);

public:
	virtual void onInitDialog(cx::wui::WindowMessage& windowMessage) override;

public:
	virtual void onEndDialog(INT_PTR result) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool showInputStringBox(HWND hwnd, std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy, std::wstring& value);
