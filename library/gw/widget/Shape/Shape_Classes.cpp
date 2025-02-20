/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void drawEllipseShape(Context* ctx, ShapeTemplate* shapeTemplate)
{
	//-----------------------------------------------------------------------
	coord_t radiusX = (shapeTemplate->getPoints()[1]._x - shapeTemplate->getPoints()[0]._x) / 2;
	coord_t radiusY = (shapeTemplate->getPoints()[1]._y - shapeTemplate->getPoints()[0]._y) / 2;
	Point center
	{
		shapeTemplate->getPoints()[0]._x + radiusX,
		shapeTemplate->getPoints()[0]._y + radiusY
	};

	ctx->getD2dRenderTarget()->FillEllipse(
		D2D1::Ellipse(D2D1_POINT_2F{ center._x, center._y }, radiusX, radiusY),
		shapeTemplate->getFill_Brush()->getSolidColorBrush()
	);
	ctx->getD2dRenderTarget()->DrawEllipse(
		D2D1::Ellipse(D2D1_POINT_2F{center._x, center._y}, radiusX, radiusY),
		shapeTemplate->getLine_Brush()->getSolidColorBrush(),
		shapeTemplate->getLineStyle()->getWidth()
	);


	//-----------------------------------------------------------------------
	D2D1_RECT_F rect = D2D1::RectF(
		shapeTemplate->getPoints()[0]._x, shapeTemplate->getPoints()[0]._y,
		shapeTemplate->getPoints()[1]._x, shapeTemplate->getPoints()[1]._y
	);
	std::wstring text = shapeTemplate->getText();
	if (text.empty() == false)
	{
		ctx->getD2dRenderTarget()->DrawTextW(
			text.c_str(),
			static_cast<UINT32>(text.length()),
			shapeTemplate->getText_TextFormat()->getTextFormat(),
			rect,
			shapeTemplate->getText_Brush()->getSolidColorBrush()
		);
	}
}

WidgetSharedPtr makeEllipseShape(void)
{
	auto o = std::make_shared<EllipseShape>();
	return o;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




