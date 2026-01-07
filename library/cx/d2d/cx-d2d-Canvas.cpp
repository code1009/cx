/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include <cx/runtime/runtime.hpp>
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Canvas::Canvas(Factory* factory, HWND hwnd, std::uint32_t cx, std::uint32_t cy)
{
	_Context = std::make_unique<Context>(hwnd, factory);
	_Renderer = std::make_unique<Renderer>(_Context.get(),  cx, cy);
	_DrawingSession = std::make_unique<DrawingSession>(_Context.get());

	_Renderer->drawingHandler =
		[this](DrawingSession* ds)
		{
			if (drawingHandler)
			{
				drawingHandler(ds);
			}
		};
}

Canvas::Canvas(Factory* factory, HWND hwnd)
{
	_Context = std::make_unique<Context>(hwnd, factory);
	_Renderer = std::make_unique<Renderer>(_Context.get());
	_DrawingSession = std::make_unique<DrawingSession>(_Context.get());

	_Renderer->drawingHandler =
		[this](DrawingSession* ds)
		{
			if (drawingHandler)
			{
				drawingHandler(ds);
			}
		};
}

//===========================================================================
Canvas::~Canvas()
{
	_Renderer.reset();
	_Context.reset();
}

//===========================================================================
void Canvas::resize(std::uint32_t width, std::uint32_t height)
{
	_Renderer->resize(width, height);
}

void Canvas::draw(void)
{
	_Renderer->render(_DrawingSession.get());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



