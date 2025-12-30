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
RectangleShape::RectangleShape():
	Shape{ L"Shape.Rectangle" }
{
	_FillStyle.setStyleChangedHandler(
		std::bind(&RectangleShape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_LineStyle.setStyleChangedHandler(
		std::bind(&RectangleShape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_TextStyle.setStyleChangedHandler(
		std::bind(&RectangleShape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);


	_Points.resize(2);


	_Points[0] = Point{   0,   0 }; // lt
	_Points[1] = Point{ 100, 100 }; // rb
}

//===========================================================================
WidgetSharedPtr RectangleShape::clone(void) const
{
	auto o = std::make_shared<RectangleShape>();
	copyTo(o);
	return o;
}

void RectangleShape::draw(Context* ctx)
{
	//-----------------------------------------------------------------------
	D2D1_RECT_F rect = D2D1::RectF(
		_Points[0]._x, _Points[0]._y,
		_Points[1]._x, _Points[1]._y
	);
	ctx->getD2dRenderTarget()->FillRectangle(
		&rect,
		_Fill_Brush->getSolidColorBrush()
	);
	ctx->getD2dRenderTarget()->DrawRectangle(
		&rect,
		_Line_Brush->getSolidColorBrush(),
		_LineStyle.getWidth()
	);


	//-----------------------------------------------------------------------
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
void RectangleShape::onWidgetDrawingStyleChanged(WidgetDrawingStyle::StyleChangedParam* param)
{
	setPropertyChanged(static_cast<std::uint32_t>(Widget::PropertyChangedParam::Code::ReloadResources));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




