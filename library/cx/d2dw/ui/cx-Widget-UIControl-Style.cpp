/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	UIControlStyle::UIControlStyle()
	{
		_FillStyle = std::make_shared<FillStyle>(Color(255, 128, 128, 255));
		_LineStyle = std::make_shared<LineStyle>(Color(255, 64, 64, 255), 2.0f);
		_TextStyle = std::make_shared<TextStyle>(
			Color(255, 255, 255, 255),
			DrawingDefaults::fontFamily,
			12.0f,
			true,
			false,
			TextHAlignment::Center,
			TextVAlignment::Center
		);

		fill_attachPropertyChangedEventHandler();
		line_attachPropertyChangedEventHandler();
		text_attachPropertyChangedEventHandler();
	}


	//=======================================================================
	UIControlStyle::UIControlStyle(const UIControlStyle& other) : 
		//propertyChangedEventListener(other.propertyChangedEventListener),
		_FillStyle(nullptr),
		_LineStyle(nullptr),
		_TextStyle(nullptr)
	{
		if (other._FillStyle) _FillStyle = std::make_shared<FillStyle>(*other._FillStyle);
		if (other._LineStyle) _LineStyle = std::make_shared<LineStyle>(*other._LineStyle);
		if (other._TextStyle) _TextStyle = std::make_shared<TextStyle>(*other._TextStyle);
	}

	UIControlStyle& UIControlStyle::operator=(const UIControlStyle& other)
	{
		if (this != &other)
		{
			//propertyChangedEventListener = other.propertyChangedEventListener;
			if (other._FillStyle)
				_FillStyle = std::make_shared<FillStyle>(*other._FillStyle);
			else
				_FillStyle.reset();

			if (other._LineStyle)
				_LineStyle = std::make_shared<LineStyle>(*other._LineStyle);
			else
				_LineStyle.reset();

			if (other._TextStyle)
				_TextStyle = std::make_shared<TextStyle>(*other._TextStyle);
			else
				_TextStyle.reset();
		}
		return *this;
	}

	UIControlStyle::UIControlStyle(UIControlStyle&& other) noexcept : 
		//propertyChangedEventListener(std::move(other.propertyChangedEventListener)),
		_FillStyle(std::move(other._FillStyle)),
		_LineStyle(std::move(other._LineStyle)),
		_TextStyle(std::move(other._TextStyle))
	{
	}

	UIControlStyle& UIControlStyle::operator=(UIControlStyle&& other) noexcept
	{
		if (this != &other)
		{
			//propertyChangedEventListener = std::move(other.propertyChangedEventListener);
			_FillStyle = std::move(other._FillStyle);
			_LineStyle = std::move(other._LineStyle);
			_TextStyle = std::move(other._TextStyle);
		}
		return *this;
	}

	//=======================================================================
	void UIControlStyle::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"UIControlStyle::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			ItemStyle_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void UIControlStyle::fill_attachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&UIControlStyle::fill_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void UIControlStyle::fill_detachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void UIControlStyle::fill_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"UIControlStyle::fill_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void UIControlStyle::line_attachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&UIControlStyle::line_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void UIControlStyle::line_detachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void UIControlStyle::line_onPropertyChanged(cx::ev::Event& /*event*/)
	{	
		/*
		CX_RUNTIME_LOG(cxLTrace) 
			<< L"UIControlStyle::line_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void UIControlStyle::text_attachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&UIControlStyle::text_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void UIControlStyle::text_detachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void UIControlStyle::text_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"UIControlStyle::text_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	FillStyle& UIControlStyle::fill() const
	{
		return *_FillStyle;
	}

	LineStyle& UIControlStyle::line() const
	{
		return *_LineStyle;
	}

	TextStyle& UIControlStyle::text() const
	{
		return *_TextStyle;
	}

	void UIControlStyle::copyTo(UIControlStyle& dest) const
	{
		//copyUIControlStyle(
		//	*this,
		//	dest
		//);
		_FillStyle->copyTo(*dest._FillStyle);
		_LineStyle->copyTo(*dest._LineStyle);
		_TextStyle->copyTo(*dest._TextStyle);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	void copyUIControlStyle(
		const UIControlStyle& source,
		UIControlStyle& dest
	)
	{
		copyFillStyle(source.fill(), dest.fill());
		copyLineStyle(source.line(), dest.line());
		copyTextStyle(source.text(), dest.text());
	}
}
