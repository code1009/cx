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

	_D2dFactory = factory->getD2dFactory();
	_DWriteFactory = factory->getDWriteFactory();
	_WICImagingFactory = factory->getWICImagingFactory();

	_DeviceResourceManager = std::make_unique<ResourceManager>();
	_DeviceIndependentResourceManager = std::make_unique<ResourceManager>();
	
#if 0
	bool rv;
	rv = createRenderTarget();
	if (!rv)
	{
		destroyRenderTarget();

		_WICImagingFactory.reset();
		_DWriteFactory.reset();
		_D2dFactory.reset();

		throw std::runtime_error("Failed to createRenderTarget()");
	}
#endif
}

Context::~Context()
{
	destroyRenderTarget();

	_WICImagingFactory.reset();
	_DWriteFactory.reset();
	_D2dFactory.reset();
}

//===========================================================================
wil::com_ptr_nothrow<ID2D1Factory>& Context::getD2dFactory(void)
{
	return _D2dFactory;
}

wil::com_ptr_nothrow<IDWriteFactory>& Context::getDWriteFactory(void)
{
	return _DWriteFactory;
}

wil::com_ptr_nothrow<IWICImagingFactory>& Context::getWICImagingFactory(void)
{
	return _WICImagingFactory;
}

//===========================================================================
bool Context::createRenderTarget(std::uint32_t cx, std::uint32_t cy)
{
	HRESULT hr;
	hr = getD2dFactory()->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_hwnd, D2D1::SizeU(cx, cy)),
		_D2dHwndRenderTarget.put()
	);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Context::createRenderTarget(void)
{
	return createRenderTarget(0, 0);
}

void Context::destroyRenderTarget(void)
{
	_D2dHwndRenderTarget.reset();
}

wil::com_ptr_nothrow<ID2D1HwndRenderTarget>& Context::getD2dHwndRenderTarget(void)
{
	return _D2dHwndRenderTarget;
}

//===========================================================================
bool Context::createResources(void)
{
	//--------------------------------------------------------------------------
	wil::com_ptr_nothrow<ID2D1SolidColorBrush> currentSolidColorBrush;
	wil::com_ptr_nothrow<ID2D1StrokeStyle> currentStrokeStyle;
	wil::com_ptr_nothrow<IDWriteTextFormat> currentTextFormat;

	HRESULT hr;


	//--------------------------------------------------------------------------
	hr = _D2dHwndRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		currentSolidColorBrush.put()
	);
	if (FAILED(hr))
	{
		return false;
	}


	//--------------------------------------------------------------------------
	hr = _D2dFactory->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_CAP_STYLE_FLAT,
			D2D1_LINE_JOIN_MITER,
			10.0f,
			D2D1_DASH_STYLE_SOLID,
			0.0f
		),
		nullptr,
		0,
		currentStrokeStyle.put()
	);
	if (FAILED(hr))
	{
		return false;
	}
	

	//--------------------------------------------------------------------------
	hr = _DWriteFactory->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12.0f,
		L"en-us",
		currentTextFormat.put()
	);
	if (FAILED(hr))
	{
		return false;
	}
	

	//--------------------------------------------------------------------------
	_CurrentSolidColorBrush = currentSolidColorBrush;
	_CurrentStrokeStyle = currentStrokeStyle;
	_CurrentTextFormat = currentTextFormat;

	return true;
}

void Context::destroyResources(void)
{
	_CurrentSolidColorBrush.reset();
	_CurrentStrokeStyle.reset();
	_CurrentTextFormat.reset();
}

wil::com_ptr_nothrow<ID2D1SolidColorBrush>& Context::getCurrentSolidColorBrush(void)
{
	return _CurrentSolidColorBrush;
}

wil::com_ptr_nothrow<ID2D1StrokeStyle>& Context::getCurrentStrokeStyle(void)
{
	return _CurrentStrokeStyle;
}

wil::com_ptr_nothrow<IDWriteTextFormat>& Context::getCurrentTextFormat(void)
{
	return _CurrentTextFormat;
}

//===========================================================================
ResourceManager* Context::getDeviceResourceManager(void)
{
	/*
	ID2D1Brush
	ID2D1Layer
	ID2D1RenderTarget
	...
	*/

	return _DeviceResourceManager.get();
}

ResourceManager* Context::getDeviceIndependentResourceManager(void)
{
	/*
	ID2D1DrawingStateBlock
	ID2D1Factory
	ID2D1Geometry
	ID2D1GeometrySink
	ID2D1SimplifiedGeometrySink
	ID2D1StrokeStyle
	*/

	return _DeviceIndependentResourceManager.get();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
