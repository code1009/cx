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
WidgetDesignerTool::WidgetDesignerTool(WidgetDesigner* widgetDesigner):
	_WidgetDesigner{ widgetDesigner }
{
	registerEventHandler();
}

//===========================================================================
void WidgetDesignerTool::getSelectBounds(Point& p0, Point& p1)
{
	p0 = _p0;
	p1 = _p1;
}

//===========================================================================
void WidgetDesignerTool::registerEventHandler(void)
{
	//-----------------------------------------------------------------------
	WidgetEventDispatcher* widgetEventDispatcher;
	
	
	widgetEventDispatcher = 
		_WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->getWidgetEventDispatcher();


	//-----------------------------------------------------------------------
	widgetEventDispatcher->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseMove,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseMove, this,std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLButtonDown,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseLButtonDown, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLButtonUp,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseLButtonUp, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDragEnterEventParam&>(
		WidgetEventType::MouseDragEnter,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseDragEnter, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDragOverEventParam&>(
		WidgetEventType::MouseDragOver,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseDragOver, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDragLeaveEventParam&>(
		WidgetEventType::MouseDragLeave,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseDragLeave, this, std::placeholders::_1)
	);
	widgetEventDispatcher->registerEventHandler<const WidgetMouseDropEventParam&>(
		WidgetEventType::MouseDrop,
		nullptr,
		std::bind(&WidgetDesignerTool::onMouseDrop, this, std::placeholders::_1)
	);
}

void WidgetDesignerTool::onMouseMove(const WidgetMouseEventParam& param)
{
	_p1 = param._MousePosition;


	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _MousePressedPoint;
	_MousePressedPoint = _MousePoint;


	if (_Action_SingleSelection)
	{
		_WidgetDesigner->moveSelection(offset);
	}
	if (_Action_Move)
	{
		_WidgetDesigner->moveSelection(offset);
	}
}

void WidgetDesignerTool::onMouseLButtonDown(const WidgetMouseEventParam& param)
{
	_p0 = param._MousePosition;
	_p1 = param._MousePosition;


	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	_MousePressedPoint = _MousePoint;


	_Action_MultipleSelection = false;
	_Action_SingleSelection = false;
	_Action_ToggleSelection = false;
	_Action_Move = false;


	auto widget = _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->find(param._MousePosition);
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);


	bool anchor_marker_pressed = false;
	if (designWidget)
	{
		if (designWidget->isPointInMarkers(param._MousePosition))
		{
			anchor_marker_pressed = true;
		}
	}
	if (anchor_marker_pressed)
	{
		return;
	}


	if ((!param._ShiftKeyPressed) && (!param._CtrlKeyPressed))
	{
		if (nullptr == designWidget)
		{
			_Action_MultipleSelection = true;
		}
		else
		{
			if (_WidgetDesigner->isSelected(widget))
			{
				_Action_Move = true;
			}
			else
			{
				_Action_SingleSelection = true;
			}
		}
	}
	else if ((param._ShiftKeyPressed) && (!param._CtrlKeyPressed))
	{
		_Action_MultipleSelection = true;
	}
	else if ((!param._ShiftKeyPressed) && (param._CtrlKeyPressed))
	{
		if (nullptr != designWidget)
		{
			_Action_ToggleSelection = true;
		}
	}


	if (_Action_SingleSelection)
	{
		_WidgetDesigner->selectSingle(widget);
	}
	if (_Action_ToggleSelection)
	{
		_WidgetDesigner->toggleSelection(widget);
	}
}

void WidgetDesignerTool::onMouseLButtonUp(const WidgetMouseEventParam& param)
{
	_p1 = param._MousePosition;


	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _MousePressedPoint;
	_MousePressedPoint = _MousePoint;


	if (_Action_MultipleSelection)
	{
		_WidgetDesigner->selectBounds();
	}
	if (_Action_SingleSelection)
	{
		_WidgetDesigner->moveSelection(offset);
	}
	if (_Action_Move)
	{
		_WidgetDesigner->moveSelection(offset);
	}


	_Action_MultipleSelection = false;
	_Action_SingleSelection = false;
	_Action_ToggleSelection = false;
	_Action_Move = false;


	_MousePressedPoint.zero();
	_p0.zero();
	_p1.zero();
}

void WidgetDesignerTool::onMouseDragEnter(const WidgetMouseDragEnterEventParam& param)
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
	if (_NewWidget)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_NewWidget);
		if (designWidget)
		{
			designWidget->setWidgetDesignerModel(_WidgetDesigner->getWidgetDesignerModel());
		}


		_NewWidget->moveOffset(_NewWidget_Point);
	}
}

void WidgetDesignerTool::onMouseDragOver(const WidgetMouseDragOverEventParam& param)
{
	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _NewWidget_Point;
	_NewWidget_Point = _MousePoint;

	
	if (_NewWidget)
	{
		_NewWidget->moveOffset(offset);
	}
}

void WidgetDesignerTool::onMouseDragLeave(const WidgetMouseDragLeaveEventParam& param)
{
	_NewWidget = nullptr;
}

void WidgetDesignerTool::onMouseDrop(const WidgetMouseDropEventParam& param)
{
	Point _MousePoint = _WidgetDesigner->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _NewWidget_Point;
	_NewWidget_Point = _MousePoint;


	if (_NewWidget)
	{
		_NewWidget->moveOffset(offset);

		_WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->addWidget(_NewWidget);

		_WidgetDesigner->selectSingle(_NewWidget);
	}


	_NewWidget = nullptr;
}

//===========================================================================
bool WidgetDesignerTool::loadResources(WidgetResourceMap* widgetResourceMap)
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
void WidgetDesignerTool::draw(Context* ctx)
{
	if (_Action_MultipleSelection)
	{
		drawSelectBounds(ctx);
	}

	if (_NewWidget.get())
	{
		_NewWidget->draw(ctx);
	}
}

void WidgetDesignerTool::drawSelectBounds(Context* ctx)
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




