/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Context::Context(HWND hwnd, Factory* factory)
{
	_hwnd = hwnd;

	_pD2dFactory = factory->getD2dFactory();
	_pDWriteFactory = factory->getDWriteFactory();
	_pWICImagingFactory = factory->getWICImagingFactory();

#if 0
	bool rv;
	rv = createRenderTarget();
	if (!rv)
	{
		destroyRenderTarget();

		_pWICImagingFactory.reset();
		_pDWriteFactory.reset();
		_pD2dFactory.reset();

		throw std::runtime_error("Failed to createRenderTarget()");
	}
#endif
}

Context::~Context()
{
	destroyRenderTarget();

	_pWICImagingFactory.reset();
	_pDWriteFactory.reset();
	_pD2dFactory.reset();
}

//===========================================================================
bool Context::createRenderTarget(void)
{
	HRESULT hr;
	hr = getD2dFactory()->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_hwnd),
		_pD2dHwndRenderTarget.put()
	);
	if (FAILED(hr))
	{
		return false;
	}


	return true;
}

void Context::destroyRenderTarget(void)
{
	_pD2dHwndRenderTarget.reset();
}

//===========================================================================
wil::com_ptr_nothrow<ID2D1HwndRenderTarget>& Context::getD2dHwndRenderTarget(void)
{
	return _pD2dHwndRenderTarget;
}

//===========================================================================
wil::com_ptr_nothrow<ID2D1Factory>& Context::getD2dFactory(void)
{
	return _pD2dFactory;
}

wil::com_ptr_nothrow<IDWriteFactory>& Context::getDWriteFactory(void)
{
	return _pDWriteFactory;
}

wil::com_ptr_nothrow<IWICImagingFactory>& Context::getWICImagingFactory(void)
{
	return _pWICImagingFactory;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
