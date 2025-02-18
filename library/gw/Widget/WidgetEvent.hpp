/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template<typename...TEventParam>
using EventHandler = std::function<void(TEventParam...)>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class EventListener
{
public:
	virtual ~EventListener() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template<typename...TEventParam>
class EventParamListener : public EventListener
{
public:
	EventHandler<TEventParam...> _handler;

public:
	EventParamListener(EventHandler<TEventParam...> handler) :
		_handler(handler)
	{
	}

public:
	void invoke(TEventParam... param)
	{
		_handler(param...);
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using EventListenerSharedPtr = std::shared_ptr<EventListener>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template<typename TEventTarget>
class TargetedEventDispatcher
{
public:
	std::unordered_map<TEventTarget, EventListenerSharedPtr> _EventListenerSharedPtrMap;

public:
	template<typename...TEventParam>
	void registerEventHandler(EventHandler<TEventParam...> handler, TEventTarget target = 0)
	{
		registerEventListener(
			std::make_shared<EventParamListener<TEventParam...>>(std::move(handler)),
			target
		);
	}

	void registerEventListener(EventListenerSharedPtr listener, TEventTarget target)
	{
		_EventListenerSharedPtrMap[target] = std::move(listener);
	}

	void unregisterTarget(TEventTarget target)
	{
		_EventListenerSharedPtrMap.erase(target);
	}

public:
	template<typename...TEventParam>
	void broadcast(TEventParam... param)
	{
		for (auto& pair : _EventListenerSharedPtrMap)
		{
			auto eventListenerSharedPtr = std::dynamic_pointer_cast<EventParamListener<TEventParam...>>(pair.second);
			if (eventListenerSharedPtr)
			{
				eventListenerSharedPtr->invoke(param...);
			}
		}
	}

	template<typename...TEventParam>
	void unicast(TEventTarget target, TEventParam... param)
	{
		auto found = _EventListenerSharedPtrMap.find(target);
		if (found != _EventListenerSharedPtrMap.end())
		{
			auto eventListenerSharedPtr = std::dynamic_pointer_cast<EventParamListener<TEventParam...>>((*found).second);
			if (eventListenerSharedPtr)
			{
				eventListenerSharedPtr->invoke(param...);
			}
		}
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template<typename TEventType, typename TEventTarget = void*>
class TypedEventDispatcher
{
public:
	std::unordered_map<
		TEventType,
		std::shared_ptr<TargetedEventDispatcher<TEventTarget>>
	> _EventDispatcherSharedPtrMap;

public:
	template<typename...TEventParam>
	void registerEventHandler(TEventType name, TEventTarget target, EventHandler<TEventParam...> handler)
	{
		auto found = _EventDispatcherSharedPtrMap.find(name);
		if (found != _EventDispatcherSharedPtrMap.end())
		{
			auto eventDispatcherSharedPtr = (*found).second;
			eventDispatcherSharedPtr->registerEventHandler<TEventParam...>(handler, target);
		}
		else
		{
			auto eventDispatcherSharedPtr = std::make_shared<TargetedEventDispatcher<TEventTarget>>();
			_EventDispatcherSharedPtrMap[name] = eventDispatcherSharedPtr;
			eventDispatcherSharedPtr->registerEventHandler<TEventParam...>(handler, target);
		}
	}

	void unregisterTarget(TEventTarget target)
	{
		for (auto& pair : _EventDispatcherSharedPtrMap)
		{
			auto eventDispatcherSharedPtr = pair.second;
			eventDispatcherSharedPtr->unregisterTarget(target);
		}
	}

public:
	template<typename...TEventParam>
	void dispatch(TEventType name, TEventParam... param)
	{
		auto found = _EventDispatcherSharedPtrMap.find(name);
		if (found != _EventDispatcherSharedPtrMap.end())
		{
			auto eventDispatcherSharedPtr = (*found).second;
			eventDispatcherSharedPtr->broadcast<TEventParam...>(param...);
		}
	}

	template<typename...TEventParam>
	void dispatch(TEventType name, TEventTarget target, TEventParam... param)
	{
		auto found = _EventDispatcherSharedPtrMap.find(name);
		if (found != _EventDispatcherSharedPtrMap.end())
		{
			auto eventDispatcherSharedPtr = (*found).second;
			eventDispatcherSharedPtr->unicast<TEventParam...>(target, param...);
		}
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template<typename TEventType, typename TEventTarget = void*>
class TypedEventSingleParamDispatcher
{
public:
	std::unordered_map<
		TEventType,
		std::shared_ptr<TargetedEventDispatcher<TEventTarget>>
	> _EventDispatcherSharedPtrMap;

public:
	template<typename TEventParam>
	void registerEventHandler(TEventType name, TEventTarget target, EventHandler<TEventParam> handler)
	{
		auto found = _EventDispatcherSharedPtrMap.find(name);
		if (found != _EventDispatcherSharedPtrMap.end())
		{
			auto eventDispatcherSharedPtr = (*found).second;
			eventDispatcherSharedPtr->registerEventHandler<TEventParam>(handler, target);
		}
		else
		{
			auto eventDispatcherSharedPtr = std::make_shared<TargetedEventDispatcher<TEventTarget>>();
			_EventDispatcherSharedPtrMap[name] = eventDispatcherSharedPtr;
			eventDispatcherSharedPtr->registerEventHandler<TEventParam>(handler, target);
		}
	}

	void unregisterTarget(TEventTarget target)
	{
		for (auto& pair : _EventDispatcherSharedPtrMap)
		{
			auto eventDispatcherSharedPtr = pair.second;
			eventDispatcherSharedPtr->unregisterTarget(target);
		}
	}

public:
	template<typename TEventParam>
	void dispatch(TEventType name, TEventParam param)
	{
		auto found = _EventDispatcherSharedPtrMap.find(name);
		if (found != _EventDispatcherSharedPtrMap.end())
		{
			auto eventDispatcherSharedPtr = (*found).second;
			eventDispatcherSharedPtr->broadcast<TEventParam>(param);
		}
	}

	template<typename TEventParam>
	void dispatch(TEventType name, TEventTarget target, TEventParam param)
	{
		auto found = _EventDispatcherSharedPtrMap.find(name);
		if (found != _EventDispatcherSharedPtrMap.end())
		{
			auto eventDispatcherSharedPtr = (*found).second;
			eventDispatcherSharedPtr->unicast<TEventParam>(target, param);
		}
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Widget;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class WidgetEventType
{
	Unknown,

	MouseMove,
	MouseLButtonDown,
	MouseLButtonUp,

	MousePressed,
	MouseReleased,
	MouseClicked,
	MouseDbClicked,
	MouseOver,
	MouseLeave,
	MouseDragging,

	MouseDragEnter,
	MouseDragOver,
	MouseDragLeave,
	MouseDrop
};

class WidgetMouseEventParam
{
public:
	Point _MousePosition{};
	bool  _MouseLButtonPressed{ false };
	bool  _MouseRButtonPressed{ false };
	bool  _ShiftKeyPressed{ false };
	bool  _CtrlKeyPressed{ false };
};

class WidgetMouseDragEnterEventParam
{
public:
	std::uint32_t _DataSeq{ 0 };
	Point _MousePosition{};
	std::wstring _String{};
};

class WidgetMouseDragOverEventParam
{
public:
	std::uint32_t _DataSeq{ 0 };
	Point _mouse_po_MousePositionsition{};
};

class WidgetMouseDragLeaveEventParam
{
public:
	std::uint32_t _DataSeq{ 0 };
};

class WidgetMouseDropEventParam
{
public:
	std::uint32_t _DataSeq{ 0 };
	Point _MousePosition{};
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using WidgetEventDispatcher = TypedEventSingleParamDispatcher<WidgetEventType, Widget*>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




