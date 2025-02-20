/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void drawEllipseShape(Context* ctx, ShapeTemplate* shapeTemplate);
struct EllipseShape: public ShapeTemplate
{EllipseShape():ShapeTemplate{ L"Shape.Ellipse", drawEllipseShape }{}};

WidgetSharedPtr makeEllipseShape(void);
struct EllipseShapeDesign: public ShapeDesignTemplate
{EllipseShapeDesign():ShapeDesignTemplate{ L"Shape.Ellipse.Design", makeEllipseShape }{}};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void drawTextShape(Context* ctx, ShapeTemplate* shapeTemplate);
struct TextShape: public ShapeTemplate
{TextShape():ShapeTemplate{ L"Shape.Text", drawTextShape }{}};

WidgetSharedPtr makeTextShape(void);
struct TextShapeDesign: public ShapeDesignTemplate
{TextShapeDesign():ShapeDesignTemplate{ L"Shape.Text.Design", makeTextShape }{}};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




