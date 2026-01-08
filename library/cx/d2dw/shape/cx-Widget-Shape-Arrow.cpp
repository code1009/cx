/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace
{
	//=======================================================================
	static constexpr std::size_t pLT = 0;
	static constexpr std::size_t pRB = 1;

	static constexpr std::size_t pLTHeadLC = 2;
	static constexpr std::size_t pLTHeadRC = 3;

	static constexpr std::size_t pLTHeadLT = 4;
	static constexpr std::size_t pLTHeadLB = 5;
	static constexpr std::size_t pLTHeadRB = 6;
	static constexpr std::size_t pLTHeadRT = 7;
	static constexpr std::size_t pLTHeadC = 8;

	static constexpr std::size_t pRBHeadLC = 9;
	static constexpr std::size_t pRBHeadRC = 10;

	static constexpr std::size_t pRBHeadLT = 11;
	static constexpr std::size_t pRBHeadLB = 12;
	static constexpr std::size_t pRBHeadRB = 13;
	static constexpr std::size_t pRBHeadRT = 14;
	static constexpr std::size_t pRBHeadC = 15;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	Arrow::Arrow() :
		Item(L"cx.Diagram.Shape.Arrow")
	{
		_ShapeStyle = std::make_shared<ShapeStyle>();
		_ShapeStyle->fill().fillColor(Colors::LightGray());
		_ShapeStyle->line().lineColor(Colors::LightGray());
		_ShapeStyle->line().lineSize(5.0f);


		std::vector<Point>& points = getPoints();
		points.resize(16);
		points[0] = Point{   0, 0 }; // lt
		points[1] = Point{ 100, 0 }; // rb		


		updatePoints();


		attachPropertyChangedEventHandler();
		shapeStyle_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void Arrow::attachPropertyChangedEventHandler(void)
	{
		propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Arrow::onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Arrow::detachPropertyChangedEventHandler(void)
	{
		propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Arrow::onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Arrow::onPropertyChanged:" << event.eventType()
			;
		*/
		updatePoints();
	}

	//=======================================================================
	void Arrow::shapeStyle_attachPropertyChangedEventHandler(void)
	{
		_ShapeStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Arrow::shapeStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Arrow::shapeStyle_detachPropertyChangedEventHandler(void)
	{
		_ShapeStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Arrow::shapeStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Arrow::shapeStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Item> Arrow::clone() const
	{
		auto item = std::make_shared<Arrow>();
		copyTo(item);
		return item;
	}

	void Arrow::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Arrow>(dest);
		Item::copyTo(other);
		other->headLength(headLength());
		other->headWidth(headWidth());
		other->hasLTHead(hasLTHead());
		other->hasRBHead(hasRBHead());
		shapeStyle().copyTo(other->shapeStyle());
	}

	//=======================================================================
	bool Arrow::isPointIn(const Point& test)
	{
		Coord hitSize;
		if (shapeStyle().line().lineSize() < _MinHitSize)
		{
			hitSize = _MinHitSize;
		}
		else
		{
			hitSize = shapeStyle().line().lineSize();
		}
		return
			isPointOnLine(
				test,
				getPoint(0),
				getPoint(1),
				hitSize
			);
	}

	//=======================================================================
	void Arrow::drawContent(DrawingContext& dctx)
	{
		drawLine(dctx);
		
		if(hasLTHead())
		{
			drawLTHead(dctx);
		}
		if(hasRBHead())
		{
			drawRBHead(dctx);
		}

#if 0
		std::vector<Point>& points = getPoints();


		dctx.DrawLine(
			points[0].X, points[0].Y,
			points[1].X, points[1].Y,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);


		std::array<winrt::Windows::Foundation::Numerics::float2, 4> ltPoints =
		{
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadLT].X, points[pLTHeadLT].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadLB].X, points[pLTHeadLB].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadRB].X, points[pLTHeadRB].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadRT].X, points[pLTHeadRT].Y }
		};
		auto ltGeometry = winrt::Microsoft::Graphics::Canvas::Geometry::CanvasGeometry::CreatePolygon(dctx.Device(), ltPoints);

		dctx.FillGeometry(ltGeometry,
			shapeStyle().fill().fillColor()
		);
		dctx.DrawGeometry(ltGeometry,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);


		std::array<winrt::Windows::Foundation::Numerics::float2, 4> rbPoints =
		{
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadLT].X, points[pRBHeadLT].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadLB].X, points[pRBHeadLB].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadRB].X, points[pRBHeadRB].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadRT].X, points[pRBHeadRT].Y }
		};
		auto rbGeometry = winrt::Microsoft::Graphics::Canvas::Geometry::CanvasGeometry::CreatePolygon(dctx.Device(), rbPoints);

		dctx.FillGeometry(rbGeometry,
			shapeStyle().fill().fillColor()
		);
		dctx.DrawGeometry(rbGeometry,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);
#endif
	}

	void Arrow::drawLine(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();


		std::size_t lt = hasLTHead() ? pLTHeadRC : 0;
		std::size_t rb = hasRBHead() ? pRBHeadLC : 1;

		dctx.DrawLine(
			points[lt].X, points[lt].Y,
			points[rb].X, points[rb].Y,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);
	}

	void Arrow::drawLTHead(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();


		dctx.DrawLine(
			points[0].X, points[0].Y,
			points[1].X, points[1].Y,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);

		std::array<winrt::Windows::Foundation::Numerics::float2, 3> ltPoints =
		{
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadLC].X, points[pLTHeadLC].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadRT].X, points[pLTHeadRT].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLTHeadRB].X, points[pLTHeadRB].Y }
		};
		auto ltGeometry = winrt::Microsoft::Graphics::Canvas::Geometry::CanvasGeometry::CreatePolygon(dctx.Device(), ltPoints);

		dctx.FillGeometry(ltGeometry,
			shapeStyle().fill().fillColor()
		);
		dctx.DrawGeometry(ltGeometry,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);
	}

	void Arrow::drawRBHead(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();


		std::array<winrt::Windows::Foundation::Numerics::float2, 3> rbPoints =
		{
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadRC].X, points[pRBHeadRC].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadLT].X, points[pRBHeadLT].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRBHeadLB].X, points[pRBHeadLB].Y }
		};
		auto rbGeometry = winrt::Microsoft::Graphics::Canvas::Geometry::CanvasGeometry::CreatePolygon(dctx.Device(), rbPoints);

		dctx.FillGeometry(rbGeometry,
			shapeStyle().fill().fillColor()
		);
		dctx.DrawGeometry(rbGeometry,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);
	}

	//=======================================================================
	void Arrow::updatePoints(void)
	{
		//-------------------------------------------------------------------
		std::vector<Point>& points = getPoints();
		Coord offset;


		//-------------------------------------------------------------------
		offset = 0.0f;
		points[pLTHeadLC] = getLineOffsetPoint(points[0], points[1], offset);

		offset = _headLength;
		points[pLTHeadRC] = getLineOffsetPoint(points[0], points[1], offset);

		offset = _headWidth * 0.5f;
		points[pLTHeadLT] = getLinePerpendicularPointTop(points[0], points[1], points[pLTHeadLC], offset);
		points[pLTHeadLB] = getLinePerpendicularPointBottom(points[0], points[1], points[pLTHeadLC], offset);
		points[pLTHeadRB] = getLinePerpendicularPointBottom(points[0], points[1], points[pLTHeadRC], offset);
		points[pLTHeadRT] = getLinePerpendicularPointTop(points[0], points[1], points[pLTHeadRC], offset);

		offset = _headLength * 0.5f;
		points[pLTHeadC] = getLineOffsetPoint(points[0], points[1], offset);


		//-------------------------------------------------------------------
		offset = 0.0f;
		points[pRBHeadRC] = getLineOffsetPoint(points[1], points[0], offset);

		offset = _headLength;
		points[pRBHeadLC] = getLineOffsetPoint(points[1], points[0], offset);

		offset = _headWidth * 0.5f;
		points[pRBHeadLT] = getLinePerpendicularPointBottom(points[1], points[0], points[pRBHeadLC], offset);
		points[pRBHeadLB] = getLinePerpendicularPointTop(points[1], points[0], points[pRBHeadLC], offset);
		points[pRBHeadRB] = getLinePerpendicularPointTop(points[1], points[0], points[pRBHeadRC], offset);
		points[pRBHeadRT] = getLinePerpendicularPointBottom(points[1], points[0], points[pRBHeadRC], offset);

		offset = _headLength * 0.5f;
		points[pRBHeadC] = getLineOffsetPoint(points[1], points[0], offset);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	ArrowDesign::ArrowDesign() :
		Design(L"cx.Diagram.Shape.Arrow.Design")
	{
		auto target = std::make_shared<Arrow>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> ArrowDesign::clone() const
	{
		auto item = std::make_shared<ArrowDesign>();
		copyTo(item);
		return item;
	}

	void ArrowDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<ArrowDesign>(dest);
		Design::copyTo(other);
	}

	//=======================================================================
	void ArrowDesign::moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position)
	{
		//-------------------------------------------------------------------
		auto target = getTarget();
		std::vector<Point>& targetPoints = target->getPoints();


		std::vector<Point>& points = getPoints();


		//-------------------------------------------------------------------
		bool rv;
		rv = setHLineAnchorPoint(
			points[0],
			points[1],
			anchor->anchorType(),
			position
		);
		if (false == rv)
		{
			return;
		}


		//-------------------------------------------------------------------
		targetPoints[0] = points[0];
		targetPoints[1] = points[1];


		//-------------------------------------------------------------------
		std::dynamic_pointer_cast<Arrow>(getTarget())->updatePoints();


		//-------------------------------------------------------------------
		notifyPropertyChanged();
	}

	//=======================================================================
	void ArrowDesign::updateTarget(void)
	{
		auto target = getTarget();
		std::vector<Point> targetPoints = target->getPoints();


		std::vector<Point>& points = getPoints();
		points.clear();
		points.resize(2);
		points[0] = targetPoints[0];
		points[1] = targetPoints[1];


		anchors().clear();
		anchors().add(std::make_shared<Anchor>(points[0], AnchorTypes::LT, 0, L"L"));
		anchors().add(std::make_shared<Anchor>(points[1], AnchorTypes::RB, 1, L"R"));
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class PropertyFriendlyNames_Arrow
	{
	public:
		static constexpr StringView headWidth  = L"머리넓이";
		static constexpr StringView headLength = L"머리길이";
		static constexpr StringView hasLTHead  = L"L화실표";
		static constexpr StringView hasRBHead  = L"R화실표";

		static constexpr StringView shapeStyle_fill = PropertyFriendlyNames::fillStyle;
		static constexpr StringView shapeStyle_line = PropertyFriendlyNames::lineStyle;
	};

	//=======================================================================
	class PropertyNames_Arrow
	{
	public:
		static constexpr StringView headWidth  = L"Arrow.headWidth";
		static constexpr StringView headLength = L"Arrow.headLength";
		static constexpr StringView hasLTHead  = L"Arrow.hasLTHead";
		static constexpr StringView hasRBHead  = L"Arrow.hasRBHead";

		static constexpr StringView shapeStyle_fill = L"Arrow.shapeStyle.fill";
		static constexpr StringView shapeStyle_line = L"Arrow.shapeStyle.line";
	};

	//=======================================================================
	class PropertiesBuilder_Arrow
	{
	private:
		std::shared_ptr<Properties>& _Properties;

	public:
		PropertiesBuilder_Arrow(std::shared_ptr<Properties>& properties) :
			_Properties(properties)
		{
		}

	public:
		void build(std::shared_ptr<Arrow> item)
		{
			build_headWidth  (item);
			build_headLength (item);
			build_hasLTHead  (item);
			build_hasRBHead  (item);

			build_shapeStyle_fill(item);
			build_shapeStyle_line(item);
		}

		void build_headWidth(std::shared_ptr<Arrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Arrow::headWidth),
				String(PropertyNames_Arrow::headWidth),
				String(PropertyTypes::Float),
				[item]() -> String
				{
					return cx::to_std_wstring(item->headWidth());
				},
				[item](StringView const& newValue)
				{
					item->headWidth(to_Coord(String(newValue)));
				}
			);
			_Properties->add(property);
		}

		void build_headLength(std::shared_ptr<Arrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Arrow::headLength),
				String(PropertyNames_Arrow::headLength),
				String(PropertyTypes::Float),
				[item]() -> String
				{
					return cx::to_std_wstring(item->headLength());
				},
				[item](StringView const& newValue)
				{
					item->headLength(to_Coord(String(newValue)));
				}
			);
			_Properties->add(property);
		}

		void build_hasLTHead(std::shared_ptr<Arrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Arrow::hasLTHead),
				String(PropertyNames_Arrow::hasLTHead),
				String(PropertyTypes::Bool),
				[item]() -> String
				{
					return cx::to_std_wstring(item->hasLTHead());
				},
				[item](StringView const& newValue)
				{
					item->hasLTHead(cx::to_bool(String(newValue)));
				}
			);
			_Properties->add(property);
		}

		void build_hasRBHead(std::shared_ptr<Arrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Arrow::hasRBHead),
				String(PropertyNames_Arrow::hasRBHead),
				String(PropertyTypes::Bool),
				[item]() -> String
				{
					return cx::to_std_wstring(item->hasRBHead());
				},
				[item](StringView const& newValue)
				{
					item->hasRBHead(cx::to_bool(String(newValue)));
				}
			);
			_Properties->add(property);
		}

		void build_shapeStyle_fill(std::shared_ptr<Arrow> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Arrow::shapeStyle_fill),
				String(PropertyNames_Arrow::shapeStyle_fill),
				String(PropertyTypes::FillStyle),
				[item]() -> String
				{
					return to_std_wstring(item->shapeStyle().fill());
				},
				[item](StringView const& newValue)
				{
					copyFillStyle(
						to_FillStyle(String(newValue)),
						item->shapeStyle().fill()
					);
				}
			);

			_Properties->add(property);
		}

		void build_shapeStyle_line(std::shared_ptr<Arrow> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Arrow::shapeStyle_line),
				String(PropertyNames_Arrow::shapeStyle_line),
				String(PropertyTypes::LineStyle),
				[item]() -> String
				{
					return to_std_wstring(item->shapeStyle().line());
				},
				[item](StringView const& newValue)
				{
					copyLineStyle(
						to_LineStyle(String(newValue)),
						item->shapeStyle().line()
					);
				}
			);

			_Properties->add(property);
		}
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Arrow(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Arrow> target = std::dynamic_pointer_cast<Arrow>(item);

		auto properties = makeProperties_Item(target);

		PropertiesBuilder_Arrow propertiesBuilder(properties);
		propertiesBuilder.build(target);

		return properties;
	}

	std::shared_ptr<Properties> makeProperties_ArrowDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<ArrowDesign>(item);
		std::shared_ptr<Arrow> target = std::dynamic_pointer_cast<Arrow>(design->getTarget());

		return makeProperties_Arrow(target);
	}
}





