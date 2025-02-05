/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class context
{
protected:
	ID2D1Factory*      _pDFactory      { nullptr };
	IDWriteFactory*    _pDWriteFactory { nullptr };
	ID2D1RenderTarget* _pDRenderTarget { nullptr };

public:
	context() = default;
	virtual ~context() = default;

public:
	context(const context&) = delete;
	context& operator=(const context&) = delete;
	context(context&&) = delete;
	context& operator=(context&&) = delete;

public:
	[[nodiscard]] constexpr ID2D1Factory*      get_DFactory     (void) { return _pDFactory;      }
	[[nodiscard]] constexpr IDWriteFactory*    get_DWriteFactory(void) { return _pDWriteFactory; }
	[[nodiscard]] constexpr ID2D1RenderTarget* get_DRenderTarget(void) { return _pDRenderTarget; }

public:
	virtual void set_DFactory(ID2D1Factory* v);
	virtual void set_DWriteFactory(IDWriteFactory* v);
	virtual void set_DRenderTarget(ID2D1RenderTarget* v);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




