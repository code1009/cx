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
ID2D1Factory*   DirectX2DGraphic::_pDFactory = nullptr;
IDWriteFactory* DirectX2DGraphic::_pDWriteFactory = nullptr;

//===========================================================================
bool DirectX2DGraphic::createFactory(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pDFactory);
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


	return true;
}

void DirectX2DGraphic::destroyFactory(void)
{
	if (_pDFactory)
	{
		_pDFactory->Release();
		_pDFactory = nullptr;
	}


	if (_pDWriteFactory)
	{
		_pDWriteFactory->Release();
		_pDWriteFactory = nullptr;
	}
}

//===========================================================================
ID2D1Factory* DirectX2DGraphic::getDFactory(void)
{
	return _pDFactory;
}

IDWriteFactory* DirectX2DGraphic::getDWriteFactory(void)
{
	return _pDWriteFactory;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




