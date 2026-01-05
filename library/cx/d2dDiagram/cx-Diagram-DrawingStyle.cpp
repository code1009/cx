/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void DrawingStyle::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"DrawingStyle::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			DrawingStyle_PropertyChangedEvent,
			nullptr
		);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	FillStyle::FillStyle(const FillStyle& other) :
		//DrawingStyle(other),
		_FillColor(other._FillColor)
	{
	}

	FillStyle& FillStyle::operator=(const FillStyle& other)
	{
		if (this != &other)
		{
			//DrawingStyle::operator=(other);
			_FillColor = other._FillColor;
		}
		return *this;
	}

	FillStyle::FillStyle(FillStyle&& other) noexcept :
		//DrawingStyle(std::move(other))
		_FillColor(std::move(other._FillColor))
	{
	}

	FillStyle& FillStyle::operator=(FillStyle&& other) noexcept
	{
		if (this != &other)
		{
			//DrawingStyle::operator=(std::move(other));
			_FillColor = std::move(other._FillColor);
		}
		return *this;
	}

	//=======================================================================
	void FillStyle::copyTo(FillStyle& dest) const
	{
		//DrawingStyle::copyTo(dest);

		bool changed = false;
		if (dest._FillColor != _FillColor)
		{
			changed = true;
			dest._FillColor = _FillColor;
		}
		if (changed)
		{
			dest.notifyPropertyChanged();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	LineStyle::LineStyle(const LineStyle& other) :
		//DrawingStyle(other),
		_LineColor(other._LineColor), 
		_LineSize(other._LineSize)
	{
	}

	LineStyle& LineStyle::operator=(const LineStyle& other)
	{
		if (this != &other)
		{
			//DrawingStyle::operator=(other);
			_LineColor = other._LineColor;
			_LineSize = other._LineSize;
		}
		return *this;
	}

	LineStyle::LineStyle(LineStyle&& other) noexcept :
		//DrawingStyle(std::move(other))
		_LineColor(std::move(other._LineColor)),
		_LineSize(std::move(other._LineSize))
	{
	}

	LineStyle& LineStyle::operator=(LineStyle&& other) noexcept
	{
		if (this != &other)
		{
			//DrawingStyle::operator=(std::move(other));
			_LineColor = std::move(other._LineColor);
			_LineSize = std::move(other._LineSize);
		}
		return *this;
	}

	//=======================================================================
	void LineStyle::copyTo(LineStyle& dest) const
	{
		//DrawingStyle::copyTo(dest);

		bool changed = false;
		if (dest._LineColor != _LineColor)
		{
			changed = true;
			dest._LineColor = _LineColor;
		}
		if (dest._LineSize != _LineSize)
		{
			changed = true;
			dest._LineSize = _LineSize;
		}
		if (changed)
		{
			dest.notifyPropertyChanged();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	TextStyle::TextStyle(const TextStyle& other) :
		//DrawingStyle(other),
		_TextColor(other._TextColor),
		_FontFamily(other._FontFamily),
		_FontSize(other._FontSize),
		_FontBold(other._FontBold),
		_FontItalic(other._FontItalic),
		_TextHAlignment(other._TextHAlignment),
		_TextVAlignment(other._TextVAlignment)
		//_TextFormat(other._TextFormat)
	{
		updateTextFormat();
	}

	TextStyle& TextStyle::operator=(const TextStyle& other)
	{
		if (this != &other)
		{
			//DrawingStyle::operator=(other);
			_TextColor = other._TextColor;
			_FontFamily = other._FontFamily;
			_FontSize = other._FontSize;
			_FontBold = other._FontBold;
			_FontItalic = other._FontItalic;
			_TextHAlignment = other._TextHAlignment;
			_TextVAlignment = other._TextVAlignment;
			//_TextFormat = other._TextFormat;
			updateTextFormat();
		}
		return *this;
	}

	TextStyle::TextStyle(TextStyle&& other) noexcept :
		//DrawingStyle(std::move(other))
		_TextColor(std::move(other._TextColor)),
		_FontFamily(std::move(other._FontFamily)),
		_FontSize(std::move(other._FontSize)),
		_FontBold(std::move(other._FontBold)),
		_FontItalic(std::move(other._FontItalic)),
		_TextHAlignment(std::move(other._TextHAlignment)),
		_TextVAlignment(std::move(other._TextVAlignment))
		//_TextFormat(std::move(other._TextFormat))
	{
		updateTextFormat();
	}

	TextStyle& TextStyle::operator=(TextStyle&& other) noexcept
	{
		if (this != &other)
		{
			//DrawingStyle::operator=(std::move(other));
			_TextColor = std::move(other._TextColor);
			_FontFamily = std::move(other._FontFamily);
			_FontSize = std::move(other._FontSize);
			_FontBold = std::move(other._FontBold);
			_FontItalic = std::move(other._FontItalic);
			_TextHAlignment = std::move(other._TextHAlignment);
			_TextVAlignment = std::move(other._TextVAlignment);
			//_TextFormat = std::move(other._TextFormat);
			updateTextFormat();
		}
		return *this;
	}

	//=======================================================================
	void TextStyle::updateTextFormat(void)
	{
#if 0
		_TextFormat.FontFamily(_FontFamily.c_str());
		_TextFormat.FontSize(_FontSize);
		_TextFormat.FontWeight(
			_FontBold ? winrt::Microsoft::UI::Text::FontWeights::Bold() : winrt::Microsoft::UI::Text::FontWeights::Normal()
		);
		_TextFormat.FontStyle(
			_FontItalic ? winrt::Windows::UI::Text::FontStyle::Italic : winrt::Windows::UI::Text::FontStyle::Normal
		);


		_TextFormat.HorizontalAlignment(
			_TextHAlignment
		);
		_TextFormat.VerticalAlignment(
			_TextVAlignment
		);


		_TextFormat.WordWrapping(
			winrt::Microsoft::Graphics::Canvas::Text::CanvasWordWrapping::Wrap
		);
#endif
		// TODO
	}

	//=======================================================================
	void TextStyle::copyTo(TextStyle& dest) const
	{
		//DrawingStyle::copyTo(dest);

		bool changed = false;
		if (dest._TextColor != _TextColor)
		{
			changed = true;
			dest._TextColor = _TextColor;
		}
		if (dest._FontFamily != _FontFamily)
		{
			changed = true;
			dest._FontFamily = _FontFamily;
		}
		if (dest._FontSize != _FontSize)
		{
			changed = true;
			dest._FontSize = _FontSize;
		}
		if (dest._FontBold != _FontBold)
		{
			changed = true;
			dest._FontBold = _FontBold;
		}
		if (dest._FontItalic != _FontItalic)
		{
			changed = true;
			dest._FontItalic = _FontItalic;
		}
		if (dest._TextHAlignment != _TextHAlignment)
		{
			changed = true;
			dest._TextHAlignment = _TextHAlignment;
		}
		if (dest._TextVAlignment != _TextVAlignment)
		{
			changed = true;
			dest._TextVAlignment = _TextVAlignment;
		}
		/*
		if (dest._TextFormat != _TextFormat)
		{
			changed = true;
			dest._TextFormat = _TextFormat;
		}
		*/
		if (changed)
		{
			dest.updateTextFormat();
			dest.notifyPropertyChanged();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void copyLineStyle(
		LineStyle const& source,
		LineStyle& target
	)
	{
		source.copyTo(target);
	}

	void copyFillStyle(
		FillStyle const& source,
		FillStyle& target
	)
	{
		source.copyTo(target);
	}

	void copyTextStyle(
		TextStyle const& source,
		TextStyle& target
	)
	{
		source.copyTo(target);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::wstring to_std_wstring(FillStyle const& v)
	{
		std::wstring s;
		s = L"{";
		s += to_std_wstring(v.fillColor());
		s += L"}";
		return s;
	}

	FillStyle to_FillStyle(std::wstring const& v)
	{
		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		if (tokens.size() == 1)
		{
			FillStyle value(to_Color(tokens[0]));
			return value;
		}
		FillStyle defaultValue;
		return defaultValue;
	}

	//=======================================================================
	std::wstring to_std_wstring(LineStyle const& v)
	{
		std::wstring s;
		s = L"{";
		s += to_std_wstring(v.lineColor());
		s += L",";
		s += cx::to_std_wstring(v.lineSize());
		s += L"}";
		return s;
	}

	LineStyle to_LineStyle(std::wstring const& v)
	{
		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		if (tokens.size() == 2)
		{
			LineStyle value(to_Color(tokens[0]), cx::to_float(tokens[1]));
			return value;
		}
		LineStyle defaultValue;
		return defaultValue;
	}

	//=======================================================================
	std::wstring to_std_wstring(TextHAlignment const& v)
	{
		switch (v)
		{
		case TextHAlignment::Left: return L"Left";
		case TextHAlignment::Center: return L"Center";
		case TextHAlignment::Right: return L"Right";
		default:
			break;
		}
		return L"Left";
	}

	TextHAlignment to_TextHAlignment(std::wstring const& v)
	{
		if (v == L"Left") return TextHAlignment::Left;
		if (v == L"Center") return TextHAlignment::Center;
		if (v == L"Right") return TextHAlignment::Right;
		return TextHAlignment::Left;
	}

	std::wstring to_std_wstring(TextVAlignment const& v)
	{
		switch (v)
		{
		case TextVAlignment::Top: return L"Top";
		case TextVAlignment::Center: return L"Center";
		case TextVAlignment::Bottom: return L"Bottom";
		default:
			break;
		}
		return L"Center";
	}

	TextVAlignment to_TextVAlignment(std::wstring const& v)
	{
		if (v == L"Top") return TextVAlignment::Top;
		if (v == L"Center") return TextVAlignment::Center;
		if (v == L"Bottom") return TextVAlignment::Bottom;
		return TextVAlignment::Center;
	}

	std::wstring to_std_wstring(TextStyle const& v)
	{
		std::wstring s;
		s = L"{";
		s += to_std_wstring(v.textColor());
		s += L",";
		s += v.fontFamily();
		s += L",";
		s += cx::to_std_wstring(v.fontSize());
		s += L",";
		s += (v.fontBold() ? L"true" : L"false");
		s += L",";
		s += (v.fontItalic() ? L"true" : L"false");
		s += L",";
		s += to_std_wstring(v.textHAlignment());
		s += L",";
		s += to_std_wstring(v.textVAlignment());
		s += L"}";
		return s;
	}

	TextStyle to_TextStyle(std::wstring const& v)
	{
		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		if (tokens.size() == 7)
		{
			TextStyle value(
				to_Color(tokens[0]),
				tokens[1],
				cx::to_float(tokens[2]),
				cx::to_bool(tokens[3]),
				cx::to_bool(tokens[4]),
				to_TextHAlignment(tokens[5]),
				to_TextVAlignment(tokens[6])
			);
			return value;
		}
		TextStyle defaultValue;
		return defaultValue;
	}
}

