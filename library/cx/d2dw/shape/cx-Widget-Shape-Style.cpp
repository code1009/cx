/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	ShapeStyle::ShapeStyle()
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
	ShapeStyle::ShapeStyle(const ShapeStyle& other) : 
		//propertyChangedEventListener(other.propertyChangedEventListener),
		_FillStyle(nullptr),
		_LineStyle(nullptr),
		_TextStyle(nullptr)
	{
		if (other._FillStyle) _FillStyle = std::make_shared<FillStyle>(*other._FillStyle);
		if (other._LineStyle) _LineStyle = std::make_shared<LineStyle>(*other._LineStyle);
		if (other._TextStyle) _TextStyle = std::make_shared<TextStyle>(*other._TextStyle);
	}

	ShapeStyle& ShapeStyle::operator=(const ShapeStyle& other)
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

	ShapeStyle::ShapeStyle(ShapeStyle&& other) noexcept : 
		//propertyChangedEventListener(std::move(other.propertyChangedEventListener)),
		_FillStyle(std::move(other._FillStyle)),
		_LineStyle(std::move(other._LineStyle)),
		_TextStyle(std::move(other._TextStyle))
	{
	}

	ShapeStyle& ShapeStyle::operator=(ShapeStyle&& other) noexcept
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
	void ShapeStyle::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"ShapeStyle::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			ShapeStyle_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void ShapeStyle::fill_attachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&ShapeStyle::fill_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void ShapeStyle::fill_detachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void ShapeStyle::fill_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"ShapeStyle::fill_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void ShapeStyle::line_attachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&ShapeStyle::line_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void ShapeStyle::line_detachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void ShapeStyle::line_onPropertyChanged(cx::ev::Event& /*event*/)
	{	
		/*
		CX_RUNTIME_LOG(cxLTrace) 
			<< L"ShapeStyle::line_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void ShapeStyle::text_attachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&ShapeStyle::text_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void ShapeStyle::text_detachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void ShapeStyle::text_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"ShapeStyle::text_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	FillStyle& ShapeStyle::fill() const
	{
		return *_FillStyle;
	}

	LineStyle& ShapeStyle::line() const
	{
		return *_LineStyle;
	}

	TextStyle& ShapeStyle::text() const
	{
		return *_TextStyle;
	}

	void ShapeStyle::copyTo(ShapeStyle& dest) const
	{
		//copyShapeStyle(
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
namespace cx::Widget::Shape
{
	//=======================================================================
	void copyShapeStyle(
		const ShapeStyle& source,
		ShapeStyle& dest
	)
	{
		copyFillStyle(source.fill(), dest.fill());
		copyLineStyle(source.line(), dest.line());
		copyTextStyle(source.text(), dest.text());
	}
}
