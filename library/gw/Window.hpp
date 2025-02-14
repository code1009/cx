/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Window
{
private:
	HWND                      _WindowHandle      { nullptr };
	std::unique_ptr<Viewport> _Viewport          {};
	ID2D1Factory*             _pDFactory         { nullptr };
	IDWriteFactory*           _pDWriteFactory    { nullptr };
	ID2D1HwndRenderTarget*    _pDHwndRenderTarget{ nullptr };

public:
	explicit Window(HWND hwnd, bool center = true);

public:
	virtual ~Window();

public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

public:
	[[nodiscard]] constexpr HWND               getWindowHandle (void) { return _WindowHandle;       }
	[[nodiscard]] constexpr ID2D1Factory*      getDFactory     (void) { return _pDFactory;          }
	[[nodiscard]] constexpr IDWriteFactory*    getDWriteFactory(void) { return _pDWriteFactory;     }
	[[nodiscard]] constexpr ID2D1RenderTarget* getDRenderTarget(void) { return _pDHwndRenderTarget; }
	[[nodiscard]] constexpr Viewport*          getViewport     (void) { return _Viewport.get();     }

public:
	virtual void render(void);

protected:
	virtual bool createDeviceResources(void);
	virtual void destroyDeviceResources(void);

protected:
	virtual bool createRenderTarget(void);
	virtual void releaseRenderTarget(void);

protected:
	virtual void draw(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




