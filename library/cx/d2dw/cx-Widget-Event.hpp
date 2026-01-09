#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	constexpr cx::ev::EventType DrawingStyle_PropertyChangedEvent = 100;
	constexpr cx::ev::EventType ItemStyle_PropertyChangedEvent = 101;
	constexpr cx::ev::EventType AnchorStyle_PropertyChangedEvent = 102;
	constexpr cx::ev::EventType EditPointerToolStyle_PropertyChangedEvent = 103;

	//=======================================================================
	constexpr cx::ev::EventType Anchor_PropertyChangedEvent = 201;
	constexpr cx::ev::EventType Anchors_PropertyChangedEvent = 202;

	constexpr cx::ev::EventType Status_PropertyChangedEvent = 301;
	constexpr cx::ev::EventType Item_PropertyChangedEvent = 401;
	constexpr cx::ev::EventType Model_PropertyChangedEvent = 402;

	constexpr cx::ev::EventType Property_PropertyChangedEvent = 501;
	constexpr cx::ev::EventType Properties_PropertyChangedEvent = 502;
	constexpr cx::ev::EventType PropertiesManipulator_PropertyChangedEvent = 503;

	constexpr cx::ev::EventType ViewBackground_PropertyChangedEvent = 601;
	constexpr cx::ev::EventType View_InvalidatedEvent = 602;

	constexpr cx::ev::EventType EditPointerTool_PropertyChangedEvent = 701;
	constexpr cx::ev::EventType Edit_ItemSelectionChangedEvent = 702;
	constexpr cx::ev::EventType Edit_ViewPropertiesChanged = 703;

	constexpr cx::ev::EventType EditSeed_ModifiedEvent = 901;
	constexpr cx::ev::EventType Edit_ModifiedEvent = 902;

	//=======================================================================
	constexpr cx::ev::EventType PointerMovedEvent = 1000;
	constexpr cx::ev::EventType PointerLButtonDownEvent = 1001;
	constexpr cx::ev::EventType PointerLButtonUpEvent = 1002;

	constexpr cx::ev::EventType ItemPointerPressedEvent = 1101;
	constexpr cx::ev::EventType ItemPointerReleasedEvent = 1102;
	constexpr cx::ev::EventType ItemPointerClickedEvent = 1103;
	constexpr cx::ev::EventType ItemPointerDbClickedEvent = 1104;
	constexpr cx::ev::EventType ItemPointerOverEvent = 1105;
	constexpr cx::ev::EventType ItemPointerLeaveEvent = 1106;
	constexpr cx::ev::EventType ItemPointerDraggingEvent = 1107;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class Item;
	class View;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class PointerEventData : public cx::ev::EventData
	{
	public:
		Point _PointerPosition{};
		bool  _LButtonPressed{ false };
		bool  _RButtonPressed{ false };
		bool  _ShiftKeyPressed{ false };
		bool  _CtrlKeyPressed{ false };
	};

	//=======================================================================
	class ItemPointerEventData : public cx::ev::EventData
	{
	public:
		std::shared_ptr<Item> _Item{ nullptr };

	public:
		Point _PointerPosition{};
		bool  _LButtonPressed{ false };
		bool  _RButtonPressed{ false };
		bool  _ShiftKeyPressed{ false };
		bool  _CtrlKeyPressed{ false };

	public:
		explicit ItemPointerEventData(std::shared_ptr<Item>& item, PointerEventData const& pointerEventData) :
			_Item(item),
			_PointerPosition(pointerEventData._PointerPosition),
			_LButtonPressed(pointerEventData._LButtonPressed),
			_RButtonPressed(pointerEventData._RButtonPressed),
			_ShiftKeyPressed(pointerEventData._ShiftKeyPressed),
			_CtrlKeyPressed(pointerEventData._CtrlKeyPressed)
		{
		}
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class EventGenerator
	{
	private:
		View& _View;

	private:
		std::shared_ptr<Item> _Item_PointerPressed{ nullptr };
		std::shared_ptr<Item> _Item_PointerReleased{ nullptr };
		std::shared_ptr<Item> _Item_PointerClicked{ nullptr };
		std::shared_ptr<Item> _Item_PointerDbClicked{ nullptr };
		std::shared_ptr<Item> _Item_PointerOver{ nullptr };
		std::shared_ptr<Item> _Item_PointerDragging{ nullptr };

	private:
		std::uint64_t _PointerClickedTime{ 0 };
		std::uint64_t _PointerDbClickTime{ 300 };

	private:
		bool  _PointerCaptured{ false };
		Point _PointerCapturedPosition{ 0.0f, 0.0f };
		Point _PointerReleasedPosition{ 0.0f, 0.0f };
		Point _PointerCurrentPosition{ 0.0f, 0.0f };

	private:
		bool _VisibleItemOnly{ false };

	public:
		explicit EventGenerator(View& view, bool visibleItemOnly);

	public:
		EventGenerator(const EventGenerator& other) = delete;
		EventGenerator& operator=(const EventGenerator& other) = delete;
		EventGenerator(EventGenerator&& other) noexcept = delete;
		EventGenerator& operator=(EventGenerator&& other) noexcept = delete;

		//-------------------------------------------------------------------
	public:
		bool contains(std::shared_ptr<Item> test);
		std::shared_ptr<Item> hitTest(const Point& point);

	protected:
		void update(void);

		//-------------------------------------------------------------------
	public:
		bool isPointerCaptured() const { return _PointerCaptured; }
		Point pointerCapturedPosition() const { return _PointerCapturedPosition; }
		Point pointerReleasedPosition() const { return _PointerReleasedPosition; }
		Point pointerCurrentPosition() const { return _PointerCurrentPosition; }

	protected:
		void capturePointer(Coord const x, Coord const y)
		{
			capturePointer(Point{ x, y });
		}
		void releasePointer(Coord const x, Coord const y)
		{
			releasePointer(Point{ x, y });
		}
		void capturePointer(Point const& position)
		{
			_PointerCaptured = true;
			_PointerCapturedPosition = position;
		}
		void releasePointer(Point const& position)
		{
			_PointerCaptured = false;
			_PointerReleasedPosition = position;
		}

	protected:
		void setCurrentPointerPosition(Coord const x, Coord const y)
		{
			setCurrentPointerPosition(Point{ x, y });
		}
		void setCurrentPointerPosition(Point const& position)
		{
			_PointerCurrentPosition = position;
		}

	public:
		void pointerPressed(Coord const px, Coord const py, bool controlKeyPressed, bool shiftKeyPressed);
		void pointerReleased(Coord const px, Coord const py, bool controlKeyPressed, bool shiftKeyPressed);
		void pointerMoved(Coord const px, Coord const py, bool controlKeyPressed, bool shiftKeyPressed);

	public:
		void notifyPointerMoved(PointerEventData& pointerEventData);
		void notifyPointerLButtonDown(PointerEventData& pointerEventData);
		void notifyPointerLButtonUp(PointerEventData& pointerEventData);
		void notifyItemPointerPressed(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
		void notifyItemPointerReleased(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
		void notifyItemPointerClicked(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
		void notifyItemPointerDbClicked(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
		void notifyItemPointerOver(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
		void notifyItemPointerLeave(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
		void notifyItemPointerDragging(std::shared_ptr<Item> item, PointerEventData& pointerEventData);
	};
}
