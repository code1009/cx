/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Context
{
protected:
	ID2D1Factory*       _pD2dFactory       { nullptr };
	IDWriteFactory*     _pDWriteFactory    { nullptr };
	IWICImagingFactory* _pWICImagingFactory{ nullptr };

protected:
	ID2D1RenderTarget*     _pD2dRenderTarget    { nullptr };
	ID2D1HwndRenderTarget* _pD2dHwndRenderTarget{ nullptr };

public:
	Context() = default;
	virtual ~Context() = default;

public:
	Context(const Context&) = delete;
	Context& operator=(const Context&) = delete;
	Context(Context&&) = delete;
	Context& operator=(Context&&) = delete;

public:
	[[nodiscard]] constexpr ID2D1Factory*       getD2dFactory       (void) { return _pD2dFactory; }
	[[nodiscard]] constexpr IDWriteFactory*     getDWriteFactory    (void) { return _pDWriteFactory; }
	[[nodiscard]] constexpr IWICImagingFactory* getWICImagingFactory(void) { return _pWICImagingFactory; }

public:
	[[nodiscard]] constexpr ID2D1RenderTarget*     getD2dRenderTarget    (void) { return _pD2dRenderTarget; }
	[[nodiscard]] constexpr ID2D1HwndRenderTarget* getD2dHwndRenderTarget(void) { return _pD2dHwndRenderTarget; }

public:
	virtual void setD2dFactory(ID2D1Factory* v);
	virtual void setDWriteFactory(IDWriteFactory* v);
	virtual void setWICImagingFactory(IWICImagingFactory* v);

public:
	virtual void setD2dRenderTarget(ID2D1RenderTarget* v);
	virtual void setD2dHwndRenderTarget(ID2D1HwndRenderTarget* v);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




