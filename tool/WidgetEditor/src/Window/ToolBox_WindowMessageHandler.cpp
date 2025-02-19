/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "ToolBox.hpp"
#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_WindowMessageHandler.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ToolBox::WindowMessageHandler::setItemView(ToolBox::ItemView* itemView)
{
	_ItemView = itemView;
}

//===========================================================================
void ToolBox::WindowMessageHandler::reset(void)
{
	if (false==isIn(_Item_MousePressed  )){_Item_MousePressed  =nullptr;}
	if (false==isIn(_Item_MouseReleased )){_Item_MouseReleased =nullptr;}
	if (false==isIn(_Item_MouseClicked  )){_Item_MouseClicked  =nullptr;}
	if (false==isIn(_Item_MouseDbClicked)){_Item_MouseDbClicked=nullptr;}
	if (false==isIn(_Item_MouseOver     )){_Item_MouseOver     =nullptr;}
	if (false==isIn(_Item_MouseDragging )){_Item_MouseDragging =nullptr;}
}

bool ToolBox::WindowMessageHandler::isIn(ToolBox::Item* test)
{
	for (auto& item : _ItemView->getItems())
	{
		if (item.get() == test)
		{
			return true;
		}
	}

	return false;
}

//===========================================================================
ToolBox::Item* ToolBox::WindowMessageHandler::hitTest(const cx::gw::Point& point)
{
	return hitTest(point, _ItemView->getItems());
}

ToolBox::Item* ToolBox::WindowMessageHandler::hitTest(const cx::gw::Point& point, ToolBox::ItemSharedPtrs& items)
{
	for (auto& item : items)
	{
		auto found = hitTest(point, item);
		if (found)
		{
			return found;
		}

		if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
		{
			if (!groupItem->getSubItems().empty())
			{
				if (!groupItem->isCollapseSubItems())
				{
					auto found = hitTest(point, groupItem->getSubItems());
					if (found)
					{
						return found;
					}
				}
			}
		}
	}

	return nullptr;
}

ToolBox::Item* ToolBox::WindowMessageHandler::hitTest(const cx::gw::Point& point, ToolBox::ItemSharedPtr& item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	auto itemDrawing = _ItemView->getItemDrawing(item);
	itemDrawing->getFace_Bounds(item.get(), p0, p1);


	auto rv = cx::gw::isPointInBounds(p0, p1, point);
	if (rv)
	{
		return item.get();
	}

	return nullptr;
}

//===========================================================================
void ToolBox::WindowMessageHandler::getMouseDbClickTime(std::uint64_t& time)
{
	time = _MouseDbClickTime;
}

void ToolBox::WindowMessageHandler::setMouseDbClickTime(std::uint64_t time)
{
	_MouseDbClickTime = time;
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

//===========================================================================
bool ToolBox::WindowMessageHandler::onWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	onWindowMouseMessage(hWnd, uMsg, wParam, lParam, handled);
	if(handled)
	{
		return true;
	}

	return false;
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
	param._ShiftKeyPressed     = fwKeys & MK_SHIFT   ? true : false;
	param._CtrlKeyPressed      = fwKeys & MK_CONTROL ? true : false;


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
	ToolBox::Item* item;
	item = hitTest(param._MousePosition);
	

	//-----------------------------------------------------------------------
	if (_Item_MousePressed)
	{
		if (_Item_MouseDragging == _Item_MousePressed)
		{
			notifyMouseDragging(_Item_MouseDragging, param);
		}
		else
		{
			_Item_MouseDragging = nullptr;
		}
	}
	else
	{
		_Item_MouseDragging = nullptr;
	}


	//-----------------------------------------------------------------------
	if (_Item_MouseOver != item)
	{
		if (_Item_MouseOver)
		{
			notifyMouseLeave(_Item_MouseOver, param);
		}

		_Item_MouseOver = item;

		if (_Item_MouseOver)
		{
			notifyMouseOver(_Item_MouseOver, param);
		}
	}
}

void ToolBox::WindowMessageHandler::onMouseLButtonDown (ToolBox::MouseEventParam& param)
{
	//-----------------------------------------------------------------------
	notifyMouseLButtonDown(param);


	//-----------------------------------------------------------------------
	ToolBox::Item* item;
	item = hitTest(param._MousePosition);

	
	//-----------------------------------------------------------------------
	_Item_MousePressed = item;


	if (_Item_MousePressed)
	{
		notifyMousePressed(_Item_MousePressed, param);
	}


	_Item_MouseDragging = _Item_MousePressed;
}

void ToolBox::WindowMessageHandler::onMouseLButtonUp (ToolBox::MouseEventParam& param)
{
	//-----------------------------------------------------------------------
	_Item_MouseDragging = nullptr;


	//-----------------------------------------------------------------------
	ToolBox::Item* item;
	item = hitTest(param._MousePosition);
	

	//-----------------------------------------------------------------------
	_Item_MouseReleased = item;

	
	if (_Item_MousePressed)
	{
		notifyMouseReleased(_Item_MousePressed, param);
	}


	/*
	if (_Item_MousePressed != _Item_MouseReleased)
	{
		if (_Item_MouseReleased)
		{
			{
				notifyMouseContext(_Item_MouseReleased, param);
			}
		}
	}
	*/


	if (_Item_MousePressed == _Item_MouseReleased)
	{
		_Item_MouseClicked = _Item_MouseReleased;
		if (_Item_MouseClicked)
		{
			notifyMouseClicked(_Item_MouseClicked, param);
		}
	}


	if (_Item_MouseDbClicked==_Item_MouseClicked)
	{
		if (_Item_MouseDbClicked)
		{
			if (GetTickCount64() < _MouseClickedTime + _MouseDbClickTime)
			{
				notifyMouseDbClicked(_Item_MouseDbClicked, param);
			}
		}
	}


	if (nullptr!=_Item_MouseClicked)
	{
		_MouseClickedTime = GetTickCount64();
	}


	_Item_MouseDbClicked = _Item_MouseClicked;


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
void ToolBox::WindowMessageHandler::notifyMousePressed (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MousePressed,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseReleased (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseReleased,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseClicked (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseClicked,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseDbClicked (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseDbClicked,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseOver (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseOver,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseLeave (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseLeave,
		item,
		param
	);
}

void ToolBox::WindowMessageHandler::notifyMouseDragging (ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	_ItemView->onMouseEvent(
		ToolBox::EventType::MouseDragging,
		item,
		param
	);

	if (param._DragDrop)
	{
		onMouseLButtonUp(param);
	}
}
