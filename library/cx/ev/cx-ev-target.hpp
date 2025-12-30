#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::target
{
	//=======================================================================
	class EventListener
	{
	public:
		using Token = std::uint32_t;

	private:
		Token _CurrentToken;
		std::unordered_map<Token, EventHandler> _EventHandlers;

	public:
		EventListener();

	public:
		Token attach(EventHandler const& eventHandler);
		void detach(Token const token);

	public:
		void clear();
		bool empty() const;

	public:
		void notify(Event& event);
		void notify(EventType const eventType, std::shared_ptr<EventData> eventData);

	public:
		EventHandler getEventHandler(Token const& token) const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::target
{
	//=======================================================================
	using EventTarget = std::shared_ptr<void>;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::target
{
	//=======================================================================
	class EventId
	{
	private:
		EventType _EventType;
		EventTarget _EventTarget;

	public:
		EventId(EventType const eventType, EventTarget const& eventTarget);

	public:
		EventType const eventType() const;
		EventTarget const eventTarget() const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::target
{
	//=======================================================================
	inline bool operator==(EventId const& lhs, EventId const& rhs)
	{
		return (lhs.eventType() == rhs.eventType() && lhs.eventTarget() == rhs.eventTarget());
	}
	inline bool operator!=(EventId const& lhs, EventId const& rhs)
	{
		return !(lhs == rhs);
	}
	inline bool operator<(EventId const& lhs, EventId const& rhs)
	{
		if (lhs.eventType() < rhs.eventType())
		{
			return true;
		}
		if (lhs.eventType() > rhs.eventType())
		{
			return false;
		}
		return lhs.eventTarget() < rhs.eventTarget();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::target
{
	//=======================================================================
	class EventDispatcher
	{
	private:
		std::map<EventId, std::shared_ptr<EventListener>> _EventListenerMap;

	public:
		void registerEventListener(EventId const eventId, std::shared_ptr<EventListener> eventListener);
		void unregisterEventListener(EventId const eventId);
		std::shared_ptr<EventListener> getEventListener(EventId const eventId);

	protected:
		void dispatchEvent(EventId const& eventId, Event& event);

	public:
		void notifyEvent(EventId const& eventId, Event& event);
		void notifyEvent(EventType const eventType, EventTarget const eventTarget, std::shared_ptr<EventData> eventData);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev::target
{
	//=======================================================================
	class EventHandlerRegistry
	{
	private:
		EventDispatcher& _EventDispatcher;
		std::vector<std::pair<EventId, EventListener::Token>> _EventIdTokens;

	public:
		explicit EventHandlerRegistry(EventDispatcher& eventDispatcher);

	public:
		void registerEventHandler(
			EventType const eventType,
			EventTarget const& eventTarget,
			EventHandler const& eventHandler
		);
		void unregisterEventHandler(EventTarget const& eventTarget);
	};
}




