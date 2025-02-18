/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDrawingStyle
{
public:
	class StyleChangedParam
	{
	public:
		WidgetDrawingStyle* _Sender;
		std::uint32_t       _Code;
		void*               _Data;
	};

	using StyleChangedHandler = std::function<void(StyleChangedParam*)>;

private:
	StyleChangedHandler _StyleChangedHandler{};

public:
	WidgetDrawingStyle() = default;

public:
	virtual ~WidgetDrawingStyle() = default;

public:
	virtual void setStyleChangedHandler(StyleChangedHandler handler);
	virtual void setStyleChanged(std::uint32_t code = 0, void* data = nullptr);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetFillStyle : public WidgetDrawingStyle
{
private:
	Color _Color { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	WidgetFillStyle() = default;

public:
	explicit WidgetFillStyle(Color color);

public:
	virtual ~WidgetFillStyle() = default;

public:
	Color getColor(void) const;
	void setColor(const Color& color);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetLineStyle : public WidgetDrawingStyle
{
private:
	Color         _Color{ };
	float         _Width{ 1.0f };
	std::uint64_t _Attr { 0 };
	std::uint64_t _Dash { 0 };

public:
	WidgetLineStyle() = default;

public:
	explicit WidgetLineStyle(
		Color color              , 
		float width        = 1.0f, 
		std::uint64_t attr = 0, 
		std::uint64_t dash = 0
	);

public:
	virtual ~WidgetLineStyle() = default;

public:
	Color getColor() const;
	void setColor(const Color& color);

	float getWidth() const;
	void setWidth(float width);

	std::uint64_t getAttr() const;
	void setAttr(std::uint64_t attr);

	std::uint64_t getDash() const;
	void setDash(std::uint64_t dash);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetTextStyle : public WidgetDrawingStyle
{
private:
	Color         _Color   { };
	float         _FontSize{ 12.0f };
	std::wstring  _FontFace{ L"굴림" };
	std::uint64_t _FontAttr{ 0 };
	std::uint64_t _Align   { 0 };

public:
	WidgetTextStyle() = default;

public:
	WidgetTextStyle(
		Color         color,
		float         fontSize = 12.0f,
		std::wstring  fontFace = L"굴림",
		std::uint64_t fontAttr = 0,
		std::uint64_t align	   = 0
	);

public:
	virtual ~WidgetTextStyle() = default;

public:
	Color getColor(void) const;
	void setColor(const Color& color);

	float getFontSize(void) const;
	void setFontSize(float fontSize);

	std::wstring getFontFace(void) const;
	void setFontFace(const std::wstring& fontFace);

	std::uint64_t getFontAttr(void) const;
	void setFontAttr(std::uint64_t fontAttr);

	std::uint64_t getAlign(void) const;
	void setAlign(std::uint64_t align);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring to_std_wstring(const WidgetFillStyle& o);
std::wstring to_std_wstring(const WidgetLineStyle& o);
std::wstring to_std_wstring(const WidgetTextStyle& o);

WidgetFillStyle to_WidgetFillStyle(std::wstring s);
WidgetLineStyle to_WidgetLineStyle(std::wstring s);
WidgetTextStyle to_WidgetTextStyle(std::wstring s);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




