/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void WindowMessageLoop::addIdleHandler(IdleHandler handler)
{
	_idleHandlers.push_back(handler);
}

//===========================================================================
void WindowMessageLoop::run(HACCEL hAccelTable)
{
	bool hasIdleHandlers = !_idleHandlers.empty();
	bool hasAccelTable = (hAccelTable != nullptr);


	if (hasIdleHandlers && hasAccelTable)
	{
		runWithIdleAndAccel(hAccelTable);
	}
	else if (hasIdleHandlers && !hasAccelTable)
	{
		runWithIdle();
	}
	else if (!hasIdleHandlers && hasAccelTable)
	{
		runWithAccel(hAccelTable);
	}
	else
	{
		runStandard();
	}
}

//===========================================================================
void WindowMessageLoop::runStandard(void)
{
	MSG msg;
	BOOL rv;

	while (true)
	{
		rv = ::GetMessageW(&msg, nullptr, 0, 0);
		if (rv == -1)
		{
			// Error 발생
			break;
		}
		else if (rv == 0)
		{
			// WM_QUIT 수신
			break;
		}

		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}
}

void WindowMessageLoop::runWithAccel(HACCEL hAccelTable)
{
	MSG msg;
	BOOL rv;

	while (true)
	{
		rv = ::GetMessageW(&msg, nullptr, 0, 0);
		if (rv == -1)
		{
			// Error 발생
			break;
		}
		else if (rv == 0)
		{
			// WM_QUIT 수신
			break;
		}

		if (!::TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}
	}
}

void WindowMessageLoop::runWithIdle(void)
{
	MSG msg;
	BOOL rv;

	while (true)
	{
		rv = ::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE);
		if (rv != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}
		else
		{
			processIdleHandlers();
		}
	}
}

void WindowMessageLoop::runWithIdleAndAccel(HACCEL hAccelTable)
{
	MSG msg;
	BOOL rv;

	while (true)
	{
		rv = ::PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE);
		if (rv != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			if (!::TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
		else
		{
			processIdleHandlers();
		}
	}
}

//===========================================================================
void WindowMessageLoop::processIdleHandlers(void)
{
	for (auto& handler : _idleHandlers)
	{
		handler();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




