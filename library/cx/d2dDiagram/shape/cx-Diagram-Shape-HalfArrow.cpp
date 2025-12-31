/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace
{
	//=======================================================================
	static constexpr std::size_t pLT = 0;
	static constexpr std::size_t pRB = 1;

	static constexpr std::size_t pLHeadLC = 2;
	static constexpr std::size_t pLHeadRC = 3;

	static constexpr std::size_t pLHeadLT = 4;
	static constexpr std::size_t pLHeadLB = 5;
	static constexpr std::size_t pLHeadRB = 6;
	static constexpr std::size_t pLHeadRT = 7;
	static constexpr std::size_t pLHeadC = 8;

	static constexpr std::size_t pRHeadLC = 9;
	static constexpr std::size_t pRHeadRC = 10;

	static constexpr std::size_t pRHeadLT = 11;
	static constexpr std::size_t pRHeadLB = 12;
	static constexpr std::size_t pRHeadRB = 13;
	static constexpr std::size_t pRHeadRT = 14;
	static constexpr std::size_t pRHeadC = 15;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	HalfArrow::HalfArrow() :
		Item(L"cx.Diagram.Shape.HalfArrow")
	{
		_ShapeStyle = std::make_shared<ShapeStyle>();
		_ShapeStyle->fill().fillColor(Colors::LightGray());
		_ShapeStyle->line().lineColor(Colors::LightGray());
		_ShapeStyle->line().lineSize(2.0f);


		std::vector<Point>& points = getPoints();
		points.resize(16);
		points[0] = Point{   0, 0 }; // lt
		points[1] = Point{ 100, 0 }; // rb	


		updatePoints();


		attachPropertyChangedEventHandler();
		shapeStyle_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void HalfArrow::attachPropertyChangedEventHandler(void)
	{
		propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&HalfArrow::onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void HalfArrow::detachPropertyChangedEventHandler(void)
	{
		propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void HalfArrow::onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"HalfArrow::onPropertyChanged:" << event.eventType()
			;
		*/
		updatePoints();
	}

	//=======================================================================
	void HalfArrow::shapeStyle_attachPropertyChangedEventHandler(void)
	{
		_ShapeStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&HalfArrow::shapeStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void HalfArrow::shapeStyle_detachPropertyChangedEventHandler(void)
	{
		_ShapeStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void HalfArrow::shapeStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"HalfArrow::shapeStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Item> HalfArrow::clone() const
	{
		auto item = std::make_shared<HalfArrow>();
		copyTo(item);
		return item;
	}

	void HalfArrow::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<HalfArrow>(dest);
		Item::copyTo(other);
		other->headLength(headLength());
		other->headWidth(headWidth());
		other->hasLTHead(hasLTHead());
		other->hasLBHead(hasLBHead());
		other->hasRTHead(hasRTHead());
		other->hasRBHead(hasRBHead());
		shapeStyle().copyTo(other->shapeStyle());
	}

	//=======================================================================
	bool HalfArrow::isPointIn(const Point& test)
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
	void HalfArrow::drawContent(DrawingContext& dctx)
	{
		drawLine(dctx);
		
		if (hasLTHead())
		{
			drawLTHead(dctx);
		}
		if (hasLBHead())
		{
			drawLBHead(dctx);
		}
		if (hasRTHead())
		{
			drawRTHead(dctx);
		}
		if (hasRBHead())
		{
			drawRBHead(dctx);
		}
	}

	void HalfArrow::drawLine(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();


		std::size_t lt = (hasLTHead() || hasLBHead()) ? pLHeadRC : 0;
		std::size_t rb = (hasRTHead() || hasRBHead()) ? pRHeadLC : 1;

		dctx.DrawLine(
			points[lt].X, points[lt].Y,
			points[rb].X, points[rb].Y,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);
	}

	void HalfArrow::drawLTHead(DrawingContext& dctx)
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
			winrt::Windows::Foundation::Numerics::float2{ points[pLHeadLC].X, points[pLHeadLC].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLHeadRT].X, points[pLHeadRT].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLHeadRC].X, points[pLHeadRC].Y }
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

	void HalfArrow::drawLBHead(DrawingContext& dctx)
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
			winrt::Windows::Foundation::Numerics::float2{ points[pLHeadLC].X, points[pLHeadLC].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLHeadRB].X, points[pLHeadRB].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pLHeadRC].X, points[pLHeadRC].Y }
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

	void HalfArrow::drawRTHead(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();


		std::array<winrt::Windows::Foundation::Numerics::float2, 3> rbPoints =
		{
			winrt::Windows::Foundation::Numerics::float2{ points[pRHeadRC].X, points[pRHeadRC].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRHeadLT].X, points[pRHeadLT].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRHeadLC].X, points[pRHeadLC].Y }
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

	void HalfArrow::drawRBHead(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();


		std::array<winrt::Windows::Foundation::Numerics::float2, 3> rbPoints =
		{
			winrt::Windows::Foundation::Numerics::float2{ points[pRHeadRC].X, points[pRHeadRC].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRHeadLB].X, points[pRHeadLB].Y },
			winrt::Windows::Foundation::Numerics::float2{ points[pRHeadLC].X, points[pRHeadLC].Y }
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
	void HalfArrow::updatePoints(void)
	{
		//-------------------------------------------------------------------
		std::vector<Point>& points = getPoints();
		Coord offset;


		//-------------------------------------------------------------------
		offset = 0.0f;
		points[pLHeadLC] = getLineOffsetPoint(points[0], points[1], offset);

		offset = _headLength;
		points[pLHeadRC] = getLineOffsetPoint(points[0], points[1], offset);

		offset = _headWidth * 0.5f;
		points[pLHeadLT] = getLinePerpendicularPointTop(points[0], points[1], points[pLHeadLC], offset);
		points[pLHeadLB] = getLinePerpendicularPointBottom(points[0], points[1], points[pLHeadLC], offset);
		points[pLHeadRB] = getLinePerpendicularPointBottom(points[0], points[1], points[pLHeadRC], offset);
		points[pLHeadRT] = getLinePerpendicularPointTop(points[0], points[1], points[pLHeadRC], offset);

		offset = _headLength * 0.5f;
		points[pLHeadC] = getLineOffsetPoint(points[0], points[1], offset);


		//-------------------------------------------------------------------
		offset = 0.0f;
		points[pRHeadRC] = getLineOffsetPoint(points[1], points[0], offset);

		offset = _headLength;
		points[pRHeadLC] = getLineOffsetPoint(points[1], points[0], offset);

		offset = _headWidth * 0.5f;
		points[pRHeadLT] = getLinePerpendicularPointBottom(points[1], points[0], points[pRHeadLC], offset);
		points[pRHeadLB] = getLinePerpendicularPointTop(points[1], points[0], points[pRHeadLC], offset);
		points[pRHeadRB] = getLinePerpendicularPointTop(points[1], points[0], points[pRHeadRC], offset);
		points[pRHeadRT] = getLinePerpendicularPointBottom(points[1], points[0], points[pRHeadRC], offset);

		offset = _headLength * 0.5f;
		points[pRHeadC] = getLineOffsetPoint(points[1], points[0], offset);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	HalfArrowDesign::HalfArrowDesign() :
		Design(L"cx.Diagram.Shape.HalfArrow.Design")
	{
		auto target = std::make_shared<HalfArrow>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> HalfArrowDesign::clone() const
	{
		auto item = std::make_shared<HalfArrowDesign>();
		copyTo(item);
		return item;
	}

	void HalfArrowDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<HalfArrowDesign>(dest);
		Design::copyTo(other);
	}

	//=======================================================================
	void HalfArrowDesign::moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position)
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
		std::dynamic_pointer_cast<HalfArrow>(getTarget())->updatePoints();


		//-------------------------------------------------------------------
		notifyPropertyChanged();
	}

	//=======================================================================
	void HalfArrowDesign::updateTarget(void)
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
	class PropertyFriendlyNames_HalfArrow
	{
	public:
		static constexpr StringView headWidth  = L"머리넓이";
		static constexpr StringView headLength = L"머리길이";
		static constexpr StringView hasLTHead = L"L상화실표";
		static constexpr StringView hasLBHead = L"L하화실표";
		static constexpr StringView hasRTHead = L"R상화실표";
		static constexpr StringView hasRBHead = L"R하화실표";

		static constexpr StringView shapeStyle_fill = PropertyFriendlyNames::fillStyle;
		static constexpr StringView shapeStyle_line = PropertyFriendlyNames::lineStyle;
	};

	//=======================================================================
	class PropertyNames_HalfArrow
	{
	public:
		static constexpr StringView headWidth  = L"HalfArrow.headWidth";
		static constexpr StringView headLength = L"HalfArrow.headLength";
		static constexpr StringView hasLTHead  = L"HalfArrow.hasLTHead";
		static constexpr StringView hasLBHead  = L"HalfArrow.hasLBHead";
		static constexpr StringView hasRTHead  = L"HalfArrow.hasRTHead";
		static constexpr StringView hasRBHead  = L"HalfArrow.hasRBHead";

		static constexpr StringView shapeStyle_fill = L"HalfArrow.shapeStyle.fill";
		static constexpr StringView shapeStyle_line = L"HalfArrow.shapeStyle.line";
	};

	//=======================================================================
	class PropertiesBuilder_HalfArrow
	{
	private:
		std::shared_ptr<Properties>& _Properties;

	public:
		PropertiesBuilder_HalfArrow(std::shared_ptr<Properties>& properties) :
			_Properties(properties)
		{
		}

	public:
		void build(std::shared_ptr<HalfArrow> item)
		{
			build_headWidth  (item);
			build_headLength (item);

			build_hasLTHead(item);
			build_hasLBHead(item);
			build_hasRTHead(item);
			build_hasRBHead(item);

			build_shapeStyle_fill(item);
			build_shapeStyle_line(item);
		}

		void build_headWidth(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::headWidth),
				String(PropertyNames_HalfArrow::headWidth),
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

		void build_headLength(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::headLength),
				String(PropertyNames_HalfArrow::headLength),
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

		void build_hasLTHead(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::hasLTHead),
				String(PropertyNames_HalfArrow::hasLTHead),
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

		void build_hasLBHead(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::hasLBHead),
				String(PropertyNames_HalfArrow::hasLBHead),
				String(PropertyTypes::Bool),
				[item]() -> String
				{
					return cx::to_std_wstring(item->hasLBHead());
				},
				[item](StringView const& newValue)
				{
					item->hasLBHead(cx::to_bool(String(newValue)));
				}
			);
			_Properties->add(property);
		}

		void build_hasRTHead(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::hasRTHead),
				String(PropertyNames_HalfArrow::hasRTHead),
				String(PropertyTypes::Bool),
				[item]() -> String
				{
					return cx::to_std_wstring(item->hasRTHead());
				},
				[item](StringView const& newValue)
				{
					item->hasRTHead(cx::to_bool(String(newValue)));
				}
			);
			_Properties->add(property);
		}

		void build_hasRBHead(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;
			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::hasRBHead),
				String(PropertyNames_HalfArrow::hasRBHead),
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

		void build_shapeStyle_fill(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::shapeStyle_fill),
				String(PropertyNames_HalfArrow::shapeStyle_fill),
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

		void build_shapeStyle_line(std::shared_ptr<HalfArrow> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_HalfArrow::shapeStyle_line),
				String(PropertyNames_HalfArrow::shapeStyle_line),
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
	std::shared_ptr<Properties> makeProperties_HalfArrow(std::shared_ptr<Item> item)
	{
		std::shared_ptr<HalfArrow> target = std::dynamic_pointer_cast<HalfArrow>(item);

		auto properties = makeProperties_Item(target);

		PropertiesBuilder_HalfArrow propertiesBuilder(properties);
		propertiesBuilder.build(target);

		return properties;
	}

	std::shared_ptr<Properties> makeProperties_HalfArrowDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<HalfArrowDesign>(item);
		std::shared_ptr<HalfArrow> target = std::dynamic_pointer_cast<HalfArrow>(design->getTarget());

		return makeProperties_HalfArrow(target);
	}
}





