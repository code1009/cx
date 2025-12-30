/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-ev.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::ev
{
	//=======================================================================
	Event::Event(EventType const eventType, std::shared_ptr<EventData>& eventData) :
		_Handled(false),
		_EventType(eventType),
		_EventData(eventData)
	{
	}

	//=======================================================================
	EventType Event::eventType() const
	{
		return _EventType;
	}

	std::shared_ptr<EventData> Event::eventData() const
	{
		return _EventData;
	}

	bool Event::handled() const
	{
		return _Handled;
	}

	void Event::handled(bool const handled)
	{
		_Handled = handled;
	}
}



