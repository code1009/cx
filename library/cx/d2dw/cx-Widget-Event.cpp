/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	EventGenerator::EventGenerator(View& view, bool visibleItemOnly) :
		_View(view),
		_VisibleItemOnly(visibleItemOnly)
	{
	}

	//=======================================================================
	bool EventGenerator::contains(std::shared_ptr<Item> test)
	{
		return _View.model().contains(test);
	}

	std::shared_ptr<Item> EventGenerator::hitTest(const Point& point)
	{
		if (!_VisibleItemOnly)
		{
			return _View.model().find(point);
		}
		return _View.model().findIfVisible(point);
	}

	void EventGenerator::update(void)
	{
		if (false == contains(_Item_PointerPressed)) { _Item_PointerPressed = nullptr; }
		if (false == contains(_Item_PointerReleased)) { _Item_PointerReleased = nullptr; }
		if (false == contains(_Item_PointerClicked)) { _Item_PointerClicked = nullptr; }
		if (false == contains(_Item_PointerDbClicked)) { _Item_PointerDbClicked = nullptr; }
		if (false == contains(_Item_PointerOver)) { _Item_PointerOver = nullptr; }
		if (false == contains(_Item_PointerDragging)) { _Item_PointerDragging = nullptr; }
	}

	//=======================================================================
	void EventGenerator::pointerPressed(Coord const px, Coord const py, bool controlKeyPressed, bool shiftKeyPressed)
	{
		//-------------------------------------------------------------------
		capturePointer(px, py);
		setCurrentPointerPosition(px, py);
		_View.viewContext().pointerCurrentPosition(pointerCurrentPosition());


		//-------------------------------------------------------------------
		PointerEventData pointerEventData;
		pointerEventData._PointerPosition = _View.viewContext().fromWindow(Point(px, py));
		pointerEventData._CtrlKeyPressed = controlKeyPressed;
		pointerEventData._ShiftKeyPressed = shiftKeyPressed;


		//-------------------------------------------------------------------
		notifyPointerLButtonDown(pointerEventData);


		//-------------------------------------------------------------------
		std::shared_ptr<Item> item;
		item = hitTest(pointerEventData._PointerPosition);


		//-------------------------------------------------------------------
		_Item_PointerPressed = item;


		if (_Item_PointerPressed)
		{
			notifyItemPointerPressed(_Item_PointerPressed, pointerEventData);
		}


		_Item_PointerDragging = _Item_PointerPressed;
	}

	void EventGenerator::pointerReleased(Coord const px, Coord const py, bool controlKeyPressed, bool shiftKeyPressed)
	{
		//-------------------------------------------------------------------
		releasePointer(px, py);
		setCurrentPointerPosition(px, py);
		_View.viewContext().pointerCurrentPosition(pointerCurrentPosition());


		//-------------------------------------------------------------------
		PointerEventData pointerEventData;
		pointerEventData._PointerPosition = _View.viewContext().fromWindow(Point(px, py));
		pointerEventData._CtrlKeyPressed = controlKeyPressed;
		pointerEventData._ShiftKeyPressed = shiftKeyPressed;


		//-------------------------------------------------------------------
		_Item_PointerDragging = nullptr;


		//-------------------------------------------------------------------
		std::shared_ptr<Item> item;
		item = hitTest(pointerEventData._PointerPosition);


		//-------------------------------------------------------------------
		_Item_PointerReleased = item;


		if (_Item_PointerPressed)
		{
			notifyItemPointerReleased(_Item_PointerPressed, pointerEventData);
		}


		if (_Item_PointerPressed == _Item_PointerReleased)
		{
			_Item_PointerClicked = _Item_PointerReleased;
			if (_Item_PointerClicked)
			{
				notifyItemPointerClicked(_Item_PointerClicked, pointerEventData);
			}
		}


		if (_Item_PointerDbClicked == _Item_PointerClicked)
		{
			if (_Item_PointerDbClicked)
			{
				if (GetTickCount64() < _PointerClickedTime + _PointerDbClickTime)
				{
					notifyItemPointerDbClicked(_Item_PointerDbClicked, pointerEventData);
				}
			}
		}
		if (nullptr != _Item_PointerClicked)
		{
			_PointerClickedTime = GetTickCount64();
		}
		_Item_PointerDbClicked = _Item_PointerClicked;


		//-------------------------------------------------------------------
		notifyPointerLButtonUp(pointerEventData);
	}

	void EventGenerator::pointerMoved(Coord const px, Coord const py, bool controlKeyPressed, bool shiftKeyPressed)
	{
		//-------------------------------------------------------------------
		setCurrentPointerPosition(px, py);
		_View.viewContext().pointerCurrentPosition(pointerCurrentPosition());


		//-------------------------------------------------------------------
		PointerEventData pointerEventData;
		pointerEventData._PointerPosition = _View.viewContext().fromWindow(Point(px, py));
		pointerEventData._CtrlKeyPressed = controlKeyPressed;
		pointerEventData._ShiftKeyPressed = shiftKeyPressed;


		//-------------------------------------------------------------------
		notifyPointerMoved(pointerEventData);


		//-------------------------------------------------------------------
		std::shared_ptr<Item> item;
		item = hitTest(pointerEventData._PointerPosition);


		//-------------------------------------------------------------------
		if (_Item_PointerPressed)
		{
			if (_Item_PointerDragging == _Item_PointerPressed)
			{
				notifyItemPointerDragging(_Item_PointerDragging, pointerEventData);
			}
			else
			{
				_Item_PointerDragging = nullptr;
			}
		}
		else
		{
			_Item_PointerDragging = nullptr;
		}


		//-------------------------------------------------------------------
		if (_Item_PointerOver != item)
		{
			if (_Item_PointerOver)
			{
				notifyItemPointerLeave(_Item_PointerOver, pointerEventData);
			}

			_Item_PointerOver = item;

			if (_Item_PointerOver)
			{
				notifyItemPointerOver(_Item_PointerOver, pointerEventData);
			}
		}
	}

	//=======================================================================
	void EventGenerator::notifyPointerMoved(PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			PointerMovedEvent,
			nullptr,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyPointerLButtonDown(PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			PointerLButtonDownEvent,
			nullptr,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyPointerLButtonUp(PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			PointerLButtonUpEvent,
			nullptr,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerPressed(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerPressedEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerReleased(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerReleasedEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerClicked(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerClickedEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerDbClicked(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerDbClickedEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerOver(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerOverEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerLeave(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerLeaveEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}

	void EventGenerator::notifyItemPointerDragging(std::shared_ptr<Item> item, PointerEventData& pointerEventData)
	{
		_View.eventDispatcher().notifyEvent(
			ItemPointerDraggingEvent,
			item,
			std::make_shared<PointerEventData>(pointerEventData)
		);
	}
}
