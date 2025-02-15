/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Window::Window(HWND hwnd, bool center):
	_WindowHandle{ hwnd }
{
	//-----------------------------------------------------------------------
	_Context = std::make_unique<Context>();


	//-----------------------------------------------------------------------
	if (center)
	{
		_Viewport = std::make_unique<CenterViewport>(
			hwnd,
			std::bind(&Window::render, this),
			[this](std::int64_t width, std::int64_t height)
			{
				if (getContext()->getD2dHwndRenderTarget())
				{
					getContext()->getD2dHwndRenderTarget()->Resize(
						D2D1::SizeU(
							static_cast<UINT32>(width),
							static_cast<UINT32>(height)
						)
					);
				}
			}
		);
	}
	else
	{
		_Viewport = std::make_unique<Viewport>(
			hwnd,
			std::bind(&Window::render, this),
			[this](std::int64_t width, std::int64_t height)
			{
				if (getContext()->getD2dHwndRenderTarget())
				{
					getContext()->getD2dHwndRenderTarget()->Resize(
						D2D1::SizeU(
							static_cast<UINT32>(width),
							static_cast<UINT32>(height)
						)
					);
				}
			}
		);
	}


	//-----------------------------------------------------------------------
	getContext()->setD2dFactory(DirectX2DGraphic::getD2dFactory());
	getContext()->setDWriteFactory(DirectX2DGraphic::getDWriteFactory());
	getContext()->setWICImagingFactory(DirectX2DGraphic::getWICImagingFactory());
	getContext()->setD2dRenderTarget(nullptr);
	getContext()->setD2dHwndRenderTarget(nullptr);
}

//===========================================================================
Window::~Window()
{
	destroyDeviceResources();
}

//===========================================================================
#if 0
void Window::resize(std::int64_t width, std::int64_t height)
{
	if (_pD2dHwndRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		_pD2dHwndRenderTarget->Resize(
			D2D1::SizeU(
				static_cast<UINT32>(width),
				static_cast<UINT32>(height)
			)
		);
	}

	_Viewport->setWindowSize(width, height);
}
#endif

//===========================================================================
void Window::render(void)
{
	bool rv;


	rv = createDeviceResources();
	if (rv)
	{
		getContext()->getD2dHwndRenderTarget()->BeginDraw();


		draw();


		HRESULT hr;


		hr = getContext()->getD2dHwndRenderTarget()->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			destroyDeviceResources();
		}
	}
	else
	{
		destroyDeviceResources();
	}
}

//===========================================================================
bool Window::createDeviceResources(void)
{
	bool rv;


	rv = createRenderTarget();
	if (!rv)
	{
		return false;
	}

	return true;
}

void Window::destroyDeviceResources(void)
{
	destroyRenderTarget();
}

//===========================================================================
bool Window::createRenderTarget(void)
{
	HRESULT hr = S_OK;


	if (getContext()->getD2dHwndRenderTarget())
	{
		return true;
	}


	//-------------------------------------------------------------------
	RECT rect;


	GetClientRect(_WindowHandle, &rect);


	//-------------------------------------------------------------------
	D2D1_SIZE_U size = D2D1::SizeU(
		rect.right - rect.left,
		rect.bottom - rect.top
	);


	//-------------------------------------------------------------------
	ID2D1HwndRenderTarget* _pD2dHwndRenderTarget{ nullptr };

#if 0
	constexpr int DEFAULT_DPI{ 96 };
	hr = getContext()->getD2dFactory()->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(),
			DEFAULT_DPI, DEFAULT_DPI,
			D2D1_RENDER_TARGET_USAGE_NONE,
			D2D1_FEATURE_LEVEL_DEFAULT
		),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&_pD2dHwndRenderTarget
	);
#else
	hr = getContext()->getD2dFactory()->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_WindowHandle, size),
		&_pD2dHwndRenderTarget
	);
#endif
	if (FAILED(hr))
	{
		return false;
	}

	getContext()->setD2dHwndRenderTarget(_pD2dHwndRenderTarget);
	getContext()->setD2dRenderTarget(_pD2dHwndRenderTarget);

	return true;
}

void Window::destroyRenderTarget(void)
{
	if (getContext()->getD2dHwndRenderTarget())
	{
		getContext()->getD2dHwndRenderTarget()->Release();

		getContext()->setD2dHwndRenderTarget(nullptr);
		getContext()->setD2dRenderTarget(nullptr);
	}
}

//===========================================================================
void Window::draw(void)
{
	//-----------------------------------------------------------------------
	D2D1::Matrix3x2F matrix;


	matrix = D2D1::Matrix3x2F::Identity();


	//-----------------------------------------------------------------------
	double translationX;
	double translationY;


	_Viewport->getDocumentViewportPointTranslation(translationX, translationY);


	//-----------------------------------------------------------------------
	double scale;


	_Viewport->getScale(scale);


	//-----------------------------------------------------------------------
	matrix =
		D2D1::Matrix3x2F::Scale(
			static_cast<FLOAT>(scale),
			static_cast<FLOAT>(scale)
		);
	matrix =
		matrix *
		D2D1::Matrix3x2F::Translation(
			static_cast<FLOAT>(translationX * scale),
			static_cast<FLOAT>(translationY * scale)
		);
	getContext()->getD2dRenderTarget()->SetTransform(matrix);


	//-----------------------------------------------------------------------
	getContext()->getD2dRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



