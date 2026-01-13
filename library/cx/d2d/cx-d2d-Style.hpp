#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
// 
// Fill
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
// 
// Line
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CapStyle
{
public:
	D2D1_CAP_STYLE _Value;
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class StrokeStyle
{
public:
    D2D1_STROKE_STYLE_PROPERTIES _Value;
};





/////////////////////////////////////////////////////////////////////////////
// 
// Text
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class TextHAlignment : int32_t
{
    Left = 0,
    Right = 1,
    Center = 2,
    Justified = 3,
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class TextVAlignment : int32_t
{
    Top = 0,
    Bottom = 1,
    Center = 2,
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class TextFormat : public DeviceResource
{
public:
    wil::com_ptr_nothrow<IDWriteTextFormat> _Value;

public:
    std::wstring   _FontFamily{ L"Segoe UI" };
    float          _FontSize  { 12.0f };
    bool           _FontBold  { false };
    bool           _FontItalic{ false };
    TextHAlignment _TextHAlign{ TextHAlignment::Left };
    TextVAlignment _TextVAlign{ TextVAlignment::Center };

public:
    TextFormat()
    {
	}

public:
    TextFormat(const TextFormat&) = delete;
    TextFormat& operator=(const TextFormat&) = delete;

    TextFormat(TextFormat&&) = delete;
    TextFormat& operator=(TextFormat&&) = delete;

public:
    virtual void releaseDeviceResources(void) override;

public:
    void FontFamily(std::wstring fontFamily);
    void FontSize(float fontSize);
    void FontWeight(bool fontBold);
    void FontStyle(bool fontItalic);
    void HorizontalAlignment(TextHAlignment textHAlign);
    void VerticalAlignment(TextVAlignment textVAlign);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
