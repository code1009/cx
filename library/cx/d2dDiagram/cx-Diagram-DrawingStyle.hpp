#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class DrawingDefaults
	{
	public:
		//-------------------------------------------------------------------
		static constexpr Color          fillColor{ 255, 255, 255, 255 };
		static constexpr Color          lineColor{ 255, 0, 0, 0 };

		//-------------------------------------------------------------------
		static constexpr Coord          lineSize = 1.0f;

		//-------------------------------------------------------------------
		static constexpr Color          textColor      { 255, 0, 0, 0 };
		//static constexpr StringView     fontFamily     = L"Segoe UI";
		static constexpr StringView     fontFamily     = L"맑은 고딕";
		static constexpr float          fontSize       = 12.0f;
		static constexpr bool           fontBold       = false;
		static constexpr bool           fontItalic     = false;
		static constexpr TextHAlignment textHAlignment = TextHAlignment::Left;
		static constexpr TextVAlignment textVAlignment = TextVAlignment::Center;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class DrawingStyle : public std::enable_shared_from_this<DrawingStyle>
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	public:
		DrawingStyle() = default;

	public:
		virtual ~DrawingStyle() = default;

	public:
		DrawingStyle(const DrawingStyle& other) = delete;
		DrawingStyle& operator=(const DrawingStyle& other) = delete;
		DrawingStyle(DrawingStyle&& other) noexcept = delete;
		DrawingStyle& operator=(DrawingStyle&& other) noexcept = delete;

	protected:
		void notifyPropertyChanged(void);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class FillStyle : public DrawingStyle
	{
	private:
		Color _FillColor{ DrawingDefaults::fillColor };

	public:
		FillStyle() = default;

	public:
		explicit FillStyle(Color const& fillColor) :
			DrawingStyle(),
			_FillColor(fillColor)
		{
		}

	public:
		FillStyle(const FillStyle& other);
		FillStyle& operator=(const FillStyle& other);
		FillStyle(FillStyle&& other) noexcept;
		FillStyle& operator=(FillStyle&& other) noexcept;

	public:
		Color fillColor() const { return _FillColor; }
		void fillColor(Color const& newValue)
		{
			if (_FillColor == newValue)
			{
				return;
			}
			_FillColor = newValue;
			notifyPropertyChanged();
		}

	public:
		void copyTo(FillStyle& dest) const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class LineStyle : public DrawingStyle
	{
	private:
		Color _LineColor{ DrawingDefaults::lineColor };
		Coord _LineSize{ DrawingDefaults::lineSize };
		//StrokeStyle _StrokeStyle;

	public:
		LineStyle() = default;

	public:
		explicit LineStyle(Color const& lineColor, Coord const lineSize) :
			DrawingStyle(),
			_LineColor(lineColor),
			_LineSize(lineSize)
		{
			//_StrokeStyle.StartCap(CapStyle::Round);
			//_StrokeStyle.EndCap(CapStyle::Round);
		}

	public:
		LineStyle(const LineStyle& other);
		LineStyle& operator=(const LineStyle& other);
		LineStyle(LineStyle&& other) noexcept;
		LineStyle& operator=(LineStyle&& other) noexcept;

	public:
		Color lineColor() const { return _LineColor; }
		Coord lineSize() const { return _LineSize; }

	public:
		void lineColor(Color const& newValue)
		{
			if (_LineColor == newValue)
			{
				return;
			}
			_LineColor = newValue;
			notifyPropertyChanged();
		}
		void lineSize(Coord const newValue)
		{
			if (_LineSize == newValue)
			{
				return;
			}
			_LineSize = newValue;
			notifyPropertyChanged();
		}

	public:
		//StrokeStyle const& strokeStyle() const { return _StrokeStyle; }
		//void strokeStyle(StrokeStyle const& newValue)
		//{
		//	if (_StrokeStyle == newValue)
		//	{
		//		return;
		//	}
		//	_StrokeStyle = newValue;
		//	notifyPropertyChanged();
		//}

	public:
		void copyTo(LineStyle& dest) const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class TextStyle : public DrawingStyle
	{
	private:
		Color          _TextColor     { DrawingDefaults::textColor      };
		String         _FontFamily    { DrawingDefaults::fontFamily     };
		float          _FontSize      { DrawingDefaults::fontSize       };
		bool           _FontBold      { DrawingDefaults::fontBold       };
		bool           _FontItalic    { DrawingDefaults::fontItalic     };
		TextHAlignment _TextHAlignment{ DrawingDefaults::textHAlignment };
		TextVAlignment _TextVAlignment{ DrawingDefaults::textVAlignment };
		TextFormat     _TextFormat{};

	public:
		TextStyle() :
			DrawingStyle()
		{
			updateTextFormat();
		}

	public:
		TextStyle(const TextStyle& other);
		TextStyle& operator=(const TextStyle& other);
		TextStyle(TextStyle&& other) noexcept;
		TextStyle& operator=(TextStyle&& other) noexcept;

	public:
		explicit TextStyle(
			Color const& textColor,
			StringView const& fontFamily,
			float const fontSize,
			bool const fontBold = false,
			bool const fontItalic = false,
			TextHAlignment const hAlignment = TextHAlignment::Left,
			TextVAlignment const vAlignment = TextVAlignment::Center
		) :
			_TextColor(textColor),
			_FontFamily(fontFamily),
			_FontSize(fontSize),
			_FontBold(fontBold),
			_FontItalic(fontItalic),
			_TextHAlignment(hAlignment),
			_TextVAlignment(vAlignment)
		{
			updateTextFormat();
		}

	public:
		Color textColor() const { return _TextColor; }
		StringView fontFamily() const { return _FontFamily; }
		float fontSize() const { return _FontSize; }
		bool fontBold() const { return _FontBold; }
		bool fontItalic() const { return _FontItalic; }
		TextHAlignment textHAlignment() const { return _TextHAlignment; }
		TextVAlignment textVAlignment() const { return _TextVAlignment; }

	public:
		void textColor(Color const& newValue)
		{
			if (_TextColor == newValue)
			{
				return;
			}
			_TextColor = newValue;
			notifyPropertyChanged();
		}
		void fontFamily(StringView const& newValue)
		{
			if (_FontFamily == newValue)
			{
				return;
			}
			_FontFamily = newValue;
			updateTextFormat();
			notifyPropertyChanged();
		}
		void fontSize(float const newValue)
		{
			if (_FontSize == newValue)
			{
				return;
			}
			_FontSize = newValue;
			updateTextFormat();
			notifyPropertyChanged();
		}
		void fontBold(bool const newValue)
		{
			if (_FontBold == newValue)
			{
				return;
			}
			_FontBold = newValue;
			updateTextFormat();
			notifyPropertyChanged();
		}
		void fontItalic(bool const newValue)
		{
			if (_FontItalic == newValue)
			{
				return;
			}
			_FontItalic = newValue;
			updateTextFormat();
			notifyPropertyChanged();
		}
		void textHAlignment(TextHAlignment const newValue)
		{
			if (_TextHAlignment == newValue)
			{
				return;
			}
			_TextHAlignment = newValue;
			updateTextFormat();
			notifyPropertyChanged();
		}
		void textVAlignment(TextVAlignment const newValue)
		{
			if (_TextVAlignment == newValue)
			{
				return;
			}
			_TextVAlignment = newValue;
			updateTextFormat();
			notifyPropertyChanged();
		}

	public:
		void updateTextFormat(void);
		TextFormat const& textFormat() const { return _TextFormat; }

	public:
		void copyTo(TextStyle& dest) const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void copyLineStyle(LineStyle const& source, LineStyle& target);
	void copyFillStyle(FillStyle const& source, FillStyle& target);
	void copyTextStyle(TextStyle const& source, TextStyle& target);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::wstring to_std_wstring(FillStyle const& v);
	FillStyle to_FillStyle(std::wstring const& v);

	//=======================================================================
	std::wstring to_std_wstring(LineStyle const& v);
	LineStyle to_LineStyle(std::wstring const& v);

	//=======================================================================
	std::wstring to_std_wstring(TextHAlignment const& v);
	TextHAlignment to_TextHAlignment(std::wstring const& v);

	std::wstring to_std_wstring(TextVAlignment const& v);
	TextVAlignment to_TextVAlignment(std::wstring const& v);

	std::wstring to_std_wstring(TextStyle const& v);
	TextStyle to_TextStyle(std::wstring const& v);
}

