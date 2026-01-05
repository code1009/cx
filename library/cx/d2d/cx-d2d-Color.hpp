#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
struct Color
{
public:
	D2D1::ColorF _Value{ 0, 1.0f };
    /*
    int A{};
    int R{};
    int G{};
    int B{};
    */
public:
    constexpr Color() = default;

    constexpr Color(uint8_t vA, uint8_t vR, uint8_t vG, uint8_t vB) noexcept;

public:
    Color(D2D1::ColorF::Enum knownColor, float a = 1.0);
};

//===========================================================================
inline bool operator==(Color const& left, Color const& right) noexcept
{
    return 
        left._Value.a == right._Value.a && 
        left._Value.r == right._Value.r && 
        left._Value.g == right._Value.g && 
        left._Value.b == right._Value.b;
}

inline bool operator!=(Color const& left, Color const& right) noexcept
{
    return !(left == right);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Colors
{
public:
    static Color AliceBlue           (void);
    static Color AntiqueWhite        (void);
    static Color Aqua                (void);
    static Color Aquamarine          (void);
    static Color Azure               (void);
    static Color Beige               (void);
    static Color Bisque              (void);
    static Color Black               (void);
    static Color BlanchedAlmond      (void);
    static Color Blue                (void);
    static Color BlueViolet          (void);
    static Color Brown               (void);
    static Color BurlyWood           (void);
    static Color CadetBlue           (void);
    static Color Chartreuse          (void);
    static Color Chocolate           (void);
    static Color Coral               (void);
    static Color CornflowerBlue      (void);
    static Color Cornsilk            (void);
    static Color Crimson             (void);
    static Color Cyan                (void);
    static Color DarkBlue            (void);
    static Color DarkCyan            (void);
    static Color DarkGoldenrod       (void);
    static Color DarkGray            (void);
    static Color DarkGreen           (void);
    static Color DarkKhaki           (void);
    static Color DarkMagenta         (void);
    static Color DarkOliveGreen      (void);
    static Color DarkOrange          (void);
    static Color DarkOrchid          (void);
    static Color DarkRed             (void);
    static Color DarkSalmon          (void);
    static Color DarkSeaGreen        (void);
    static Color DarkSlateBlue       (void);
    static Color DarkSlateGray       (void);
    static Color DarkTurquoise       (void);
    static Color DarkViolet          (void);
    static Color DeepPink            (void);
    static Color DeepSkyBlue         (void);
    static Color DimGray             (void);
    static Color DodgerBlue          (void);
    static Color Firebrick           (void);
    static Color FloralWhite         (void);
    static Color ForestGreen         (void);
    static Color Fuchsia             (void);
    static Color Gainsboro           (void);
    static Color GhostWhite          (void);
    static Color Gold                (void);
    static Color Goldenrod           (void);
    static Color Gray                (void);
    static Color Green               (void);
    static Color GreenYellow         (void);
    static Color Honeydew            (void);
    static Color HotPink             (void);
    static Color IndianRed           (void);
    static Color Indigo              (void);
    static Color Ivory               (void);
    static Color Khaki               (void);
    static Color Lavender            (void);
    static Color LavenderBlush       (void);
    static Color LawnGreen           (void);
    static Color LemonChiffon        (void);
    static Color LightBlue           (void);
    static Color LightCoral          (void);
    static Color LightCyan           (void);
    static Color LightGoldenrodYellow(void);
    static Color LightGreen          (void);
    static Color LightGray           (void);
    static Color LightPink           (void);
    static Color LightSalmon         (void);
    static Color LightSeaGreen       (void);
    static Color LightSkyBlue        (void);
    static Color LightSlateGray      (void);
    static Color LightSteelBlue      (void);
    static Color LightYellow         (void);
    static Color Lime                (void);
    static Color LimeGreen           (void);
    static Color Linen               (void);
    static Color Magenta             (void);
    static Color Maroon              (void);
    static Color MediumAquamarine    (void);
    static Color MediumBlue          (void);
    static Color MediumOrchid        (void);
    static Color MediumPurple        (void);
    static Color MediumSeaGreen      (void);
    static Color MediumSlateBlue     (void);
    static Color MediumSpringGreen   (void);
    static Color MediumTurquoise     (void);
    static Color MediumVioletRed     (void);
    static Color MidnightBlue        (void);
    static Color MintCream           (void);
    static Color MistyRose           (void);
    static Color Moccasin            (void);
    static Color NavajoWhite         (void);
    static Color Navy                (void);
    static Color OldLace             (void);
    static Color Olive               (void);
    static Color OliveDrab           (void);
    static Color Orange              (void);
    static Color OrangeRed           (void);
    static Color Orchid              (void);
    static Color PaleGoldenrod       (void);
    static Color PaleGreen           (void);
    static Color PaleTurquoise       (void);
    static Color PaleVioletRed       (void);
    static Color PapayaWhip          (void);
    static Color PeachPuff           (void);
    static Color Peru                (void);
    static Color Pink                (void);
    static Color Plum                (void);
    static Color PowderBlue          (void);
    static Color Purple              (void);
    static Color Red                 (void);
    static Color RosyBrown           (void);
    static Color RoyalBlue           (void);
    static Color SaddleBrown         (void);
    static Color Salmon              (void);
    static Color SandyBrown          (void);
    static Color SeaGreen            (void);
    static Color SeaShell            (void);
    static Color Sienna              (void);
    static Color Silver              (void);
    static Color SkyBlue             (void);
    static Color SlateBlue           (void);
    static Color SlateGray           (void);
    static Color Snow                (void);
    static Color SpringGreen         (void);
    static Color SteelBlue           (void);
    static Color Tan                 (void);
    static Color Teal                (void);
    static Color Thistle             (void);
    static Color Tomato              (void);
    static Color Turquoise           (void);
    static Color Violet              (void);
    static Color Wheat               (void);
    static Color White               (void);
    static Color WhiteSmoke          (void);
    static Color Yellow              (void);
    static Color YellowGreen         (void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
