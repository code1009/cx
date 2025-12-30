#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	using Key = std::uintptr_t;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	class EventListener
	{
	private:
		std::unordered_map<Key, EventHandler> _EventHandlers;

	public:
		void attach(Key const& key, EventHandler const& eventHandler);
		void detach(Key const& key);

	public:
		void clear();
		bool empty() const;

	public:
		void notify(Event& event);
		void notify(EventType const eventType, std::shared_ptr<EventData> eventData);

	public:
		EventHandler getEventHandler(Key const& key) const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	class EventDispatcher
	{
	private:
		std::map<EventType, std::shared_ptr<EventListener>> _EventListenerMap;

	public:
		void registerEventListener(EventType const eventType, std::shared_ptr<EventListener> eventListener);
		void unregisterEventListener(EventType const eventType);
		void unregisterEventHandler(Key const key);
		std::shared_ptr<EventListener> getEventListener(EventType const eventType);

	protected:
		void dispatchEvent(EventType const eventType, Event& event);

	public:
		void notifyEvent(EventType const eventType, Event& event);
		void notifyEvent(EventType const eventType, std::shared_ptr<EventData> eventData);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::key
{
	//=======================================================================
	class EventHandlerRegistry
	{
	private:
		EventDispatcher& _EventDispatcher;

	public:
		explicit EventHandlerRegistry(EventDispatcher& eventDispatcher);

	public:
		void registerEventHandler(
			EventType const eventType,
			Key const key,
			EventHandler const& eventHandler
		);
		void unregisterEventHandler(Key const key);
	};
}




