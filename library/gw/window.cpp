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
window::window(HWND hwnd, bool center):
	_hwnd{ hwnd }
{
	//-----------------------------------------------------------------------
	if (center)
	{
		_viewport = std::make_unique<center_viewport>(
			hwnd,
			std::bind(&window::render, this)
		);
	}
	else
	{
		_viewport = std::make_unique<viewport>(
			hwnd,
			std::bind(&window::render, this)
		);
	}


	//-----------------------------------------------------------------------
	_pDFactory = dx2d::getDFactory();
	_pDWriteFactory = dx2d::getDWriteFactory();
	_pDHwndRenderTarget = nullptr;
}

//===========================================================================
window::~window()
{
	destroy_device_resources();
}


//===========================================================================
void window::resize(std::int64_t width, std::int64_t height)
{
	if (_pDHwndRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		_pDHwndRenderTarget->Resize(
			D2D1::SizeU(static_cast<UINT32>(width), static_cast<UINT32>(height))
		);
	}

	_viewport->set_window_size(width, height);
}

//===========================================================================
void window::render(void)
{
	bool rv;


	rv = create_device_resources();
	if (rv)
	{
		_pDHwndRenderTarget->BeginDraw();


		draw();


		HRESULT hr;


		hr = _pDHwndRenderTarget->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			destroy_device_resources();
		}
	}
	else
	{
		destroy_device_resources();
	}
}

//===========================================================================
bool window::create_device_resources(void)
{
	bool rv;


	rv = create_render_target();
	if (!rv)
	{
		return false;
	}

	return true;
}

void window::destroy_device_resources(void)
{
	release_render_target();
}

//===========================================================================
bool window::create_render_target(void)
{
	HRESULT hr = S_OK;


	if (!_pDHwndRenderTarget)
	{
		//-------------------------------------------------------------------
		RECT rect;


		GetClientRect(_hwnd, &rect);


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
			D2D1::HwndRenderTargetProperties(_hwnd, size),
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

void window::release_render_target(void)
{
	if (_pDHwndRenderTarget)
	{
		_pDHwndRenderTarget->Release();
		_pDHwndRenderTarget = nullptr;
	}
}

//===========================================================================
void window::draw(void)
{
	//-----------------------------------------------------------------------
	D2D1::Matrix3x2F matrix;


	matrix = D2D1::Matrix3x2F::Identity();


	//-----------------------------------------------------------------------
	double translation_x;
	double translation_y;


	_viewport->get_document_viewport_point_translation(translation_x, translation_y);


	//-----------------------------------------------------------------------
	double scale;


	_viewport->get_scale(scale);


	//-----------------------------------------------------------------------
	matrix =
		D2D1::Matrix3x2F::Scale(
			static_cast<FLOAT>(scale),
			static_cast<FLOAT>(scale)
		);
	matrix =
		matrix *
		D2D1::Matrix3x2F::Translation(
			static_cast<FLOAT>(translation_x * scale),
			static_cast<FLOAT>(translation_y * scale)
		);
	_pDHwndRenderTarget->SetTransform(matrix);


	//-----------------------------------------------------------------------
	_pDHwndRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
basic_window::basic_window(HWND hwnd, bool center) :
	window{ hwnd, center }
{
	//-----------------------------------------------------------------------
	_document_grid = std::make_unique<document_grid>();
	_document_grid->set_visible_changed_handler(
		std::bind(&basic_window::render, this)
	);


	//-----------------------------------------------------------------------
	_status = std::make_unique<status>();
	_status->set_visible_changed_handler(
		std::bind(&basic_window::render, this)
	);


	//-----------------------------------------------------------------------
	_viewport->set_window_size(0, 0);
	_viewport->set_document_size(1920, 1080);
	_viewport->enable_scrollbar(true);


	//-----------------------------------------------------------------------
	//_document_grid->set_visible(true);
	//_status->set_visible(true);
}

//===========================================================================
bool basic_window::create_device_resources(void)
{
	bool rv;


	//-----------------------------------------------------------------------
	rv = window::create_device_resources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	rv = _status->create_device_resources(this);
	if (!rv)
	{
		return false;
	}
	rv = _document_grid->create_device_resources(this);
	if (!rv)
	{
		return false;
	}


	return true;
}

void basic_window::destroy_device_resources(void)
{
	//-----------------------------------------------------------------------
	_status->destroy_device_resources();
	_document_grid->destroy_device_resources();


	//-----------------------------------------------------------------------
	window::destroy_device_resources();
}

void basic_window::draw(void)
{
	//-----------------------------------------------------------------------
	window::draw();


	//-----------------------------------------------------------------------
	_document_grid->draw(this);


	//-----------------------------------------------------------------------
	draw_document();


	//-----------------------------------------------------------------------
	_status->draw(this);
}

void basic_window::draw_document(void)
{

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



