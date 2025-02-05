/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class window
{
protected:
	HWND                      _hwnd     { nullptr };
	std::unique_ptr<viewport> _viewport {};
	ID2D1Factory*             _pDFactory         { nullptr };
	IDWriteFactory*           _pDWriteFactory    { nullptr };
	ID2D1HwndRenderTarget*    _pDHwndRenderTarget{ nullptr };

public:
	explicit window(HWND hwnd, bool center = true);

public:
	virtual ~window();

public:
	window(const window&) = delete;
	window& operator=(const window&) = delete;

	window(window&&) = delete;
	window& operator=(window&&) = delete;

public:
	[[nodiscard]] constexpr HWND               get_hwnd         (void) { return _hwnd; }
	[[nodiscard]] constexpr ID2D1Factory*      get_DFactory     (void) { return _pDFactory; }
	[[nodiscard]] constexpr IDWriteFactory*    get_DWriteFactory(void) { return _pDWriteFactory; }
	[[nodiscard]] constexpr ID2D1RenderTarget* get_DRenderTarget(void) { return _pDHwndRenderTarget; }
	[[nodiscard]] constexpr viewport*          get_viewport     (void) { return _viewport.get(); }

public:
	virtual void resize(std::int64_t width, std::int64_t height);

public:
	virtual void render(void);

protected:
	virtual bool create_device_resources(void);
	virtual void destroy_device_resources(void);

protected:
	virtual bool create_render_target(void);
	virtual void release_render_target(void);

protected:
	virtual void draw(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class basic_window : public window
{
protected:
	std::unique_ptr<document_grid>   _document_grid{};
	std::unique_ptr<status>          _status{};

public:
	explicit basic_window(HWND hwnd, bool center=true);

	//-----------------------------------------------------------------------
	// window
protected:
	virtual bool create_device_resources(void) override;
	virtual void destroy_device_resources(void) override;
	virtual void draw(void) override;

	//-----------------------------------------------------------------------
	// basic_window
public:
	[[nodiscard]] constexpr document_grid* get_document_grid(void) { return _document_grid.get(); }
	[[nodiscard]] constexpr status*        get_status       (void) { return _status.get(); }

public:
	virtual void draw_document(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




