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
WidgetDesignerMouseTool::WidgetDesignerMouseTool(WidgetDesigner* widgetDesigner):
	_WidgetDesigner{ widgetDesigner }
{
	registerEventHandler();
}

//===========================================================================
void WidgetDesignerMouseTool::getSelectBounds(Point& p0, Point& p1)
{
	p0 = _p0;
	p1 = _p1;
}

//===========================================================================
void WidgetDesignerMouseTool::registerEventHandler(void)
{
	//-----------------------------------------------------------------------
	WidgetEventDispatcher* widgetEventDispatcher;
	
	
	widgetEventDispatcher = 
		_WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->getWidgetEventDispatcher();


	//-----------------------------------------------------------------------
	widgetEventDispatcher->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseMove,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseMove, this,std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLButtonDown,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseLButtonDown, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLButtonUp,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseLButtonUp, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDragEnterEventParam&>(
		WidgetEventType::MouseDragEnter,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseDragEnter, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDragOverEventParam&>(
		WidgetEventType::MouseDragOver,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseDragOver, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDragLeaveEventParam&>(
		WidgetEventType::MouseDragLeave,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseDragLeave, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDropEventParam&>(
		WidgetEventType::MouseDrop,
		nullptr,
		std::bind(&WidgetDesignerMouseTool::onMouseDrop, this, std::placeholders::_1)
	);
}

//===========================================================================
void WidgetDesignerMouseTool::onMouseMove(const WidgetMouseEventParam& param)
{
	_p1 = param._MousePosition;


	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _MousePressedPoint;
	_MousePressedPoint = _MousePoint;


	if (_Action_SelectSingle)
	{
		_WidgetDesigner->moveSelectedWidgets(offset);
	}
	if (_Action_MoveSelection)
	{
		_WidgetDesigner->moveSelectedWidgets(offset);
	}
}

void WidgetDesignerMouseTool::onMouseLButtonDown(const WidgetMouseEventParam& param)
{
	_p0 = param._MousePosition;
	_p1 = param._MousePosition;


	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	_MousePressedPoint = _MousePoint;


	_Action_SelectMultiple = false;
	_Action_SelectSingle = false;
	_Action_ToggleSelection = false;
	_Action_MoveSelection = false;


	auto widget = _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->find(param._MousePosition);
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);


	bool designMarkerPressed = false;
	if (designWidget)
	{
		if (designWidget->isPointInMarkers(param._MousePosition))
		{
			designMarkerPressed = true;
		}
	}
	if (designMarkerPressed)
	{
		return;
	}


	if ((!param._ShiftKeyPressed) && (!param._CtrlKeyPressed))
	{
		if (nullptr == designWidget)
		{
			_Action_SelectMultiple = true;
		}
		else
		{
			if (_WidgetDesigner->isSelectedWidget(widget))
			{
				_Action_MoveSelection = true;
			}
			else
			{
				_Action_SelectSingle = true;
			}
		}
	}
	else if ((param._ShiftKeyPressed) && (!param._CtrlKeyPressed))
	{
		_Action_SelectMultiple = true;
	}
	else if ((!param._ShiftKeyPressed) && (param._CtrlKeyPressed))
	{
		if (nullptr != designWidget)
		{
			_Action_ToggleSelection = true;
		}
	}


	if (_Action_SelectSingle)
	{
		_WidgetDesigner->selectWidget(widget);
	}
	if (_Action_ToggleSelection)
	{
		_WidgetDesigner->toggleWidgetSelection(widget);
	}
}

void WidgetDesignerMouseTool::onMouseLButtonUp(const WidgetMouseEventParam& param)
{
	_p1 = param._MousePosition;


	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _MousePressedPoint;
	_MousePressedPoint = _MousePoint;


	if (_Action_SelectMultiple)
	{
		_WidgetDesigner->selectWidgetsInBounds();
	}
	if (_Action_SelectSingle)
	{
		_WidgetDesigner->moveSelectedWidgets(offset);
	}
	if (_Action_MoveSelection)
	{
		_WidgetDesigner->moveSelectedWidgets(offset);
	}


	_Action_SelectMultiple = false;
	_Action_SelectSingle = false;
	_Action_ToggleSelection = false;
	_Action_MoveSelection = false;


	_MousePressedPoint.zero();
	_p0.zero();
	_p1.zero();
}

//===========================================================================
void WidgetDesignerMouseTool::onMouseDragEnter(const WidgetMouseDragEnterEventParam& param)
{
	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	_NewWidget_Point = _MousePoint;


	WidgetSharedPtr widget;
	widget = _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->getWidgetFactory()->find(param._String);
	if (nullptr == widget)
	{
		return;
	}

	_NewWidget = widget->clone();
	if (nullptr == _NewWidget)
	{
		return;
	}


	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_NewWidget);
	if (designWidget)
	{
		designWidget->setWidgetDesignerModel(_WidgetDesigner->getWidgetDesignerModel());
	}


	_NewWidget->moveOffset(_NewWidget_Point);
	_WidgetDesigner->getWidgetDesignerModel()->getWindow()->render();
}

void WidgetDesignerMouseTool::onMouseDragOver(const WidgetMouseDragOverEventParam& param)
{
	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _NewWidget_Point;
	_NewWidget_Point = _MousePoint;

	
	if (nullptr == _NewWidget)
	{
		return;
	}


	_NewWidget->moveOffset(offset);
	_WidgetDesigner->getWidgetDesignerModel()->getWindow()->render();
}

void WidgetDesignerMouseTool::onMouseDragLeave(const WidgetMouseDragLeaveEventParam& param)
{
	if (nullptr == _NewWidget)
	{
		return;
	}


	_NewWidget = nullptr;
	_WidgetDesigner->getWidgetDesignerModel()->getWindow()->render();
}

void WidgetDesignerMouseTool::onMouseDrop(const WidgetMouseDropEventParam& param)
{
	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _NewWidget_Point;
	_NewWidget_Point = _MousePoint;


	if (nullptr == _NewWidget)
	{
		return;
	}

	/*
	_NewWidget->moveOffset(offset);
	_WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->addWidget(_NewWidget);
	_WidgetDesigner->selectWidget(_NewWidget);
	*/
	_NewWidget->moveOffset(offset);
	_WidgetDesigner->addWidget(_NewWidget);
	_WidgetDesigner->selectWidget(_NewWidget);


	_NewWidget = nullptr;
	_WidgetDesigner->getWidgetDesignerModel()->getWindow()->render();
}

//===========================================================================
bool WidgetDesignerMouseTool::loadResources(WidgetResourceMap* widgetResourceMap)
{
	_Select_Fill_Brush = widgetResourceMap->getFill_SolidColorBrush(&_Select_FillStyle);
	if (nullptr == _Select_Fill_Brush)
	{
		return false;
	}
	_Select_Line_Brush = widgetResourceMap->getLine_SolidColorBrush(&_Select_LineStyle);
	if (nullptr == _Select_Line_Brush)
	{
		return false;
	}
	_Select_Text_Brush = widgetResourceMap->getText_SolidColorBrush(&_Select_TextStyle);
	if (nullptr == _Select_Text_Brush)
	{
		return false;
	}
	_Select_Text_TextFormat = widgetResourceMap->getText_TextFormat(&_Select_TextStyle);
	if (nullptr == _Select_Text_TextFormat)
	{
		return false;
	}

	return true;
}

//===========================================================================
void WidgetDesignerMouseTool::draw(Context* ctx)
{
	if (_Action_SelectMultiple)
	{
		drawSelectBounds(ctx);
	}

	if (_NewWidget.get())
	{
		_NewWidget->draw(ctx);
	}
}

void WidgetDesignerMouseTool::drawSelectBounds(Context* ctx)
{
	//-----------------------------------------------------------------------
	Point p0;
	Point p1;
	calculateBounds(_p0, _p1, p0, p1);


	//-----------------------------------------------------------------------
	D2D1_RECT_F rect = D2D1::RectF(
		p0._x, p0._y,
		p1._x, p1._y
	);


	//-----------------------------------------------------------------------
	ctx->getD2dRenderTarget()->FillRectangle(
		&rect,
		_Select_Fill_Brush->getSolidColorBrush()
	);
	ctx->getD2dRenderTarget()->DrawRectangle(
		&rect,
		_Select_Line_Brush->getSolidColorBrush(),
		_Select_LineStyle.getWidth()
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




