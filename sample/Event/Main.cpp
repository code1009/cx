/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <iostream>
#include <memory>
#include <map>
#include <format>
#include <functional>
#include <unordered_map>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <cx/ev/cx-ev.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{
	class ObjectEventData : public cx::ev::EventData
	{
	public:
		int value;

	public:
		explicit ObjectEventData(int val) : value(val)
		{
		}
	};
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{
	class Object : public std::enable_shared_from_this<Object>
	{
	private:
		std::uint32_t _Id;

	public:
		explicit Object(std::uint32_t id) : _Id(id)
		{
		}

	public:
		virtual ~Object() = default;

		void eventHandler_A(cx::ev::Event& event)
		{
			std::cout
				<< std::format("[{}] ", _Id)
				<< "eventHandler_A:"
				<< " type=" << event.eventType()
				<< " value=" << event.eventDataAs<ObjectEventData>()->value
				<< std::endl
				;

			//event.handled(true);
		}

		void eventHandler_B(cx::ev::Event& event)
		{
			std::cout
				<< std::format("[{}] ", _Id)
				<< "eventHandler_B:"
				<< " type=" << event.eventType()
				<< " value=" << event.eventDataAs<ObjectEventData>()->value
				<< std::endl
				;

			//event.handled(true);
		}

		void eventHandler_C(cx::ev::Event& event)
		{
			std::cout
				<< std::format("[{}] ", _Id)
				<< "eventHandler_C:"
				<< " type=" << event.eventType()
				<< " is_null=" << (event.eventData() == nullptr ? "null" : "not null")
				<< std::endl
				;

			//event.handled(true);
		}
	};
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test1(void)
{
	const cx::ev::EventType EventType_A = 1;
	const cx::ev::EventType EventType_B = 2;
	const cx::ev::EventType EventType_C = 3;

	std::shared_ptr<app::Object> object1 = std::make_shared<app::Object>(1);
	std::shared_ptr<app::Object> object2 = std::make_shared<app::Object>(2);

	cx::ev::target::EventDispatcher eventDispatcher;
	cx::ev::target::EventHandlerRegistry eventHandlerRegistry(eventDispatcher);


	eventHandlerRegistry.registerEventHandler(
		EventType_A,
		object1,
		std::bind(&app::Object::eventHandler_A, object1, std::placeholders::_1)
	);
	eventHandlerRegistry.registerEventHandler(
		EventType_B,
		object1,
		std::bind(&app::Object::eventHandler_B, object1, std::placeholders::_1)
	);

	eventHandlerRegistry.registerEventHandler(
		EventType_A,
		object2,
		std::bind(&app::Object::eventHandler_A, object2, std::placeholders::_1)
	);
	eventHandlerRegistry.registerEventHandler(
		EventType_B,
		object2,
		std::bind(&app::Object::eventHandler_B, object2, std::placeholders::_1)
	);
	eventHandlerRegistry.registerEventHandler(
		EventType_C,
		object2,
		std::bind(&app::Object::eventHandler_C, object2, std::placeholders::_1)
	);

	eventDispatcher.notifyEvent(EventType_A, object1, std::make_shared<app::ObjectEventData>(101));
	eventDispatcher.notifyEvent(EventType_B, object1, std::make_shared<app::ObjectEventData>(102));
	eventDispatcher.notifyEvent(EventType_A, object2, std::make_shared<app::ObjectEventData>(103));
	eventDispatcher.notifyEvent(EventType_B, object2, std::make_shared<app::ObjectEventData>(104));


	eventHandlerRegistry.unregisterEventHandler(object1);


	eventDispatcher.notifyEvent(EventType_B, object1, std::make_shared<app::ObjectEventData>(105));
	eventDispatcher.notifyEvent(EventType_B, object2, std::make_shared<app::ObjectEventData>(106));
	eventDispatcher.notifyEvent(EventType_C, object2, nullptr);
	eventDispatcher.notifyEvent(EventType_C, object2, std::make_shared<app::ObjectEventData>(107));
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test2(void)
{
	const cx::ev::EventType EventType_A = 1;
	const cx::ev::EventType EventType_B = 2;

	std::shared_ptr<app::Object> object1 = std::make_shared<app::Object>(1);
	std::shared_ptr<app::Object> object2 = std::make_shared<app::Object>(2);

	cx::ev::key::EventListener eventListener;

	eventListener.attach(
		reinterpret_cast<std::uintptr_t>(object1.get()),
		std::bind(&app::Object::eventHandler_A, object1, std::placeholders::_1)
	);
	eventListener.attach(
		reinterpret_cast<std::uintptr_t>(object1.get()), // &app::Object::eventHandler_A 덮어 씀. 주의 할 것!
		std::bind(&app::Object::eventHandler_B, object1, std::placeholders::_1)
	);
	eventListener.attach(
		reinterpret_cast<std::uintptr_t>(object2.get()),
		std::bind(&app::Object::eventHandler_A, object2, std::placeholders::_1)
	);
	eventListener.attach(
		reinterpret_cast<std::uintptr_t>(object2.get()), // &app::Object::eventHandler_A 덮어 씀. 주의 할 것!
		std::bind(&app::Object::eventHandler_B, object2, std::placeholders::_1)
	);

	std::shared_ptr<cx::ev::EventData> eventData = std::make_shared<app::ObjectEventData>(101);
	cx::ev::Event event_A{ EventType_A, eventData };
	eventListener.notify(event_A);
	eventListener.notify(EventType_B, std::make_shared<app::ObjectEventData>(102));

	eventListener.detach(reinterpret_cast<std::uintptr_t>(object1.get()));

	eventListener.notify(EventType_A, std::make_shared<app::ObjectEventData>(103));
	eventListener.notify(EventType_B, std::make_shared<app::ObjectEventData>(104));
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test3(void)
{
	const cx::ev::EventType EventType_A = 1;
	const cx::ev::EventType EventType_B = 2;
	const cx::ev::EventType EventType_C = 3;

	std::shared_ptr<app::Object> object1 = std::make_shared<app::Object>(1);
	std::shared_ptr<app::Object> object2 = std::make_shared<app::Object>(2);

	cx::ev::key::EventDispatcher eventDispatcher;
	cx::ev::key::EventHandlerRegistry eventHandlerRegistry(eventDispatcher);


	eventHandlerRegistry.registerEventHandler(
		EventType_A,
		reinterpret_cast<std::uintptr_t>(object1.get()),
		std::bind(&app::Object::eventHandler_A, object1, std::placeholders::_1)
	);
	eventHandlerRegistry.registerEventHandler(
		EventType_B,
		reinterpret_cast<std::uintptr_t>(object1.get()),
		std::bind(&app::Object::eventHandler_B, object1, std::placeholders::_1)
	);

	eventHandlerRegistry.registerEventHandler(
		EventType_A,
		reinterpret_cast<std::uintptr_t>(object2.get()),
		std::bind(&app::Object::eventHandler_A, object2, std::placeholders::_1)
	);
	eventHandlerRegistry.registerEventHandler(
		EventType_B,
		reinterpret_cast<std::uintptr_t>(object2.get()),
		std::bind(&app::Object::eventHandler_B, object2, std::placeholders::_1)
	);
	eventHandlerRegistry.registerEventHandler(
		EventType_C,
		reinterpret_cast<std::uintptr_t>(object2.get()),
		std::bind(&app::Object::eventHandler_C, object2, std::placeholders::_1)
	);

	eventDispatcher.notifyEvent(EventType_A, std::make_shared<app::ObjectEventData>(101));
	eventDispatcher.notifyEvent(EventType_B, std::make_shared<app::ObjectEventData>(102));
	eventDispatcher.notifyEvent(EventType_A, std::make_shared<app::ObjectEventData>(103));
	eventDispatcher.notifyEvent(EventType_B, std::make_shared<app::ObjectEventData>(104));


	eventHandlerRegistry.unregisterEventHandler(reinterpret_cast<std::uintptr_t>(object1.get()));


	eventDispatcher.notifyEvent(EventType_B, std::make_shared<app::ObjectEventData>(105));
	eventDispatcher.notifyEvent(EventType_B, std::make_shared<app::ObjectEventData>(106));
	eventDispatcher.notifyEvent(EventType_C, nullptr);
	eventDispatcher.notifyEvent(EventType_C, std::make_shared<app::ObjectEventData>(107));
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	std::cout
		<< "-----------------------------------------------------------------"
		<< std::endl
		;
	test1();
	std::cout
		<< std::endl
		<< std::endl
		<< std::endl
		;



	std::cout
		<< "-----------------------------------------------------------------"
		<< std::endl
		;
	test2();
	std::cout
		<< std::endl
		<< std::endl
		<< std::endl
		;



	std::cout
		<< "-----------------------------------------------------------------"
		<< std::endl
		;
	test3();
	std::cout
		<< std::endl
		<< std::endl
		<< std::endl
		;



	std::cout
		<< "-----------------------------------------------------------------"
		<< std::endl
		<< "End"
		<< std::endl
		;
	return 0;
}