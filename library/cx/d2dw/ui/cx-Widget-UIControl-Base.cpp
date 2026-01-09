/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	Base::Base(StringView const& className, StringView const& text) :
		Item(className),
		_Text(text)
	{
		_UIControlStyle = std::make_unique<UIControlStyle>();


		std::vector<Point>& points = getPoints();
		points.resize(2);
		points[0] = Point{   0,   0 }; // lt
		points[1] = Point{ 100, 100 }; // rb


		uiControlStyle_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void Base::uiControlStyle_attachPropertyChangedEventHandler(void)
	{
		_UIControlStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Base::uiControlStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Base::uiControlStyle_detachPropertyChangedEventHandler(void)
	{
		_UIControlStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Base::uiControlStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Base::uiControlStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Base::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Base>(dest);
		Item::copyTo(other);
		other->text(text());
		uiControlStyle().copyTo(other->uiControlStyle());
	}

	//=======================================================================
	void Base::drawContent(DrawingContext& dctx)
	{
		drawText(dctx);
	}

	//=======================================================================
	void Base::drawText(DrawingContext& dctx)
	{
		if (text().empty())
		{
			return;
		}

		auto& points = getPoints();
		Coord x = points[0].X;
		Coord y = points[0].Y;
		Coord w = points[1].X - points[0].X;
		Coord h = points[1].Y - points[0].Y;
		dctx.DrawText(
			text().c_str(),
			x, y,
			w, h,
			uiControlStyle().text().textColor(),
			uiControlStyle().text().textFormat()
		);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	BaseDesign::BaseDesign(StringView const& className) :
		Design(className)
	{
	}

	//=======================================================================
	void BaseDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<BaseDesign>(dest);
		Design::copyTo(other);
	}

	//=======================================================================
	void BaseDesign::moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position)
	{
		//-------------------------------------------------------------------
		auto target = getTarget();
		std::vector<Point>& targetPoints = target->getPoints();


		std::vector<Point>& points = getPoints();


		//-------------------------------------------------------------------
		bool rv;
		rv = setRectangleAnchorPoint(
			points[0],
			points[1],
			points[2],
			points[3],
			anchor->anchorType(),
			position
		);
		if (false == rv)
		{
			return;
		}


		//-------------------------------------------------------------------
		targetPoints[0] = points[0];
		targetPoints[1] = points[2];


		//-------------------------------------------------------------------
		notifyPropertyChanged();
	}

	//=======================================================================
	void BaseDesign::updateTarget(void)
	{
		//-------------------------------------------------------------------
		auto target = getTarget();
		Point p0;
		Point p1;
		target->getBounds(p0, p1);
		std::vector<Point> targetPoints = getBoundsPoints(p0, p1);


		//-------------------------------------------------------------------
		std::vector<Point>& points = getPoints();
		points.clear();
		points.resize(4);
		points[0] = targetPoints[0];
		points[1] = targetPoints[1];
		points[2] = targetPoints[2];
		points[3] = targetPoints[3];


		anchors().clear();
		anchors().add(std::make_shared<Anchor>(points[0], AnchorTypes::LT, 0));
		anchors().add(std::make_shared<Anchor>(points[1], AnchorTypes::LB, 1));
		anchors().add(std::make_shared<Anchor>(points[2], AnchorTypes::RB, 2));
		anchors().add(std::make_shared<Anchor>(points[3], AnchorTypes::RT, 3));
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	class PropertiesBuilder_Base
	{
	private:
		std::shared_ptr<Properties>& _Properties;

	public:
		PropertiesBuilder_Base(std::shared_ptr<Properties>& properties) :
			_Properties(properties)
		{
		}

	public:
		void build(std::shared_ptr<Base> item)
		{
			build_uiControlStyle_fill(item);
			build_uiControlStyle_line(item);
			build_uiControlStyle_text(item);
			build_text(item);
		}

		void build_uiControlStyle_fill(std::shared_ptr<Base> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Base::uiControlStyle_fill),
				String(PropertyNames_Base::uiControlStyle_fill),
				String(PropertyTypes::FillStyle),
				[item]() -> String
				{
					return to_std_wstring(item->uiControlStyle().fill());
				},
				[item](StringView const& newValue)
				{
					copyFillStyle(
						to_FillStyle(String(newValue)),
						item->uiControlStyle().fill()
					);
				}
			);

			_Properties->add(property);
		}
		void build_uiControlStyle_line(std::shared_ptr<Base> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Base::uiControlStyle_line),
				String(PropertyNames_Base::uiControlStyle_line),
				String(PropertyTypes::LineStyle),
				[item]() -> String
				{
					return to_std_wstring(item->uiControlStyle().line());
				},
				[item](StringView const& newValue)
				{
					copyLineStyle(
						to_LineStyle(String(newValue)),
						item->uiControlStyle().line()
					);
				}
			);

			_Properties->add(property);
		}
		void build_uiControlStyle_text(std::shared_ptr<Base> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Base::uiControlStyle_text),
				String(PropertyNames_Base::uiControlStyle_text),
				String(PropertyTypes::TextStyle),
				[item]() -> String
				{
					return to_std_wstring(item->uiControlStyle().text());
				},
				[item](StringView const& newValue)
				{
					copyTextStyle(
						to_TextStyle(String(newValue)),
						item->uiControlStyle().text()
					);
				}
			);

			_Properties->add(property);
		}

		void build_text(std::shared_ptr<Base> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Base::text),
				String(PropertyNames_Base::text),
				String(PropertyTypes::String),
				[item]() -> String
				{
					return item->text();
				},
				[item](StringView const& newValue)
				{
					item->text(newValue);
				}
			);

			_Properties->add(property);
		}
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Base(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Base> target = std::dynamic_pointer_cast<Base>(item);
		auto properties = makeProperties_Item(target);

		PropertiesBuilder_Base propertiesBuilder(properties);
		propertiesBuilder.build(target);

		return properties;
	}

	std::shared_ptr<Properties> makeProperties_BaseDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<BaseDesign> design = std::dynamic_pointer_cast<BaseDesign>(item);
		std::shared_ptr<Base> target = std::dynamic_pointer_cast<Base>(design->getTarget());

		return makeProperties_Base(target);
	}
}


