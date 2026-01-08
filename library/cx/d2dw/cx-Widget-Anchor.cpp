/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	AnchorStyle::AnchorStyle()
	{
		_FillStyle = std::make_shared<FillStyle>(Color{ 192, 64, 64, 255 });
		_LineStyle = std::make_shared<LineStyle>(Color{ 128, 64, 64, 255 }, 1.0f);
		_TextStyle = std::make_shared<TextStyle>(
			Colors::White(),
			DrawingDefaults::fontFamily,
			9.0f,
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
	AnchorStyle::AnchorStyle(const AnchorStyle& other) :
		//propertyChangedEventListener(other.propertyChangedEventListener),
		_FillStyle(nullptr),
		_LineStyle(nullptr),
		_TextStyle(nullptr)
	{
		if (other._FillStyle) _FillStyle = std::make_shared<FillStyle>(*other._FillStyle);
		if (other._LineStyle) _LineStyle = std::make_shared<LineStyle>(*other._LineStyle);
		if (other._TextStyle) _TextStyle = std::make_shared<TextStyle>(*other._TextStyle);
	}

	AnchorStyle& AnchorStyle::operator=(const AnchorStyle& other)
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

	AnchorStyle::AnchorStyle(AnchorStyle&& other) noexcept :
		//propertyChangedEventListener(std::move(other.propertyChangedEventListener)),
		_FillStyle(std::move(other._FillStyle)),
		_LineStyle(std::move(other._LineStyle)),
		_TextStyle(std::move(other._TextStyle))
	{
	}

	AnchorStyle& AnchorStyle::operator=(AnchorStyle&& other) noexcept
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
	void AnchorStyle::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"AnchorStyle::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			AnchorStyle_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void AnchorStyle::fill_attachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&AnchorStyle::fill_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void AnchorStyle::fill_detachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}
	
	void AnchorStyle::fill_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"AnchorStyle::fill_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}
	
	//=======================================================================
	void AnchorStyle::line_attachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&AnchorStyle::line_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void AnchorStyle::line_detachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void AnchorStyle::line_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"AnchorStyle::line_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void AnchorStyle::text_attachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&AnchorStyle::text_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void AnchorStyle::text_detachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void AnchorStyle::text_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"AnchorStyle::text_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Anchor::Anchor(Point& p, AnchorType const anchorType, std::uint32_t const id, StringView const& text) :
		_Point(p),
		_AnchorType(anchorType),
		_Id(id),
		_Text(text)
	{
		_AnchorStyle = std::make_unique<AnchorStyle>();


		anchorStyle_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void Anchor::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Anchor::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			Anchor_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void Anchor::anchorStyle_attachPropertyChangedEventHandler(void)
	{
		_AnchorStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Anchor::anchorStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Anchor::anchorStyle_detachPropertyChangedEventHandler(void)
	{
		_AnchorStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Anchor::anchorStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace) 
			<< L"Anchor::anchorStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Anchor::visible(bool const newValue)
	{
		if (_Visible == newValue)
		{
			return;
		}
		_Visible = newValue;
		notifyPropertyChanged();
	}

	//=======================================================================
	void Anchor::calcBounds(Point& p0, Point& p1) const
	{
		p0.X = _Point.X - _DefaultHalfSize;
		p0.Y = _Point.Y - _DefaultHalfSize;
		p1.X = _Point.X + _DefaultHalfSize;
		p1.Y = _Point.Y + _DefaultHalfSize;
	}

	bool Anchor::isPointIn(Point const& test)
	{
		Point p0;
		Point p1;
		calcBounds(p0, p1);
		if (isPointInBounds(p0, p1, test))
		{
			return true;
		}
		return false;
	}

	//=======================================================================
	void Anchor::draw(DrawingContext& dctx)
	{
		dctx.FillCircle(_Point, _DefaultHalfSize, anchorStyle().fill().fillColor());
		dctx.DrawCircle(_Point, _DefaultHalfSize, anchorStyle().line().lineColor(), anchorStyle().line().lineSize());

		Coord left = _Point.X - _DefaultHalfSize;
		Coord top = _Point.Y - _DefaultHalfSize;
		Coord right = _Point.X + _DefaultHalfSize;
		Coord bottom = _Point.Y + _DefaultHalfSize;

		if (!text().empty())
		{
			dctx.DrawText(
				text(),
				left, top,
				right - left, bottom - top,
				anchorStyle().text().textColor(),
				anchorStyle().text().textFormat()
			);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Anchors::Anchors()
	{
		_AnchorStyle = std::make_unique<AnchorStyle>();


		anchorStyle_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void Anchors::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Anchors::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			Anchors_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void Anchors::anchorStyle_attachPropertyChangedEventHandler(void)
	{
		_AnchorStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Anchors::anchorStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Anchors::anchorStyle_detachPropertyChangedEventHandler(void)
	{
		_AnchorStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Anchors::anchorStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Anchors::anchorStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Anchors::anchor_attachPropertyChangedEventHandler(std::shared_ptr<Anchor> const& item)
	{
		item->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Anchors::anchor_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Anchors::anchor_detachPropertyChangedEventHandler(std::shared_ptr<Anchor> const& item)
	{
		item->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Anchors::anchor_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Anchors::anchor_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Anchor> Anchors::at(std::size_t const index) const
	{
		if (index < _Items.size())
		{
			return _Items.at(index);
		}
		return nullptr;
	}

	bool Anchors::contains(std::shared_ptr<Anchor> const& test)
	{
		auto found = std::find(
			_Items.begin(),
			_Items.end(),
			test);
		if (found != _Items.end())
		{
			return true;
		}
		return false;
	}

	std::size_t Anchors::indexOf(std::shared_ptr<Anchor> const& item) const
	{
		auto found = std::find(
			_Items.begin(),
			_Items.end(),
			item);

		std::size_t index = std::numeric_limits<std::size_t>::max();
		if (found != _Items.end())
		{
			index = std::distance(_Items.begin(), found);
		}
		return index;
	}

	void Anchors::add(std::shared_ptr<Anchor> const& item)
	{
		anchor_attachPropertyChangedEventHandler(item);
		if (_Visible)
		{
			item->visible(true);
		}
		_Items.push_back(item);
		notifyPropertyChanged();
	}

	//=======================================================================
	void Anchors::insert(std::size_t const index, std::shared_ptr<Anchor> const& item)
	{
		if (index >= _Items.size())
		{
			add(item);
			return;
		}
		anchor_attachPropertyChangedEventHandler(item);
		if (_Visible)
		{
			item->visible(true);
		}
		_Items.insert(_Items.begin() + index, item);
		notifyPropertyChanged();
	}

	void Anchors::remove(std::shared_ptr<Anchor> const& item)
	{
		auto found = std::find(_Items.begin(), _Items.end(), item);
		if (found == _Items.end())
		{
			return;
		}
		anchor_detachPropertyChangedEventHandler(*found);
		_Items.erase(found);
		/*
		if (empty())
		{
			visible(false);
		}
		*/
		notifyPropertyChanged();
	}

	void Anchors::clear()
	{
		if (empty())
		{
			return;
		}
		for (auto& item : _Items)
		{
			anchor_detachPropertyChangedEventHandler(item);
		}
		_Items.clear();
		/*
		visible(false);
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Anchor> Anchors::find(Point const& test) const
	{
		for (auto it = _Items.rbegin(); it != _Items.rend(); it++)
		{
			auto item = *it;
			if (item->isPointIn(test))
			{
				return item;
			}
		}
		return nullptr;
	}

	//=======================================================================
	void Anchors::visible(bool const newValue)
	{
		if (_Visible == newValue)
		{
			return;
		}
		_Visible = newValue;
		for (auto& item : _Items)
		{
			item->visible(newValue);
		}
		notifyPropertyChanged();
	}

	//=======================================================================
	bool Anchors::isPointIn(const Point& test)
	{
		if (!visible())
		{
			return false;
		}
		for (auto& item : _Items)
		{
			if (item->isPointIn(test))
			{
				return true;
			}
		}
		return false;
	}

	//=======================================================================
	void Anchors::draw(DrawingContext& dctx)
	{
		if (!_Visible)
		{
			return;
		}
		drawPoints(dctx);
		drawPointsBounds(dctx);
	}

	//=======================================================================
	void Anchors::drawPoints(DrawingContext& dctx)
	{
		for (auto item : _Items)
		{
			item->draw(dctx);
		}
	}

	//=======================================================================
	void Anchors::calculatePointsBounds(Point& p0, Point& p1) const
	{
		std::vector<Point> points;
		for (auto& item : _Items)
		{
			points.push_back(item->point());
		}
		calculateBounds(points, p0, p1);
	}

	void Anchors::drawPointsBounds(DrawingContext& dctx)
	{
		Point p0;
		Point p1;
		calculatePointsBounds(p0, p1);

		Coord x = p0.X;
		Coord y = p0.Y;
		Coord w = p1.X - p0.X;
		Coord h = p1.Y - p0.Y;
		dctx.DrawRectangle(
			x, y, w, h,
			_AnchorStyle->line().lineColor(),
			_AnchorStyle->line().lineSize()
		);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	bool setRectangleAnchorPoint(
		Point& lt,
		Point& lb,
		Point& rb,
		Point& rt,
		const AnchorType anchorType,
		const Point& p
	)
	{
		switch (anchorType)
		{
		case AnchorTypes::LT:
			if (p == lb) { return false; }
			if (p == rt) { return false; }
			if (p == rb) { return false; }
			if (rb.X <= p.X) { return false; }
			if (rb.Y <= p.Y) { return false; }

			lb.X = p.X;
			rt.Y = p.Y;
			lt = p;
			break;

		case AnchorTypes::RT:
			if (p == lt) { return false; }
			if (p == lb) { return false; }
			if (p == rb) { return false; }
			if (lb.X >= p.X) { return false; }
			if (lb.Y <= p.Y) { return false; }

			rb.X = p.X;
			lt.Y = p.Y;
			rt = p;
			break;

		case AnchorTypes::RB:
			if (p == lt) { return false; }
			if (p == lb) { return false; }
			if (p == rt) { return false; }
			if (lt.X >= p.X) { return false; }
			if (lt.Y >= p.Y) { return false; }

			rt.X = p.X;
			lb.Y = p.Y;
			rb = p;
			break;

		case AnchorTypes::LB:
			if (p == lt) { return false; }
			if (p == rt) { return false; }
			if (p == rb) { return false; }
			if (rt.X <= p.X) { return false; }
			if (rt.Y >= p.Y) { return false; }

			lt.X = p.X;
			rb.Y = p.Y;
			lb = p;
			break;

		default:
			return false;
		}

		return true;
	}

	//=======================================================================
	bool setHLineAnchorPoint(
		Point& lt,
		Point& rb,
		const AnchorType anchorType,
		const Point& p
	)
	{
		switch (anchorType)
		{
		case AnchorTypes::LT:
			if (p == rb) { return false; }
			if (rb.X < p.X) { return false; }
			//if (rb.Y < p.Y) { return false; }

			lt = p;
			break;

		case AnchorTypes::RB:
			if (p == lt) { return false; }
			if (lt.X > p.X) { return false; }
			//if (lt.Y > p.Y) { return false; }

			rb = p;
			break;

		default:
			return false;
		}

		return true;
	}

	bool setVLineAnchorPoint(
		Point& lt,
		Point& rb,
		const AnchorType anchorType,
		const Point& p
	)
	{
		switch (anchorType)
		{
		case AnchorTypes::LT:
			if (p == rb) { return false; }
			//if (rb.X < p.X) { return false; }
			if (rb.Y < p.Y) { return false; }

			lt = p;
			break;

		case AnchorTypes::RB:
			if (p == lt) { return false; }
			//if (lt.X > p.X) { return false; }
			if (lt.Y > p.Y) { return false; }

			rb = p;
			break;

		default:
			return false;
		}

		return true;
	}
}
