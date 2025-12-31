/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Design::Design(StringView const& className) :
		Item(className)
	{
		_Anchors = std::make_unique<Anchors>();


		anchors_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void Design::anchors_attachPropertyChangedEventHandler(void)
	{
		_Anchors->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Design::anchors_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Design::anchors_detachPropertyChangedEventHandler(void)
	{
		_Anchors->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Design::anchors_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace) 
			<< L"Design::anchors_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Design::target_attachPropertyChangedEventHandler(void)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Design::target_attachPropertyChangedEventHandler:" << std::endl
			<< L"    className=" << className() << std::endl
			<< L"    this=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(this)) << std::endl
			<< L"    target=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(getTarget().get()))
			;
		*/
		getTarget()->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Design::target_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}
	void Design::target_detachPropertyChangedEventHandler(void)
	{
		getTarget()->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}
	void Design::target_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Design::target_onPropertyChanged:" << std::endl
			<< L"    eventType=" << event.eventType() << std::endl
			<< L"    className=" << className() << std::endl
			<< L"    this=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(this)) << std::endl
			<< L"    target=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(getTarget().get()))
			;
		*/

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Design::target_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Design::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Design>(dest);
		if (!other)
		{
			CX_RUNTIME_ASSERT(0);
			return;
		}
		if (className() != other->className())
		{
			CX_RUNTIME_ASSERT(0);
			return;
		}
		Item::copyTo(other);

		other->anchors().visible(anchors().visible());

		getTarget()->copyTo(other->getTarget());
	}

	//=======================================================================
	bool Design::isPointIn(const Point& test)
	{
		if (anchors().visible())
		{
			if (anchors().isPointIn(test))
			{
				return true;
			}
		}
		return getTarget()->isPointIn(test);
	}

	void Design::moveOffset(Point const& offset)
	{
		getTarget()->moveOffset(offset);
		Item::moveOffset(offset);
	}

	//=======================================================================
	void Design::registerEventHandler(cx::ev::target::EventHandlerRegistry& eventHandlerRegistry)
	{
		eventHandlerRegistry.registerEventHandler(
			ItemPointerPressedEvent,
			shared_from_this(),
			std::bind(
				&Design::onItemPointerPressed,
				this,
				std::placeholders::_1
			)
		);
		eventHandlerRegistry.registerEventHandler(
			ItemPointerReleasedEvent,
			shared_from_this(),
			std::bind(
				&Design::onItemPointerReleased,
				this,
				std::placeholders::_1
			)
		);
		eventHandlerRegistry.registerEventHandler(
			ItemPointerDraggingEvent,
			shared_from_this(),
			std::bind(
				&Design::onItemPointerDragging,
				this,
				std::placeholders::_1
			)
		);
	}

	//=======================================================================
	void Design::drawContentUnderlay(DrawingContext& dctx)
	{
		getTarget()->drawContentUnderlay(dctx);
	}

	void Design::drawContent(DrawingContext& dctx)
	{
		getTarget()->drawContent(dctx);
	}

	void Design::drawContentOverlay(DrawingContext& dctx)
	{
		getTarget()->drawContentOverlay(dctx);
		if (anchors().visible())
		{
			anchors().drawPointsBounds(dctx);
			anchors().drawPoints(dctx);
		}
	}

	//=======================================================================
	void Design::onItemPointerPressed(cx::ev::Event& event)
	{
		std::shared_ptr<PointerEventData> eventData = event.eventDataAs<PointerEventData>();
		Point PointerPressedPosition = eventData->_PointerPosition;;

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Design::onItemPointerPressed:" << event.eventType()
			;
		*/

		_AnchorPointerPressedPosition = PointerPressedPosition;
		_AnchorPointerPressed = anchors().find(_AnchorPointerPressedPosition);
	}

	void Design::onItemPointerReleased(cx::ev::Event& event)
	{
		std::shared_ptr<PointerEventData> eventData = event.eventDataAs<PointerEventData>();
		Point PointerPressedPosition = eventData->_PointerPosition;;
		Point PointerPressedSnappedPosition = getEdit().toSnappedPoint(PointerPressedPosition);
		//Point offset = PointerPressedSnappedPosition - _AnchorPointerPressedPosition;
		_AnchorPointerPressedPosition = PointerPressedSnappedPosition;

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Design::onItemPointerReleased:" << event.eventType()
			;
		*/

		if (_AnchorPointerPressed)
		{
			getEdit().moveItemAnchor(
				shared_from_this(),
				_AnchorPointerPressed,
				PointerPressedSnappedPosition
			);
		}

		_AnchorPointerPressed = nullptr;
	}

	void Design::onItemPointerDragging(cx::ev::Event& event)
	{
		std::shared_ptr<PointerEventData> eventData = event.eventDataAs<PointerEventData>();
		Point PointerPressedPosition = eventData->_PointerPosition;;
		Point PointerPressedSnappedPosition = getEdit().toSnappedPoint(PointerPressedPosition);
		//Point offset = PointerPressedSnappedPosition - _AnchorPointerPressedPosition;
		_AnchorPointerPressedPosition = PointerPressedSnappedPosition;

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Design::onItemPointerDragging:" << event.eventType()
			;
		*/

		if (_AnchorPointerPressed)
		{
			getEdit().moveItemAnchor(
				shared_from_this(),
				_AnchorPointerPressed,
				PointerPressedSnappedPosition
			);
		}
	}

	//=======================================================================
	void Design::moveAnchor(std::shared_ptr<Anchor>& /*anchor*/, const Point& /*position*/)
	{
	}
	
	//=======================================================================
	void Design::setTarget(std::shared_ptr<Item> const& target)
	{
		if (_Target == target)
		{
			return;
		}
		_Target = target;
		updateTarget();
		target_attachPropertyChangedEventHandler();
	}

	void Design::updateTarget(void)
	{
	}
}
