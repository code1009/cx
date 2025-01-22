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
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ID2D1Factory*   dx2d::_pDFactory = nullptr;
IDWriteFactory* dx2d::_pDWriteFactory = nullptr;

//===========================================================================
bool dx2d::new_factory(void)
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
		delete_factory();
		return false;
	}


	return true;
}

void dx2d::delete_factory(void)
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
ID2D1Factory* dx2d::getDFactory(void)
{
	return _pDFactory;
}

IDWriteFactory* dx2d::getDWriteFactory(void)
{
	return _pDWriteFactory;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




