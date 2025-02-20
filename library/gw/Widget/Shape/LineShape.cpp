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
LineShape::LineShape():
	Shape{ L"Shape.Line" }
{
	_FillStyle.setStyleChangedHandler(
		std::bind(&LineShape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_LineStyle.setStyleChangedHandler(
		std::bind(&LineShape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_TextStyle.setStyleChangedHandler(
		std::bind(&LineShape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);


	_Points.resize(2);


	_Points[0] = Point{   0,   0 }; // lt
	_Points[1] = Point{ 100, 100 }; // rb
}

//===========================================================================
WidgetSharedPtr LineShape::clone(void) const
{
	auto o = std::make_shared<LineShape>();
	copyTo(o);
	return o;
}

void LineShape::draw(Context* ctx)
{
	//-----------------------------------------------------------------------
	ctx->getD2dRenderTarget()->DrawLine(
		D2D1::Point2F(_Points[0]._x, _Points[0]._y),
		D2D1::Point2F(_Points[1]._x, _Points[1]._y),
		_Line_Brush->getSolidColorBrush(),
		_LineStyle.getWidth()
	);


	//-----------------------------------------------------------------------
	D2D1_RECT_F rect = D2D1::RectF(
		_Points[0]._x, _Points[0]._y,
		_Points[1]._x, _Points[1]._y
	);
	std::wstring text = getText();
	if (text.empty() == false)
	{
		ctx->getD2dRenderTarget()->DrawTextW(
			text.c_str(),
			static_cast<UINT32>(text.length()),
			_Text_TextFormat->getTextFormat(),
			rect,
			_Text_Brush->getSolidColorBrush()
		);
	}
}

//===========================================================================
void LineShape::onWidgetDrawingStyleChanged(WidgetDrawingStyle::StyleChangedParam* param)
{
	setPropertyChanged(0x80000000);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




