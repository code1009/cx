/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Color::Color(std::uint32_t rgb, float a)
{
	static const std::uint32_t sc_redShift = 16;
	static const std::uint32_t sc_greenShift = 8;
	static const std::uint32_t sc_blueShift = 0;

	static const std::uint32_t sc_redMask = 0xff << sc_redShift;
	static const std::uint32_t sc_greenMask = 0xff << sc_greenShift;
	static const std::uint32_t sc_blueMask = 0xff << sc_blueShift;

	_Value.r = static_cast<float>((rgb & sc_redMask) >> sc_redShift) / 255.f;
	_Value.g = static_cast<float>((rgb & sc_greenMask) >> sc_greenShift) / 255.f;
	_Value.b = static_cast<float>((rgb & sc_blueMask) >> sc_blueShift) / 255.f;
	_Value.a = a;
}

Color::Color(D2D1::ColorF::Enum knownColor,	float a) :
	Color(static_cast<std::uint32_t>(knownColor), a)
{
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Color Colors::AliceBlue           (void) { return Color(D2D1::ColorF::AliceBlue             ); }
Color Colors::AntiqueWhite        (void) { return Color(D2D1::ColorF::AntiqueWhite          ); }
Color Colors::Aqua                (void) { return Color(D2D1::ColorF::Aqua                  ); }
Color Colors::Aquamarine          (void) { return Color(D2D1::ColorF::Aquamarine            ); }
Color Colors::Azure               (void) { return Color(D2D1::ColorF::Azure                 ); }
Color Colors::Beige               (void) { return Color(D2D1::ColorF::Beige                 ); }
Color Colors::Bisque              (void) { return Color(D2D1::ColorF::Bisque                ); }
Color Colors::Black               (void) { return Color(D2D1::ColorF::Black                 ); }
Color Colors::BlanchedAlmond      (void) { return Color(D2D1::ColorF::BlanchedAlmond        ); }
Color Colors::Blue                (void) { return Color(D2D1::ColorF::Blue                  ); }
Color Colors::BlueViolet          (void) { return Color(D2D1::ColorF::BlueViolet            ); }
Color Colors::Brown               (void) { return Color(D2D1::ColorF::Brown                 ); }
Color Colors::BurlyWood           (void) { return Color(D2D1::ColorF::BurlyWood             ); }
Color Colors::CadetBlue           (void) { return Color(D2D1::ColorF::CadetBlue             ); }
Color Colors::Chartreuse          (void) { return Color(D2D1::ColorF::Chartreuse            ); }
Color Colors::Chocolate           (void) { return Color(D2D1::ColorF::Chocolate             ); }
Color Colors::Coral               (void) { return Color(D2D1::ColorF::Coral                 ); }
Color Colors::CornflowerBlue      (void) { return Color(D2D1::ColorF::CornflowerBlue        ); }
Color Colors::Cornsilk            (void) { return Color(D2D1::ColorF::Cornsilk              ); }
Color Colors::Crimson             (void) { return Color(D2D1::ColorF::Crimson               ); }
Color Colors::Cyan                (void) { return Color(D2D1::ColorF::Cyan                  ); }
Color Colors::DarkBlue            (void) { return Color(D2D1::ColorF::DarkBlue              ); }
Color Colors::DarkCyan            (void) { return Color(D2D1::ColorF::DarkCyan              ); }
Color Colors::DarkGoldenrod       (void) { return Color(D2D1::ColorF::DarkGoldenrod         ); }
Color Colors::DarkGray            (void) { return Color(D2D1::ColorF::DarkGray              ); }
Color Colors::DarkGreen           (void) { return Color(D2D1::ColorF::DarkGreen             ); }
Color Colors::DarkKhaki           (void) { return Color(D2D1::ColorF::DarkKhaki             ); }
Color Colors::DarkMagenta         (void) { return Color(D2D1::ColorF::DarkMagenta           ); }
Color Colors::DarkOliveGreen      (void) { return Color(D2D1::ColorF::DarkOliveGreen        ); }
Color Colors::DarkOrange          (void) { return Color(D2D1::ColorF::DarkOrange            ); }
Color Colors::DarkOrchid          (void) { return Color(D2D1::ColorF::DarkOrchid            ); }
Color Colors::DarkRed             (void) { return Color(D2D1::ColorF::DarkRed               ); }
Color Colors::DarkSalmon          (void) { return Color(D2D1::ColorF::DarkSalmon            ); }
Color Colors::DarkSeaGreen        (void) { return Color(D2D1::ColorF::DarkSeaGreen          ); }
Color Colors::DarkSlateBlue       (void) { return Color(D2D1::ColorF::DarkSlateBlue         ); }
Color Colors::DarkSlateGray       (void) { return Color(D2D1::ColorF::DarkSlateGray         ); }
Color Colors::DarkTurquoise       (void) { return Color(D2D1::ColorF::DarkTurquoise         ); }
Color Colors::DarkViolet          (void) { return Color(D2D1::ColorF::DarkViolet            ); }
Color Colors::DeepPink            (void) { return Color(D2D1::ColorF::DeepPink              ); }
Color Colors::DeepSkyBlue         (void) { return Color(D2D1::ColorF::DeepSkyBlue           ); }
Color Colors::DimGray             (void) { return Color(D2D1::ColorF::DimGray               ); }
Color Colors::DodgerBlue          (void) { return Color(D2D1::ColorF::DodgerBlue            ); }
Color Colors::Firebrick           (void) { return Color(D2D1::ColorF::Firebrick             ); }
Color Colors::FloralWhite         (void) { return Color(D2D1::ColorF::FloralWhite           ); }
Color Colors::ForestGreen         (void) { return Color(D2D1::ColorF::ForestGreen           ); }
Color Colors::Fuchsia             (void) { return Color(D2D1::ColorF::Fuchsia               ); }
Color Colors::Gainsboro           (void) { return Color(D2D1::ColorF::Gainsboro             ); }
Color Colors::GhostWhite          (void) { return Color(D2D1::ColorF::GhostWhite            ); }
Color Colors::Gold                (void) { return Color(D2D1::ColorF::Gold                  ); }
Color Colors::Goldenrod           (void) { return Color(D2D1::ColorF::Goldenrod             ); }
Color Colors::Gray                (void) { return Color(D2D1::ColorF::Gray                  ); }
Color Colors::Green               (void) { return Color(D2D1::ColorF::Green                 ); }
Color Colors::GreenYellow         (void) { return Color(D2D1::ColorF::GreenYellow           ); }
Color Colors::Honeydew            (void) { return Color(D2D1::ColorF::Honeydew              ); }
Color Colors::HotPink             (void) { return Color(D2D1::ColorF::HotPink               ); }
Color Colors::IndianRed           (void) { return Color(D2D1::ColorF::IndianRed             ); }
Color Colors::Indigo              (void) { return Color(D2D1::ColorF::Indigo                ); }
Color Colors::Ivory               (void) { return Color(D2D1::ColorF::Ivory                 ); }
Color Colors::Khaki               (void) { return Color(D2D1::ColorF::Khaki                 ); }
Color Colors::Lavender            (void) { return Color(D2D1::ColorF::Lavender              ); }
Color Colors::LavenderBlush       (void) { return Color(D2D1::ColorF::LavenderBlush         ); }
Color Colors::LawnGreen           (void) { return Color(D2D1::ColorF::LawnGreen             ); }
Color Colors::LemonChiffon        (void) { return Color(D2D1::ColorF::LemonChiffon          ); }
Color Colors::LightBlue           (void) { return Color(D2D1::ColorF::LightBlue             ); }
Color Colors::LightCoral          (void) { return Color(D2D1::ColorF::LightCoral            ); }
Color Colors::LightCyan           (void) { return Color(D2D1::ColorF::LightCyan             ); }
Color Colors::LightGoldenrodYellow(void) { return Color(D2D1::ColorF::LightGoldenrodYellow  ); }
Color Colors::LightGreen          (void) { return Color(D2D1::ColorF::LightGreen            ); }
Color Colors::LightGray           (void) { return Color(D2D1::ColorF::LightGray             ); }
Color Colors::LightPink           (void) { return Color(D2D1::ColorF::LightPink             ); }
Color Colors::LightSalmon         (void) { return Color(D2D1::ColorF::LightSalmon           ); }
Color Colors::LightSeaGreen       (void) { return Color(D2D1::ColorF::LightSeaGreen         ); }
Color Colors::LightSkyBlue        (void) { return Color(D2D1::ColorF::LightSkyBlue          ); }
Color Colors::LightSlateGray      (void) { return Color(D2D1::ColorF::LightSlateGray        ); }
Color Colors::LightSteelBlue      (void) { return Color(D2D1::ColorF::LightSteelBlue        ); }
Color Colors::LightYellow         (void) { return Color(D2D1::ColorF::LightYellow           ); }
Color Colors::Lime                (void) { return Color(D2D1::ColorF::Lime                  ); }
Color Colors::LimeGreen           (void) { return Color(D2D1::ColorF::LimeGreen             ); }
Color Colors::Linen               (void) { return Color(D2D1::ColorF::Linen                 ); }
Color Colors::Magenta             (void) { return Color(D2D1::ColorF::Magenta               ); }
Color Colors::Maroon              (void) { return Color(D2D1::ColorF::Maroon                ); }
Color Colors::MediumAquamarine    (void) { return Color(D2D1::ColorF::MediumAquamarine      ); }
Color Colors::MediumBlue          (void) { return Color(D2D1::ColorF::MediumBlue            ); }
Color Colors::MediumOrchid        (void) { return Color(D2D1::ColorF::MediumOrchid          ); }
Color Colors::MediumPurple        (void) { return Color(D2D1::ColorF::MediumPurple          ); }
Color Colors::MediumSeaGreen      (void) { return Color(D2D1::ColorF::MediumSeaGreen        ); }
Color Colors::MediumSlateBlue     (void) { return Color(D2D1::ColorF::MediumSlateBlue       ); }
Color Colors::MediumSpringGreen   (void) { return Color(D2D1::ColorF::MediumSpringGreen     ); }
Color Colors::MediumTurquoise     (void) { return Color(D2D1::ColorF::MediumTurquoise       ); }
Color Colors::MediumVioletRed     (void) { return Color(D2D1::ColorF::MediumVioletRed       ); }
Color Colors::MidnightBlue        (void) { return Color(D2D1::ColorF::MidnightBlue          ); }
Color Colors::MintCream           (void) { return Color(D2D1::ColorF::MintCream             ); }
Color Colors::MistyRose           (void) { return Color(D2D1::ColorF::MistyRose             ); }
Color Colors::Moccasin            (void) { return Color(D2D1::ColorF::Moccasin              ); }
Color Colors::NavajoWhite         (void) { return Color(D2D1::ColorF::NavajoWhite           ); }
Color Colors::Navy                (void) { return Color(D2D1::ColorF::Navy                  ); }
Color Colors::OldLace             (void) { return Color(D2D1::ColorF::OldLace               ); }
Color Colors::Olive               (void) { return Color(D2D1::ColorF::Olive                 ); }
Color Colors::OliveDrab           (void) { return Color(D2D1::ColorF::OliveDrab             ); }
Color Colors::Orange              (void) { return Color(D2D1::ColorF::Orange                ); }
Color Colors::OrangeRed           (void) { return Color(D2D1::ColorF::OrangeRed             ); }
Color Colors::Orchid              (void) { return Color(D2D1::ColorF::Orchid                ); }
Color Colors::PaleGoldenrod       (void) { return Color(D2D1::ColorF::PaleGoldenrod         ); }
Color Colors::PaleGreen           (void) { return Color(D2D1::ColorF::PaleGreen             ); }
Color Colors::PaleTurquoise       (void) { return Color(D2D1::ColorF::PaleTurquoise         ); }
Color Colors::PaleVioletRed       (void) { return Color(D2D1::ColorF::PaleVioletRed         ); }
Color Colors::PapayaWhip          (void) { return Color(D2D1::ColorF::PapayaWhip            ); }
Color Colors::PeachPuff           (void) { return Color(D2D1::ColorF::PeachPuff             ); }
Color Colors::Peru                (void) { return Color(D2D1::ColorF::Peru                  ); }
Color Colors::Pink                (void) { return Color(D2D1::ColorF::Pink                  ); }
Color Colors::Plum                (void) { return Color(D2D1::ColorF::Plum                  ); }
Color Colors::PowderBlue          (void) { return Color(D2D1::ColorF::PowderBlue            ); }
Color Colors::Purple              (void) { return Color(D2D1::ColorF::Purple                ); }
Color Colors::Red                 (void) { return Color(D2D1::ColorF::Red                   ); }
Color Colors::RosyBrown           (void) { return Color(D2D1::ColorF::RosyBrown             ); }
Color Colors::RoyalBlue           (void) { return Color(D2D1::ColorF::RoyalBlue             ); }
Color Colors::SaddleBrown         (void) { return Color(D2D1::ColorF::SaddleBrown           ); }
Color Colors::Salmon              (void) { return Color(D2D1::ColorF::Salmon                ); }
Color Colors::SandyBrown          (void) { return Color(D2D1::ColorF::SandyBrown            ); }
Color Colors::SeaGreen            (void) { return Color(D2D1::ColorF::SeaGreen              ); }
Color Colors::SeaShell            (void) { return Color(D2D1::ColorF::SeaShell              ); }
Color Colors::Sienna              (void) { return Color(D2D1::ColorF::Sienna                ); }
Color Colors::Silver              (void) { return Color(D2D1::ColorF::Silver                ); }
Color Colors::SkyBlue             (void) { return Color(D2D1::ColorF::SkyBlue               ); }
Color Colors::SlateBlue           (void) { return Color(D2D1::ColorF::SlateBlue             ); }
Color Colors::SlateGray           (void) { return Color(D2D1::ColorF::SlateGray             ); }
Color Colors::Snow                (void) { return Color(D2D1::ColorF::Snow                  ); }
Color Colors::SpringGreen         (void) { return Color(D2D1::ColorF::SpringGreen           ); }
Color Colors::SteelBlue           (void) { return Color(D2D1::ColorF::SteelBlue             ); }
Color Colors::Tan                 (void) { return Color(D2D1::ColorF::Tan                   ); }
Color Colors::Teal                (void) { return Color(D2D1::ColorF::Teal                  ); }
Color Colors::Thistle             (void) { return Color(D2D1::ColorF::Thistle               ); }
Color Colors::Tomato              (void) { return Color(D2D1::ColorF::Tomato                ); }
Color Colors::Turquoise           (void) { return Color(D2D1::ColorF::Turquoise             ); }
Color Colors::Violet              (void) { return Color(D2D1::ColorF::Violet                ); }
Color Colors::Wheat               (void) { return Color(D2D1::ColorF::Wheat                 ); }
Color Colors::White               (void) { return Color(D2D1::ColorF::White                 ); }
Color Colors::WhiteSmoke          (void) { return Color(D2D1::ColorF::WhiteSmoke            ); }
Color Colors::Yellow              (void) { return Color(D2D1::ColorF::Yellow                ); }
Color Colors::YellowGreen         (void) { return Color(D2D1::ColorF::YellowGreen           ); }





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
