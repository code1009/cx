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

	//o->_PressedState = _PressedState;
	//o->_PressedPoint = _PressedPoint;
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

void DesignWidget::registerEventHandler(WidgetDocument* doc)
{
	doc->getWidgetEventDispatcher()->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MousePressed,
		this,
		std::bind(&DesignWidget::onMousePressed, this, std::placeholders::_1)
	);

	doc->getWidgetEventDispatcher()->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseReleased,
		this,
		std::bind(&DesignWidget::onMouseReleased, this, std::placeholders::_1)
	);

	doc->getWidgetEventDispatcher()->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseDragging,
		this,
		std::bind(&DesignWidget::onMouseDragging, this, std::placeholders::_1)
	);
}

//===========================================================================
void DesignWidget::setWidgetDesignerModel(WidgetDesignerModel* widgetDesignerModel)
{
	_WidgetDesignerModel = widgetDesignerModel;
}

DesignWidgetContext* DesignWidget::getDesignWidgetContext(void) const
{
	return _WidgetDesignerModel->getDesignWidgetContext();
}

WidgetDesigner* DesignWidget::getWidgetDesinger(void) const
{
	return _WidgetDesignerModel->getWidgetDesigner();
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
	WidgetResource* _Marker_Fill_Brush      = getDesignWidgetContext()->getMarker_Fill_Brush     ();
	WidgetResource* _Marker_Line_Brush      = getDesignWidgetContext()->getMarker_Line_Brush     ();
	WidgetResource* _Marker_Text_Brush      = getDesignWidgetContext()->getMarker_Text_Brush     ();
	WidgetResource* _Marker_Text_TextFormat = getDesignWidgetContext()->getMarker_Text_TextFormat();

#if 0
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
		getDesignWidgetContext()->_Marker_LineStyle.getWidth()
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
		1.0f // getDesignWidgetContext()->_Marker_LineStyle.getWidth()
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
	WidgetResource* _Guide_Line_Brush       = getDesignWidgetContext()->getGuide_Line_Brush      ();
	WidgetResource* _Guide_Line_StrokeStyle = getDesignWidgetContext()->getGuide_Line_StrokeStyle();


	D2D1_RECT_F rect = D2D1::RectF(
		p0._x, p0._y,
		p1._x, p1._y
	);

	ctx->getD2dRenderTarget()->DrawRectangle(
		&rect,
		_Guide_Line_Brush->getSolidColorBrush(),
		2.0f, //  getDesignWidgetContext()->_Guide_LineStyle.get_width(),
		_Guide_Line_StrokeStyle->getStrokeStyle()
	);
}

//===========================================================================
void DesignWidget::moveMarker(const DesignWidgetMarkerId s, const Point& p)
{

}

DesignWidgetMarkerId DesignWidget::findMarker(const Point& test)
{
	return static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE);
}

void DesignWidget::onMousePressed(const WidgetMouseEventParam& param)
{
	// toSnappedPoint() 사용하면 안됨.
	//Point _MousePoint = getWidgetDesinger()->toSnappedPoint(param._MousePosition);
	Point _MousePoint = param._MousePosition;
	_PressedPoint = _MousePoint;


	_PressedState = findMarker(_MousePoint);
}

void DesignWidget::onMouseReleased(const WidgetMouseEventParam& param)
{
	Point _MousePoint = getWidgetDesinger()->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _PressedPoint;
	_PressedPoint = _MousePoint;


	if (static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE) != _PressedState)
	{
		//moveMarker(_PressedState, _MousePoint);

	}


	_PressedState = static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE);
}

void DesignWidget::onMouseDragging(const WidgetMouseEventParam& param)
{
	Point _MousePoint = getWidgetDesinger()->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _PressedPoint;
	_PressedPoint = _MousePoint;


	if (static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE) != _PressedState)
	{
		//moveMarker(_PressedState, _MousePoint);
	}
}

//===========================================================================
WidgetSharedPtr DesignWidget::getTargetWidget(void)
{
	return nullptr;
}

void DesignWidget::setTargetWidgetPropertyChanged(std::uint32_t code)
{
	setPropertyChanged(code);
}

void DesignWidget::onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param)
{
	setTargetWidgetPropertyChanged(param->_Code);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




