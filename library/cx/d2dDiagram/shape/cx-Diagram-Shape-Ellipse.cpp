/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	void drawEllipseShape(DrawingContext& dctx, std::shared_ptr<Item> item)
	{
		std::shared_ptr<Ellipse> shape = std::dynamic_pointer_cast<Ellipse>(item);

		auto& points = shape->getPoints();
		Coord radiusX = (points[1].X - points[0].X) / 2.0f;
		Coord radiusY = (points[1].Y - points[0].Y) / 2.0f;
		Coord x = points[0].X + radiusX;
		Coord y = points[0].Y + radiusY;
		dctx.FillEllipse(x, y, radiusX, radiusY, shape->shapeStyle().fill().fillColor());
		dctx.DrawEllipse(x, y, radiusX, radiusY, shape->shapeStyle().line().lineColor(), shape->shapeStyle().line().lineSize());

		shape->drawText(dctx);
	}

	std::shared_ptr<Item> makeEllipseShape(void)
	{
		return std::make_shared<Ellipse>();
	}
}
