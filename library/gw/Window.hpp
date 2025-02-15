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
	HWND                      _WindowHandle { nullptr };
	std::unique_ptr<Context>  _Context      {};
	std::unique_ptr<Viewport> _Viewport     {};

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
	[[nodiscard]] constexpr HWND      getWindowHandle (void) { return _WindowHandle;   }
	[[nodiscard]] constexpr Context*  getContext      (void) { return _Context.get();  }
	[[nodiscard]] constexpr Viewport* getViewport     (void) { return _Viewport.get(); }
	
public:
	virtual void render(void);

protected:
	virtual bool createDeviceResources(void);
	virtual void destroyDeviceResources(void);

protected:
	virtual bool createRenderTarget(void);
	virtual void destroyRenderTarget(void);

protected:
	virtual void draw(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




