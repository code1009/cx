/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "ToolBox.hpp"

#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"

#include "ToolBox_WindowMessageHandler.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr std::uint64_t _MouseDbClickTime = 300u;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ToolBox::WindowMessageHandler::setItemView(ToolBox::ItemView* itemView)
{
	_ItemView = itemView;
}

//===========================================================================
bool ToolBox::WindowMessageHandler::isItemIn(ToolBox::ItemSharedPtr test)
{
	for(auto& item : _ItemView->getItems())
	{
		if (item == test)
		{
			return true;
		}
	}

	return false;
}

void ToolBox::WindowMessageHandler::reset(void)
{
	if (false==isItemIn(_MousePressedItem  )){_MousePressedItem  =nullptr;}
	if (false==isItemIn(_MouseReleasedItem )){_MouseReleasedItem =nullptr;}
	if (false==isItemIn(_MouseClickedItem  )){_MouseClickedItem  =nullptr;}
	if (false==isItemIn(_MouseDbclickedItem)){_MouseDbclickedItem=nullptr;}
	if (false==isItemIn(_MouseOverItem     )){_MouseOverItem     =nullptr;}
	if (false==isItemIn(_MouseDraggingItem )){_MouseDraggingItem =nullptr;}
}

//===========================================================================
bool ToolBox::WindowMessageHandler::getWindowMouseCaptureEnabled (void)
{
	return _WindowMouseCaptureEnabled;
}

void ToolBox::WindowMessageHandler::setWindowMouseCaptureEnabled (bool enabled)
{
	_WindowMouseCaptureEnabled = enabled;

	if (false==enabled)
	{
		releaseWindowMouseCapture();
	}
}

void ToolBox::WindowMessageHandler::setWindowMouseCapture(HWND hwnd)
{
	if ( getWindowMouseCaptureEnabled() )
	{
		_WindowMouseCaptured = true;
		::SetCapture(hwnd);
	}
}

void ToolBox::WindowMessageHandler::releaseWindowMouseCapture(void)
{
	if (_WindowMouseCaptured)
	{
		::ReleaseCapture();
		_WindowMouseCaptured = false;
	}		
}

void ToolBox::WindowMessageHandler::onWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;


	onWindowMouseMessage(hWnd, uMsg, wParam, lParam, handled);
	if(handled)
	{
		return;
	}
}

void ToolBox::WindowMessageHandler::onWindowMouseMessage (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled)
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
	_ItemView->getWindow()->getViewport()->WindowToDocument(_mouse_x, _mouse_y, _document_mouse_x, _document_mouse_y);


	//-----------------------------------------------------------------------
	ToolBox::EventType eventType;


	eventType = ToolBox::EventType::Unknown;


	//-----------------------------------------------------------------------
	ToolBox::MouseEventParam param;


	param._MousePosition._x    = static_cast<cx::gw::coord_t>(_document_mouse_x);
	param._MousePosition._y    = static_cast<cx::gw::coord_t>(_document_mouse_y);
	param._MouseLButtonPressed = fwKeys & MK_LBUTTON ? true : false;
	param._MouseRButtonPressed = fwKeys & MK_RBUTTON ? true : false;
	param._KeyShiftPressed     = fwKeys & MK_SHIFT   ? true : false;
	param._KeyCtrlPressed      = fwKeys & MK_CONTROL ? true : false;


	// 메세지 발생순서
	// WM_LBUTTONDOWN, WM_LBUTTONUP, WM_LBUTTONDBLCLK, and WM_LBUTTONUP
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		eventType = ToolBox::EventType::MouseMove;
		break;

	case WM_MOUSELEAVE:
		break;

	case WM_LBUTTONDBLCLK:
		break;

	case WM_LBUTTONDOWN:
		setWindowMouseCapture(hWnd);
		eventType = ToolBox::EventType::MouseLButtonDown;
		break;

	case WM_LBUTTONUP:
		releaseWindowMouseCapture();
		eventType = ToolBox::EventType::MouseLButtonUp;
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

	if (ToolBox::EventType::Unknown != eventType)
	{
		onMouse(eventType, param);

		handled = true;
	}
}

//===========================================================================
void ToolBox::WindowMessageHandler::onMouse(ToolBox::EventType eventType, ToolBox::MouseEventParam& param)
{
	switch (eventType)
	{
	case ToolBox::EventType::MouseMove:        onMouseMove       ( param ); break;
	case ToolBox::EventType::MouseLButtonDown: onMouseLButtonDown( param ); break;
	case ToolBox::EventType::MouseLButtonUp:   onMouseLButtonUp  ( param ); break;

	default:
		break;
	}
}

//===========================================================================
void ToolBox::WindowMessageHandler::onMouseMove (ToolBox::MouseEventParam& param)
{
	//-----------------------------------------------------------------------
	notifyMouseMove(param);


	//-----------------------------------------------------------------------
	ToolBox::ItemSharedPtr item;
	

	item = _ItemView->hitTest(param._MousePosition);
	

	//-----------------------------------------------------------------------
	if (_MousePressedItem)
	{
		if (_MouseDraggingItem == _MousePressedItem)
		{
			notifyMouseDragging(_MouseDraggingItem, param);
		}
		else
		{
			_MouseDraggingItem = nullptr;
		}
	}
	else
	{
		_MouseDraggingItem = nullptr;
	}


	//-----------------------------------------------------------------------
	if (_MouseOverItem != item)
	{
		if (_MouseOverItem)
		{
			notifyMouseLeave(_MouseOverItem, param);
		}

		_MouseOverItem = item;

		if (_MouseOverItem)
		{
			notifyMouseOver(_MouseOverItem, param);
		}
	}
}

void ToolBox::WindowMessageHandler::onMouseLButtonDown (ToolBox::MouseEventParam& param)
{
	//-----------------------------------------------------------------------
	notifyMouseLButtonDown(param);


	//-----------------------------------------------------------------------
	ToolBox::ItemSharedPtr item;

	item = _ItemView->hitTest(param._MousePosition);

	
	//-----------------------------------------------------------------------
	_MousePressedItem = item;


	if (_MousePressedItem)
	{
		notifyMousePressed(_MousePressedItem, param);
	}


	_MouseDraggingItem = _MousePressedItem;
}

void ToolBox::WindowMessageHandler::onMouseLButtonUp (ToolBox::MouseEventParam& param)
{
	//-----------------------------------------------------------------------
	_MouseDraggingItem = nullptr;


	//-----------------------------------------------------------------------
	ToolBox::ItemSharedPtr item;


	item = _ItemView->hitTest(param._MousePosition);
	

	//-----------------------------------------------------------------------
	_MouseReleasedItem = item;

	
	if (_MousePressedItem)
	{
		notifyMouseReleased(_MousePressedItem, param);
	}


	/*
	if (_MousePressedItem != _MouseReleasedItem)
	{
		if (_MouseReleasedItem)
		{
			{
				notify_widget_mouse_context(_MouseReleasedItem, param);
			}
		}
	}
	*/


	if (_MousePressedItem == _MouseReleasedItem)
	{
		_MouseClickedItem = _MouseReleasedItem;
		if (_MouseClickedItem)
		{
			notifyMouseClicked(_MouseClickedItem, param);
		}
	}


	if (_MouseDbclickedItem==_MouseClickedItem)
	{
		if (_MouseDbclickedItem)
		{
			if (GetTickCount64() < _MouseClickedTime + _MouseDbClickTime)
			{
				notifyMouseDbClicked(_MouseDbclickedItem, param);
			}
		}
	}


	if (nullptr!=_MouseClickedItem)
	{
		_MouseClickedTime = GetTickCount64();
	}


	_MouseDbclickedItem = _MouseClickedItem;


	//-----------------------------------------------------------------------
	notifyMouseLButtonUp(param);
}

//===========================================================================
void ToolBox::WindowMessageHandler::notifyMouseMove (ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseMove,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseLButtonDown (ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseLButtonDown,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseLButtonUp (ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseLButtonUp,
		param
	);
}

//===========================================================================
void ToolBox::WindowMessageHandler::notifyMousePressed (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MousePressed,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseReleased (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseReleased,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseClicked (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseClicked,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseDbClicked (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseDbClicked,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseOver (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseOver,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseLeave (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseLeave,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseDragging (ToolBox::ItemSharedPtr item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseDragging,
		item,
		param
	);
}
