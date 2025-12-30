#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Context
{
private:
	HWND _hwnd{ nullptr };

private:
	wil::com_ptr_nothrow<ID2D1HwndRenderTarget>	_pD2dHwndRenderTarget;

private:
	wil::com_ptr_nothrow<ID2D1Factory> _pD2dFactory;
	wil::com_ptr_nothrow<IDWriteFactory> _pDWriteFactory;
	wil::com_ptr_nothrow<IWICImagingFactory> _pWICImagingFactory;

public:
	explicit Context(HWND hwnd, Factory* factory);
	virtual ~Context();

public:
	Context(const Context&) = delete;
	Context& operator=(const Context&) = delete;

	Context(Context&&) = delete;
	Context& operator=(Context&&) = delete;

public:
	bool createRenderTarget(void);
	void destroyRenderTarget(void);

public:
	bool isValid(void) const noexcept
	{
		return (_pD2dHwndRenderTarget != nullptr);
	}

public:
	HWND getHwnd(void) const noexcept
	{
		return _hwnd;
	}

public:
	wil::com_ptr_nothrow<ID2D1HwndRenderTarget>& getD2dHwndRenderTarget(void);

public:
	wil::com_ptr_nothrow<ID2D1Factory>& getD2dFactory(void);
	wil::com_ptr_nothrow<IDWriteFactory>& getDWriteFactory(void);
	wil::com_ptr_nothrow<IWICImagingFactory>& getWICImagingFactory(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
