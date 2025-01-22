#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename TWindow>
class WindowMessageMapT final
{
public:
	using WindowMessageHandler = WindowMessageHandlerT<TWindow>;
	using WindowMessageHandlers = std::map<std::uint32_t, WindowMessageHandler>;

private:
	WindowMessageHandlers _WindowMessageHandlers{};

public:
	void registerWindowMessageHandler(UINT umsg, WindowMessageHandler handler)
	{
		_WindowMessageHandlers[umsg] = handler;
	}

	void unregisterWindowMessageHandler(UINT umsg)
	{
		_WindowMessageHandlers.erase(umsg);
	}

public:
	WindowMessageHandler& handle(std::uint32_t umsg)
	{
		return _WindowMessageHandlers[umsg];
	}

public:
	bool onWindowMessage(TWindow* window, WindowMessage& windowMessage)
	{
		auto found = _WindowMessageHandlers.find(windowMessage.uMsg);


		if (found != _WindowMessageHandlers.end())
		{
			auto handler = (*found).second;


			if (handler)
			{
				(window->*handler)(windowMessage);

				return true;
			}
		}

		return false;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




