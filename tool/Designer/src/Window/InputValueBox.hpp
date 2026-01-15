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
	virtual void setup(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy);
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
class InputTextBox : 
	public cx::wui::MessageMapWindowT<InputTextBox, cx::wui::ModalIndirectBaseDialog>
{
protected:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

protected:
	std::uint32_t _X{ 0 };
	std::uint32_t _Y{ 0 };
	std::uint32_t _CX{ 0 };
	std::uint32_t _CY{ 0 };

protected:
	std::unique_ptr<cx::wui::Font> _Font;
	HWND _EditControlHandle{ nullptr };

public:
	enum class TextType
	{
		String,
		Float,
		Double,
		UInt8,
		UInt16,
		UInt32,
		UInt64,
		Int8,
		Int16,
		Int32,
		Int64
	};

	TextType _TextType{ TextType::String };

protected:
	std::wstring _Text;
	bool _ReadOnly{ false };

public:
	InputTextBox();

public:
	virtual void setup(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy, bool readOnly, TextType textType);

public:
	virtual void initializeDialogTemplate(void);

public:
	virtual void registerWindowMessageMap(void);
	virtual void onInitDialog(cx::wui::WindowMessage& windowMessage);
	virtual void onCommand(cx::wui::WindowMessage& windowMessage);
	virtual void onNcActivate(cx::wui::WindowMessage& windowMessage);
	virtual void onUser1(cx::wui::WindowMessage& windowMessage);

public:
	virtual void setText(std::wstring const& text);
	virtual std::wstring getText(void);

public:
	virtual void onEndDialog(INT_PTR result);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class InputTextListBox :
	public cx::wui::MessageMapWindowT<InputTextListBox, cx::wui::ModalIndirectBaseDialog>
{
protected:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

protected:
	std::uint32_t _X{ 0 };
	std::uint32_t _Y{ 0 };
	std::uint32_t _CX{ 0 };
	std::uint32_t _CY{ 0 };

protected:
	std::unique_ptr<cx::wui::Font> _Font;
	HWND _ListBoxControlHandle{ nullptr };

public:
	enum class TextType
	{
		String,
		Float,
		Double,
		UInt8,
		UInt16,
		UInt32,
		UInt64,
		Int8,
		Int16,
		Int32,
		Int64
	};

	TextType _TextType{ TextType::String };

protected:
	std::vector<std::wstring> _TextList;
	std::wstring _Text;
	bool _ReadOnly{ false };

public:
	InputTextListBox();

public:
	virtual void setup(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy, bool readOnly, TextType textType);

public:
	virtual void initializeDialogTemplate(void);

public:
	virtual void registerWindowMessageMap(void);
	virtual void onInitDialog(cx::wui::WindowMessage& windowMessage);
	virtual void onCommand(cx::wui::WindowMessage& windowMessage);
	virtual void onNcActivate(cx::wui::WindowMessage& windowMessage);
	virtual void onUser1(cx::wui::WindowMessage& windowMessage);

public:
	virtual void setTextList(std::vector<std::wstring> const& textList);
	virtual void setText(std::wstring const& text);
	virtual std::wstring getText(void);

public:
	virtual void onEndDialog(INT_PTR result);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool showInputTextBox(
	HWND hwnd, 
	std::uint32_t x, std::uint32_t y, 
	std::uint32_t cx, std::uint32_t cy, 
	bool readOnly,
	InputTextBox::TextType textType, 
	std::wstring& text
);

bool showInputTextListBox(
	HWND hwnd,
	std::uint32_t x, std::uint32_t y,
	std::uint32_t cx, std::uint32_t cy,
	bool readOnly,
	InputTextListBox::TextType textType,
	std::vector<std::wstring> textList,
	std::wstring& text
);




