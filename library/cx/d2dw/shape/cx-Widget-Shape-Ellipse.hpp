#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	void drawEllipseShape(DrawingContext& dctx, std::shared_ptr<Item> shapeTemplate);

	struct Ellipse : public BasicTemplate
	{
		Ellipse() : BasicTemplate{ L"cx.Diagram.Shape.Ellipse", drawEllipseShape } {}
	};

	//=======================================================================
	std::shared_ptr<Item> makeEllipseShape(void);

	struct EllipseDesign : public BasicTemplateDesign
	{
		EllipseDesign() : BasicTemplateDesign{ L"cx.Diagram.Shape.Ellipse.Design", makeEllipseShape } {}
	};
}
