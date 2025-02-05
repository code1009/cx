/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Window::Window(HWND hwnd, bool center):
	_WindowHandle{ hwnd }
{
	//-----------------------------------------------------------------------
	if (center)
	{
		_Viewport = std::make_unique<CenterViewport>(
			hwnd,
			std::bind(&Window::render, this),
			[this](std::int64_t width, std::int64_t height)
			{
				if (_pDHwndRenderTarget)
				{
					_pDHwndRenderTarget->Resize(
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
				if (_pDHwndRenderTarget)
				{
					_pDHwndRenderTarget->Resize(
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
	_pDFactory = DirectX2DGraphic::getDFactory();
	_pDWriteFactory = DirectX2DGraphic::getDWriteFactory();
	_pDHwndRenderTarget = nullptr;
}

//===========================================================================
Window::~Window()
{
	destroyDeviceResources();
}


//===========================================================================
void Window::resize(std::int64_t width, std::int64_t height)
{
#if 0
	if (_pDHwndRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		_pDHwndRenderTarget->Resize(
			D2D1::SizeU(
				static_cast<UINT32>(width),
				static_cast<UINT32>(height)
			)
		);
	}
#endif

	_Viewport->setWindowSize(width, height);
}

//===========================================================================
void Window::render(void)
{
	bool rv;


	rv = createDeviceResources();
	if (rv)
	{
		_pDHwndRenderTarget->BeginDraw();


		draw();


		HRESULT hr;


		hr = _pDHwndRenderTarget->EndDraw();
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
	releaseRenderTarget();
}

//===========================================================================
bool Window::createRenderTarget(void)
{
	HRESULT hr = S_OK;


	if (!_pDHwndRenderTarget)
	{
		//-------------------------------------------------------------------
		RECT rect;


		GetClientRect(_WindowHandle, &rect);


		//-------------------------------------------------------------------
		D2D1_SIZE_U size = D2D1::SizeU(
			rect.right - rect.left,
			rect.bottom - rect.top
		);

#if 0
		constexpr int DEFAULT_DPI{ 96 };
		hr = _pDFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(),
				DEFAULT_DPI, DEFAULT_DPI,
				D2D1_RENDER_TARGET_USAGE_NONE,
				D2D1_FEATURE_LEVEL_DEFAULT
			),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&_pDHwndRenderTarget
		);
#else
		hr = _pDFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(_WindowHandle, size),
			&_pDHwndRenderTarget
		);
#endif
		if (FAILED(hr))
		{
			return false;
		}
	}


	return true;
}

void Window::releaseRenderTarget(void)
{
	if (_pDHwndRenderTarget)
	{
		_pDHwndRenderTarget->Release();
		_pDHwndRenderTarget = nullptr;
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
	_pDHwndRenderTarget->SetTransform(matrix);


	//-----------------------------------------------------------------------
	_pDHwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



