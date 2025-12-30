/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment (lib, "D2d1.lib")
#pragma comment (lib, "Dwrite.lib")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ID2D1Factory*       DirectX2dGraphic::_pD2dFactory        = nullptr;
IDWriteFactory*     DirectX2dGraphic::_pDWriteFactory     = nullptr;
IWICImagingFactory* DirectX2dGraphic::_pWICImagingFactory = nullptr;

//===========================================================================
bool DirectX2dGraphic::createFactory(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pD2dFactory);
	if (FAILED(hr))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&_pDWriteFactory)
	);
	if (FAILED(hr))
	{
		destroyFactory();
		return false;
	}


	//-----------------------------------------------------------------------
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_pWICImagingFactory)
	);
	if (FAILED(hr))
	{
		destroyFactory();
		return false;
	}


	return true;
}

void DirectX2dGraphic::destroyFactory(void)
{
	if (_pD2dFactory)
	{
		_pD2dFactory->Release();
		_pD2dFactory = nullptr;
	}


	if (_pDWriteFactory)
	{
		_pDWriteFactory->Release();
		_pDWriteFactory = nullptr;
	}


	if (_pWICImagingFactory)
	{
		_pWICImagingFactory->Release();
		_pWICImagingFactory = nullptr;
	}
}

//===========================================================================
ID2D1Factory* DirectX2dGraphic::getD2dFactory(void)
{
	return _pD2dFactory;
}

IDWriteFactory* DirectX2dGraphic::getDWriteFactory(void)
{
	return _pDWriteFactory;
}

IWICImagingFactory* DirectX2dGraphic::getWICImagingFactory(void)
{
	return _pWICImagingFactory;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




