#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	void drawEllipseShape(DrawingContext& dctx, std::shared_ptr<Item> shapeTemplate);

	struct Ellipse : public BasicTemplate
	{
		Ellipse() : BasicTemplate{ L"cx.Widget.Shape.Ellipse", drawEllipseShape } {}
	};

	//=======================================================================
	std::shared_ptr<Item> makeEllipseShape(void);

	struct EllipseDesign : public BasicTemplateDesign
	{
		EllipseDesign() : BasicTemplateDesign{ L"cx.Widget.Shape.Ellipse.Design", makeEllipseShape } {}
	};
}
