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
	: _Context{ context },
	_cx{ 0 },
	_cy{ 0 }
{
}

Renderer::Renderer(Context* context, std::uint32_t cx, std::uint32_t cy)
	: _Context{ context }
	, _cx{ cx }
	, _cy{ cy }
{
}

//===========================================================================
Renderer::~Renderer()
{
	destroyDeviceResources();
	destroyDeviceIndependentResources();
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

	rv = _Context->createRenderTarget(_cx, _cy);
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
void Renderer::resize(std::uint32_t cx, std::uint32_t cy)
{
	_cx = cx;
	_cy = cy;


	if (_Context->getD2dHwndRenderTarget())
	{
		_Context->getD2dHwndRenderTarget()->Resize(
			D2D1::SizeU(
				static_cast<UINT32>(_cx),
				static_cast<UINT32>(_cy)
			)
		);
#if 0
		CX_RUNTIME_LOG(cxLInfo)
			<< L"cx=" << _cx
			<< L" "
			<< L"cy=" << _cy
			<< L" "
			<< L"RenderTarget"
			;
#endif
	}
	else
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"cx=" << _cx
			<< L" "
			<< L"cy=" << _cy
			;
	}
}

//===========================================================================
void Renderer::render(DrawingSession* ds)
{
	bool rv;


	rv = createDeviceResources();
	if (rv)
	{
		_Context->getD2dHwndRenderTarget()->BeginDraw();


		draw(ds);


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
void Renderer::draw(DrawingSession* ds)
{
	if (onDraw)
	{
		onDraw(ds);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



