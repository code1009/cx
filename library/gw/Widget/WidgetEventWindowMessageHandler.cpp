/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"


//===========================================================================
#include "../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void WidgetEventWindowMessageHandler::setWidgetDocument(WidgetDocument* doc)
{
	_WidgetDocument = doc;
}

void WidgetEventWindowMessageHandler::setViewport(Viewport* viewport)
{
	_Viewport = viewport;
}

//===========================================================================
void WidgetEventWindowMessageHandler::reset(void)
{
	if (false==isIn(_Widget_MousePressed  )){_Widget_MousePressed  =nullptr;}
	if (false==isIn(_Widget_MouseReleased )){_Widget_MouseReleased =nullptr;}
	if (false==isIn(_Widget_MouseClicked  )){_Widget_MouseClicked  =nullptr;}
	if (false==isIn(_Widget_MouseDbClicked)){_Widget_MouseDbClicked=nullptr;}
	if (false==isIn(_Widget_MouseOver     )){_Widget_MouseOver     =nullptr;}
	if (false==isIn(_Widget_MouseDragging )){_Widget_MouseDragging =nullptr;}
}

bool WidgetEventWindowMessageHandler::isIn(Widget* test)
{
	for (auto& widget: _WidgetDocument->_Widgets)
	{
		if (widget.get() == test)
		{
			return true;
		}
	}

	return false;
}

//===========================================================================
Widget* WidgetEventWindowMessageHandler::hitTest(const Point& point)
{
	WidgetSharedPtr widget;
	widget = _WidgetDocument->find(point);
	return widget.get();
}

//===========================================================================
void WidgetEventWindowMessageHandler::getMouseDbClickTime(std::uint64_t& time)
{
	time = _MouseDbClickTime;
}

void WidgetEventWindowMessageHandler::setMouseDbClickTime(std::uint64_t time)
{
	_MouseDbClickTime = time;
}

//===========================================================================
bool WidgetEventWindowMessageHandler::getWindowMouseCaptureEnabled (void)
{
	return _WindowMouseCaptureEnabled;
}

void WidgetEventWindowMessageHandler::setWindowMouseCaptureEnabled (bool enabled)
{
	_WindowMouseCaptureEnabled = enabled;
	if (false==enabled)
	{
		releaseWindowMouseCapture();
	}
}

void WidgetEventWindowMessageHandler::setWindowMouseCapture(HWND hwnd)
{
	if ( getWindowMouseCaptureEnabled() )
	{
		_WindowMouseCaptured = true;
		::SetCapture(hwnd);
	}
}

void WidgetEventWindowMessageHandler::releaseWindowMouseCapture(void)
{
	if (_WindowMouseCaptured)
	{
		::ReleaseCapture();
		_WindowMouseCaptured = false;
	}		
}

//===========================================================================
bool WidgetEventWindowMessageHandler::onWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	onWindowMouseMessage(hWnd, uMsg, wParam, lParam, handled);
	if(handled)
	{
		return true;
	}

	return false;
}

void WidgetEventWindowMessageHandler::onWindowMouseMessage (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled)
{
	//-----------------------------------------------------------------------
	UINT fwKeys  = GET_KEYSTATE_WPARAM(wParam);
	SHORT zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);


	/*
	switch (fwKeys)
	{
	case MK_LBUTTON:
		break;
	case MK_RBUTTON:
		break;
	case MK_MBUTTON:
		break;
	case MK_SHIFT:
		break;
	case MK_CONTROL:
		break;
	case MK_XBUTTON1:
		break;
	case MK_XBUTTON2:
		break;
	}
	*/


	//-----------------------------------------------------------------------
	double _document_mouse_x;
	double _document_mouse_y;
	std::int64_t _mouse_x;
	std::int64_t _mouse_y;


	_mouse_x = x;
	_mouse_y = y;
	_Viewport->WindowToDocument(_mouse_x, _mouse_y, _document_mouse_x, _document_mouse_y);


	//-----------------------------------------------------------------------
	WidgetEventType eventType;


	eventType = WidgetEventType::Unknown;


	//-----------------------------------------------------------------------
	WidgetMouseEventParam param;


	param._MousePosition._x    = static_cast<coord_t>(_document_mouse_x);
	param._MousePosition._y    = static_cast<coord_t>(_document_mouse_y);
	param._MouseLButtonPressed = fwKeys & MK_LBUTTON ? true : false;
	param._MouseRButtonPressed = fwKeys & MK_RBUTTON ? true : false;
	param._ShiftKeyPressed     = fwKeys & MK_SHIFT   ? true : false;
	param._CtrlKeyPressed      = fwKeys & MK_CONTROL ? true : false;


	// 메세지 발생순서
	// WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		eventType = WidgetEventType::MouseMove;
		break;

	case WM_MOUSELEAVE:
		break;

	case WM_LBUTTONDBLCLK:
		break;

	case WM_LBUTTONDOWN:
		setWindowMouseCapture(hWnd);
		eventType = WidgetEventType::MouseLButtonDown;
		break;

	case WM_LBUTTONUP:
		releaseWindowMouseCapture();
		eventType = WidgetEventType::MouseLButtonUp;
		break;

	case WM_RBUTTONDBLCLK:
		break;

	case WM_RBUTTONDOWN:
		break;

	case WM_RBUTTONUP:
		break;

		//-----------------------------------------------------------------------
	case WM_MOUSEWHEEL:
		break;

	default:
		break;
	}

	if (WidgetEventType::Unknown != eventType)
	{
		onMouse(eventType, param);

		handled = true;
	}
}

//===========================================================================
void WidgetEventWindowMessageHandler::onMouse(WidgetEventType eventType, WidgetMouseEventParam& param)
{
	switch (eventType)
	{
	case WidgetEventType::MouseMove:        onMouseMove       ( param ); break;
	case WidgetEventType::MouseLButtonDown: onMouseLButtonDown( param ); break;
	case WidgetEventType::MouseLButtonUp:   onMouseLButtonUp  ( param ); break;

	default:
		break;
	}
}

//===========================================================================
void WidgetEventWindowMessageHandler::onMouseMove (WidgetMouseEventParam& param)
{
	//-----------------------------------------------------------------------
	notifyMouseMove(param);


	//-----------------------------------------------------------------------
	Widget* widget;
	widget = hitTest(param._MousePosition);
	

	//-----------------------------------------------------------------------
	if (_Widget_MousePressed)
	{
		if (_Widget_MouseDragging == _Widget_MousePressed)
		{
			notifyMouseDragging(_Widget_MouseDragging, param);
		}
		else
		{
			_Widget_MouseDragging = nullptr;
		}
	}
	else
	{
		_Widget_MouseDragging = nullptr;
	}


	//-----------------------------------------------------------------------
	if (_Widget_MouseOver != widget)
	{
		if (_Widget_MouseOver)
		{
			notifyMouseLeave(_Widget_MouseOver, param);
		}

		_Widget_MouseOver = widget;

		if (_Widget_MouseOver)
		{
			notifyMouseOver(_Widget_MouseOver, param);
		}
	}
}

void WidgetEventWindowMessageHandler::onMouseLButtonDown (WidgetMouseEventParam& param)
{
	//-----------------------------------------------------------------------
	notifyMouseLButtonDown(param);


	//-----------------------------------------------------------------------
	Widget* widget;
	widget = hitTest(param._MousePosition);

	
	//-----------------------------------------------------------------------
	_Widget_MousePressed = widget;


	if (_Widget_MousePressed)
	{
		notifyMousePressed(_Widget_MousePressed, param);
	}


	_Widget_MouseDragging = _Widget_MousePressed;
}

void WidgetEventWindowMessageHandler::onMouseLButtonUp (WidgetMouseEventParam& param)
{
	//-----------------------------------------------------------------------
	_Widget_MouseDragging = nullptr;


	//-----------------------------------------------------------------------
	Widget* widget;
	widget = hitTest(param._MousePosition);
	

	//-----------------------------------------------------------------------
	_Widget_MouseReleased = widget;

	
	if (_Widget_MousePressed)
	{
		notifyMouseReleased(_Widget_MousePressed, param);
	}


	/*
	if (_Widget_MousePressed != _Widget_MouseReleased)
	{
		if (_Widget_MouseReleased)
		{
			{
				notifyMouseContext(_Widget_MouseReleased, param);
			}
		}
	}
	*/


	if (_Widget_MousePressed == _Widget_MouseReleased)
	{
		_Widget_MouseClicked = _Widget_MouseReleased;
		if (_Widget_MouseClicked)
		{
			notifyMouseClicked(_Widget_MouseClicked, param);
		}
	}


	if (_Widget_MouseDbClicked==_Widget_MouseClicked)
	{
		if (_Widget_MouseDbClicked)
		{
			if (GetTickCount64() < _MouseClickedTime + _MouseDbClickTime)
			{
				notifyMouseDbClicked(_Widget_MouseDbClicked, param);
			}
		}
	}


	if (nullptr!=_Widget_MouseClicked)
	{
		_MouseClickedTime = GetTickCount64();
	}


	_Widget_MouseDbClicked = _Widget_MouseClicked;


	//-----------------------------------------------------------------------
	notifyMouseLButtonUp(param);
}

//===========================================================================
void WidgetEventWindowMessageHandler::notifyMouseMove (WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseMove,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseLButtonDown (WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLButtonDown,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseLButtonUp (WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLButtonUp,
		param
	);
}

//===========================================================================
void WidgetEventWindowMessageHandler::notifyMousePressed (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MousePressed,
		widget,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseReleased (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseReleased,
		widget,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseClicked (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseClicked,
		widget,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseDbClicked (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseDbClicked,
		widget,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseOver (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseOver,
		widget,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseLeave (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseLeave,
		widget,
		param
	);
}

void WidgetEventWindowMessageHandler::notifyMouseDragging (Widget* widget, WidgetMouseEventParam& param)
{
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseEventParam&>(
		WidgetEventType::MouseDragging,
		widget,
		param
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



