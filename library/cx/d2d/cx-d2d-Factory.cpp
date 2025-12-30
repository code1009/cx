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
Factory::Factory()
{
	createFactory();
}

Factory::~Factory()
{
	destroyFactory();
}

//===========================================================================
bool Factory::createFactory(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, _pD2dFactory.put());
	if (FAILED(hr))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		_pDWriteFactory.put_unknown()
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
		IID_PPV_ARGS(_pWICImagingFactory.put())
	);
	if (FAILED(hr))
	{
		destroyFactory();
		return false;
	}


	return true;
}

void Factory::destroyFactory(void)
{
	_pWICImagingFactory.reset();
	_pDWriteFactory.reset();
	_pD2dFactory.reset();
}

//===========================================================================
wil::com_ptr_nothrow<ID2D1Factory> Factory::getD2dFactory(void)
{
	return _pD2dFactory;
}

wil::com_ptr_nothrow<IDWriteFactory> Factory::getDWriteFactory(void)
{
	return _pDWriteFactory;
}

wil::com_ptr_nothrow<IWICImagingFactory> Factory::getWICImagingFactory(void)
{
	return _pWICImagingFactory;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
