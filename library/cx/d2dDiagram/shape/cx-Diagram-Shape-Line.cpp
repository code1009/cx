/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	Line::Line() :
		Item(L"cx.Diagram.Shape.Line")
	{
		_ShapeStyle = std::make_shared<ShapeStyle>();


		std::vector<Point>& points = getPoints();
		points.resize(2);
		points[0] = Point{   0,   0 }; // lt
		points[1] = Point{ 100, 100 }; // rb	


		shapeStyle_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void Line::shapeStyle_attachPropertyChangedEventHandler(void)
	{
		_ShapeStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Line::shapeStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Line::shapeStyle_detachPropertyChangedEventHandler(void)
	{
		_ShapeStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Line::shapeStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace) 
			<< L"Line::shapeStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Item> Line::clone() const
	{
		auto item = std::make_shared<Line>();
		copyTo(item);
		return item;
	}

	void Line::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Line>(dest);
		Item::copyTo(other);
		shapeStyle().copyTo(other->shapeStyle());
	}

	//=======================================================================
	bool Line::isPointIn(const Point& test)
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
	void Line::drawContent(DrawingContext& dctx)
	{
		std::vector<Point>& points = getPoints();

		dctx.DrawLine(
			points[0].X, points[0].Y,
			points[1].X, points[1].Y,
			shapeStyle().line().lineColor(),
			shapeStyle().line().lineSize()
		);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	LineDesign::LineDesign() :
		Design(L"cx.Diagram.Shape.Line.Design")
	{
		auto target = std::make_shared<Line>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> LineDesign::clone() const
	{
		auto item = std::make_shared<LineDesign>();
		copyTo(item);
		return item;
	}

	void LineDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<LineDesign>(dest);
		Design::copyTo(other);
	}

	//=======================================================================
	void LineDesign::moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position)
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
		notifyPropertyChanged();
	}

	//=======================================================================
	void LineDesign::updateTarget(void)
	{
		auto target = getTarget();
		std::vector<Point> targetPoints = target->getPoints();


		std::vector<Point>& points = getPoints();
		points.clear();
		points.resize(2);
		points[0] = targetPoints[0];
		points[1] = targetPoints[1];


		anchors().clear();
		anchors().add(std::make_shared<Anchor>(points[0], AnchorTypes::LT, 0));
		anchors().add(std::make_shared<Anchor>(points[1], AnchorTypes::RB, 1));
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class PropertyFriendlyNames_Line
	{
	public:
		static constexpr StringView shapeStyle_line = PropertyFriendlyNames::lineStyle;
	};

	//=======================================================================
	class PropertyNames_Line
	{
	public:
		static constexpr StringView shapeStyle_line = L"Line.shapeStyle.line";
	};

	//=======================================================================
	class PropertiesBuilder_Line
	{
	private:
		std::shared_ptr<Properties>& _Properties;

	public:
		PropertiesBuilder_Line(std::shared_ptr<Properties>& properties) :
			_Properties(properties)
		{
		}

	public:
		void build(std::shared_ptr<Line> item)
		{
			build_shapeStyle_line(item);
		}

		void build_shapeStyle_line(std::shared_ptr<Line> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Line::shapeStyle_line),
				String(PropertyNames_Line::shapeStyle_line),
				String(PropertyTypes::LineStyle),
				[item]() -> String
				{
					CX_RUNTIME_ASSERT(nullptr != item);

					return to_std_wstring(item->shapeStyle().line());
				},
				[item](StringView const& newValue)
				{
					CX_RUNTIME_ASSERT(nullptr != item);

					/*
					CX_RUNTIME_LOG(cxLTrace)
						<< L"PropertiesBuilder_Line::build_shapeStyle_line::setter-BEGIN" << std::endl
						<< "    item = " << std::format(L"{}", reinterpret_cast<uintptr_t>(item.get()))
						;
					*/

					copyLineStyle(
						to_LineStyle(String(newValue)),
						item->shapeStyle().line()
					);

					/*
					CX_RUNTIME_LOG(cxLTrace)
						<< L"PropertiesBuilder_Line::build_shapeStyle_line::setter-END" << std::endl
						<< "    item = " << std::format(L"{}", reinterpret_cast<uintptr_t>(item.get()))
						;
					*/
				}
			);

			_Properties->add(property);
		}
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Line(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Line> target = std::dynamic_pointer_cast<Line>(item);

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"makeProperties_Line:" << std::endl
			<< L"    className=" << target->className() << std::endl
			<< L"    target=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(target.get()))
			;
		*/

		auto properties = makeProperties_Item(target);

		PropertiesBuilder_Line propertiesBuilder(properties);
		propertiesBuilder.build(target);

		return properties;
	}

	std::shared_ptr<Properties> makeProperties_LineDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<LineDesign>(item);
		std::shared_ptr<Line> target = std::dynamic_pointer_cast<Line>(design->getTarget());

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"makeProperties_LineDesign:" << std::endl
			<< L"    className=" << design->className() << std::endl
			<< L"    design=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(design.get())) << std::endl
			<< L"    target=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(target.get()))
			;
		*/

		return makeProperties_Line(target);
	}
}





