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
DesignWidget::DesignWidget(std::wstring className) :
	Widget{ std::move(className) }
{
}

//===========================================================================
void DesignWidget::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	Widget::copyTo(widget);


	DesignWidget* o = dynamic_cast<DesignWidget*>(widget.get());
	o->_WidgetDesignerModel = _WidgetDesignerModel;
	o->_MarkerVisible       = _MarkerVisible      ;
	o->_MarkerWidth         = _MarkerWidth        ;
}

bool DesignWidget::isPointIn(const Point& test) const
{
	if (!_MarkerVisible)
	{
		return false;
	}


	Point p0;
	Point p1;


	for (auto& p : _Points)
	{
		getMarkerBounds(p, p0, p1);

		if (isPointInBounds(p0, p1, test))
		{
			return true;
		}
	}

	return false;
}

//===========================================================================
void DesignWidget::setWidgetDesignerModel(WidgetDesignerModel* widgetDesignerModel)
{
	_WidgetDesignerModel = widgetDesignerModel;
}

DesigeWidgetContext* DesignWidget::getDesigeWidgetContext(void) const
{
	return _WidgetDesignerModel->getDesigeWidgetContext();
}

void DesignWidget::setMarkerVisible(bool visible)
{
	if (_MarkerVisible != visible)
	{
		_MarkerVisible = visible;

		setPropertyChanged();
	}
}

bool DesignWidget::getMarkerVisible(void) const
{
	return _MarkerVisible;
}

void DesignWidget::getMarkerBounds(const Point& marker, Point& p0, Point& p1) const
{
	const coord_t halfWidth = _MarkerWidth / 2.0f;


	p0._x = marker._x - halfWidth;
	p0._y = marker._y - halfWidth;

	p1._x = marker._x + halfWidth;
	p1._y = marker._y + halfWidth;
}

bool DesignWidget::isPointInMarker(const Point& makrer, const Point& test) const
{
	if (!_MarkerVisible)
	{
		return false;
	}


	Point p0;
	Point p1;


	getMarkerBounds(makrer, p0, p1);
	if (isPointInBounds(p0, p1, test))
	{
		return true;
	}

	return false;
}

bool DesignWidget::isPointInMarkers(const Point& test) const
{
	for (auto& p : _Points)
	{
		if (isPointInMarker(p, test))
		{
			return true;
		}
	}

	return false;
}

void DesignWidget::drawMarker(Context* ctx, Point marker)
{
	WidgetResource* _Marker_Fill_Brush      = getDesigeWidgetContext()->getMarker_Fill_Brush     ();
	WidgetResource* _Marker_Line_Brush      = getDesigeWidgetContext()->getMarker_Line_Brush     ();
	WidgetResource* _Marker_Text_Brush      = getDesigeWidgetContext()->getMarker_Text_Brush     ();
	WidgetResource* _Marker_Text_TextFormat = getDesigeWidgetContext()->getMarker_Text_TextFormat();

#if 1
	Point p0;
	Point p1;


	getMarkerBounds(marker, p0, p1);


	D2D1_RECT_F rect = D2D1::RectF(
		p0._x, p0._y,
		p1._x, p1._y
	);

	ctx->getD2dRenderTarget()->FillRectangle(
		&rect,
		_Marker_Fill_Brush->getSolidColorBrush()
	);
	ctx->getD2dRenderTarget()->DrawRectangle(
		&rect,
		_Marker_Line_Brush->getSolidColorBrush(),
		getDesigeWidgetContext()->_Marker_Line_Style.getWidth()
	);
#else
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(marker._x, marker._y),
		_MarkerWidth / 2.0f,
		_MarkerWidth / 2.0f
	);
	ctx->getD2dRenderTarget()->FillEllipse(
		&ellipse,
		_Marker_Fill_Brush->getSolidColorBrush()
	);
	ctx->getD2dRenderTarget()->DrawEllipse(
		&ellipse,
		_Marker_Line_Brush->getSolidColorBrush(),
		1.0f // getDesigeWidgetContext()->_Marker_Line_Style.getWidth()
	);
#endif
}

void DesignWidget::drawMarkers(Context* ctx)
{
	for (auto& p : _Points)
	{
		drawMarker(ctx, p);
	}
}

//===========================================================================
void DesignWidget::drawBounds(Context* ctx, const Point& p0, const Point& p1)
{
	WidgetResource* _Guide_Line_Brush       = getDesigeWidgetContext()->getGuide_Line_Brush      ();
	WidgetResource* _Guide_Line_StrokeStyle = getDesigeWidgetContext()->getGuide_Line_StrokeStyle();


	D2D1_RECT_F rect = D2D1::RectF(
		p0._x, p0._y,
		p1._x, p1._y
	);

	ctx->getD2dRenderTarget()->DrawRectangle(
		&rect,
		_Guide_Line_Brush->getSolidColorBrush(),
		2.0f, //  getDesigeWidgetContext()->_Guide_Line_Style.get_width(),
		_Guide_Line_StrokeStyle->getStrokeStyle()
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




