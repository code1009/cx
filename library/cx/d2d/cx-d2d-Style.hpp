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
namespace Stroke
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class CapStyle : int32_t
{
    Flat = 0,
    Square = 1,
    Round = 2,
    Triangle = 3,
};

enum class DashStyle : int32_t
{
    Solid = 0,
    Dash = 1,
    Dot = 2,
    DashDot = 3,
    DashDotDot = 4,
};

enum class LineJoin : int32_t
{
    Miter = 0,
    Bevel = 1,
    Round = 2,
    MiterOrBevel = 3,
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class StrokeStyle : public Resource
{
public:
    wil::com_ptr_nothrow<ID2D1StrokeStyle> _Value;
    D2D1_STROKE_STYLE_PROPERTIES _Properties;
    
public:
    Stroke::CapStyle  _StartCap  { Stroke::CapStyle::Flat  };
    Stroke::CapStyle  _EndCap    { Stroke::CapStyle::Flat };
    Stroke::CapStyle  _DashCap   { Stroke::CapStyle::Flat };
    Stroke::LineJoin  _LineJoin  { Stroke::LineJoin::Miter };
	float             _MiterLimit{ 10.0f };
    Stroke::DashStyle _DashStyle { Stroke::DashStyle::Solid};
	float             _DashOffset{ 0.0f };

public:
    StrokeStyle() = default;

public:
    StrokeStyle(const StrokeStyle&) = delete;
    StrokeStyle& operator=(const StrokeStyle&) = delete;

    StrokeStyle(StrokeStyle&&) = delete;
    StrokeStyle& operator=(StrokeStyle&&) = delete;

public:
    virtual bool createResources(Context* context) override;
    virtual void releaseResources(void) override;

public:
    void StartCap(Stroke::CapStyle value);
    void EndCap(Stroke::CapStyle value);
    void DashCap(Stroke::CapStyle value);
    void LineJoin(Stroke::LineJoin value);
    void MiterLimit(float value);
    void DashStyle(Stroke::DashStyle value);
    void DashOffset(float value);
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
class TextFormat : public Resource
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
    TextFormat() = default;

public:
    TextFormat(const TextFormat&) = delete;
    TextFormat& operator=(const TextFormat&) = delete;

    TextFormat(TextFormat&&) = delete;
    TextFormat& operator=(TextFormat&&) = delete;

public:
    virtual bool createResources(Context* context) override;
    virtual void releaseResources(void) override;

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
