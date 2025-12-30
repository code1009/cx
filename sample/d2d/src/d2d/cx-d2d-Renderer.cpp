/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include <cx/runtime/runtime.hpp>
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Renderer::Renderer(Context* context)
	: _Context{ context }
{
}

//===========================================================================
Renderer::~Renderer()
{
	destroyDeviceResources();
	destroyDeviceIndependentResources();
}

//===========================================================================
void Renderer::resize(std::uint32_t width, std::uint32_t height)
{
	_Width = width;
	_Height = height;


	if (_Context->getD2dHwndRenderTarget())
	{
		_Context->getD2dHwndRenderTarget()->Resize(
			D2D1::SizeU(
				static_cast<UINT32>(width),
				static_cast<UINT32>(height)
			)
		);

		CX_RUNTIME_LOG(cxLInfo)
			<< L"width=" << width
			<< L" "
			<< L"height=" << height
			;
	}
}

//===========================================================================
void Renderer::render(void)
{
	bool rv;


	rv = createDeviceResources();
	if (rv)
	{
		_Context->getD2dHwndRenderTarget()->BeginDraw();


		draw();


		HRESULT hr;
		hr = _Context->getD2dHwndRenderTarget()->EndDraw();
		if (hr == D2DERR_RECREATE_TARGET)
		{
			CX_RUNTIME_LOG(cxLInfo)
				<< L"D2DERR_RECREATE_TARGET" << std::endl
				;
			destroyDeviceResources();
		}
	}
	else
	{
		destroyDeviceResources();
	}
}

//===========================================================================
bool Renderer::createDeviceIndependentResources(void)
{
	return true;
}

void Renderer::destroyDeviceIndependentResources(void)
{
}

//===========================================================================
bool Renderer::createDeviceResources(void)
{
	if (_Context->isValid())
	{
		return true;
	}


	bool rv;

	rv = _Context->createRenderTarget(_Width, _Height);
	if (!rv)
	{
		return false;
	}

	CX_RUNTIME_LOG(cxLInfo)
		<< L"createDeviceResources()"
		;

	return true;
}

void Renderer::destroyDeviceResources(void)
{
	CX_RUNTIME_LOG(cxLInfo)
		<< L"destroyDeviceResources()"
		;

	_Context->destroyRenderTarget();
}

//===========================================================================
void Renderer::draw(void)
{
#if 0
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
	_Context->getD2dRenderTarget()->SetTransform(matrix);


	//-----------------------------------------------------------------------
	_Context->getD2dRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::White));
#endif

	_Context->getD2dHwndRenderTarget()->Clear(D2D1::ColorF(D2D1::ColorF::Yellow));


	wil::com_ptr_nothrow<ID2D1SolidColorBrush> solidColorBrush;

	_Context->getD2dHwndRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		solidColorBrush.put()
	);

	_Context->getD2dHwndRenderTarget()->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(100.0f, 100.0f),
			50.0f,
			50.0f
		),
		solidColorBrush.get(),
		5.0f
	);

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



