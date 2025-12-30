/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-ev.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	void EventListener::attach(Key const& key, EventHandler const& eventHandler)
	{
		_EventHandlers[key] = eventHandler;
	}

	void EventListener::detach(Key const& key)
	{
		_EventHandlers.erase(key);
	}

	//=======================================================================
	void EventListener::clear()
	{
		_EventHandlers.clear();
	}

	bool EventListener::empty() const
	{
		return _EventHandlers.empty();
	}

	//=======================================================================
	void EventListener::notify(Event& event)
	{
		for (const auto& [eventTarget, eventHandler] : _EventHandlers)
		{
			eventHandler(event);
			if (event.handled())
			{
				break;
			}
		}
	}

	void EventListener::notify(EventType const eventType, std::shared_ptr<EventData> eventData)
	{
		Event event{ eventType, eventData };
		notify(event);
	}

	EventHandler EventListener::getEventHandler(Key const& key) const
	{
		auto it = _EventHandlers.find(key);
		if (it != _EventHandlers.end())
		{
			return it->second;
		}
		return nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	void EventDispatcher::registerEventListener(EventType const eventType, std::shared_ptr<EventListener> eventListener)
	{
		_EventListenerMap[eventType] = eventListener;
	}

	void EventDispatcher::unregisterEventListener(EventType const eventType)
	{
		_EventListenerMap.erase(eventType);
	}

	void EventDispatcher::unregisterEventHandler(Key const key)
	{
		std::vector<EventType> eventTypes;
		for (auto& [eventType, eventListener] : _EventListenerMap)
		{
			if (eventListener)
			{
				eventListener->detach(key);
				if (eventListener->empty())
				{
					eventTypes.push_back(eventType);
				}
			}
		}
		for (const auto& eventType : eventTypes)
		{
			_EventListenerMap.erase(eventType);
		}
	}

	//=======================================================================
	std::shared_ptr<EventListener> EventDispatcher::getEventListener(EventType const eventType)
	{
		auto it = _EventListenerMap.find(eventType);
		if (it != _EventListenerMap.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void EventDispatcher::dispatchEvent(EventType const eventType, Event& event)
	{
		auto eventListener = getEventListener(eventType);
		if (eventListener)
		{
			eventListener->notify(event);
		}
	}

	//=======================================================================
	void EventDispatcher::notifyEvent(EventType const eventType, Event& event)
	{
		dispatchEvent(eventType, event);
	}

	void EventDispatcher::notifyEvent(EventType const eventType, std::shared_ptr<EventData> eventData)
	{
		Event event{ eventType, eventData };
		notifyEvent(eventType, event);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	EventHandlerRegistry::EventHandlerRegistry(EventDispatcher& eventDispatcher) :
		_EventDispatcher(eventDispatcher)
	{
	}

	//=======================================================================
	void EventHandlerRegistry::registerEventHandler(
		EventType const eventType,
		Key const key,
		EventHandler const& eventHandler
	)
	{
		auto eventListener = _EventDispatcher.getEventListener(eventType);
		if (!eventListener)
		{
			eventListener = std::make_shared<EventListener>();
			_EventDispatcher.registerEventListener(eventType, eventListener);
		}
		eventListener->attach(key, eventHandler);
	}

	void EventHandlerRegistry::unregisterEventHandler(Key const key)
	{
		_EventDispatcher.unregisterEventHandler(key);
	}
}




