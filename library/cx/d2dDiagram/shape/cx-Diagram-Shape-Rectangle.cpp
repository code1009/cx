/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	Rectangle::Rectangle() :
		Base(L"cx.Diagram.Shape.Rectangle")
	{
	}

	//=======================================================================
	std::shared_ptr<Item> Rectangle::clone() const
	{
		auto item = std::make_shared<Rectangle>();
		copyTo(item);
		return item;
	}

	void Rectangle::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Rectangle>(dest);
		Base::copyTo(other);
	}

	//=======================================================================
	void Rectangle::drawContent(DrawingContext& dctx)
	{
		auto& points = getPoints();
		Coord x = points[0].X;
		Coord y = points[0].Y;
		Coord w = points[1].X - points[0].X;
		Coord h = points[1].Y - points[0].Y;
		dctx.FillRectangle(x, y, w, h, shapeStyle().fill().fillColor());
		dctx.DrawRectangle(x, y, w, h, shapeStyle().line().lineColor(), shapeStyle().line().lineSize());

		drawText(dctx);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	RectangleDesign::RectangleDesign() :
		BaseDesign(L"cx.Diagram.Shape.Rectangle.Design")
	{
		auto target = std::make_shared<Rectangle>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> RectangleDesign::clone() const
	{
		auto item = std::make_shared<RectangleDesign>();
		copyTo(item);
		return item;
	}

	void RectangleDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<RectangleDesign>(dest);
		BaseDesign::copyTo(other);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Rectangle(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Rectangle> target = std::dynamic_pointer_cast<Rectangle>(item);
		auto properties = makeProperties_Base(target);
		return properties;
	}

	std::shared_ptr<Properties> makeProperties_RectangleDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<RectangleDesign>(item);
		std::shared_ptr<Rectangle> target = std::dynamic_pointer_cast<Rectangle>(design->getTarget());

		return makeProperties_Rectangle(target);
	}
}
