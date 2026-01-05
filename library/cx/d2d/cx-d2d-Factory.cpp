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
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, _D2dFactory.put());
	if (FAILED(hr))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		_DWriteFactory.put_unknown()
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
		IID_PPV_ARGS(_WICImagingFactory.put())
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
	_WICImagingFactory.reset();
	_DWriteFactory.reset();
	_D2dFactory.reset();
}

//===========================================================================
wil::com_ptr_nothrow<ID2D1Factory> Factory::getD2dFactory(void)
{
	return _D2dFactory;
}

wil::com_ptr_nothrow<IDWriteFactory> Factory::getDWriteFactory(void)
{
	return _DWriteFactory;
}

wil::com_ptr_nothrow<IWICImagingFactory> Factory::getWICImagingFactory(void)
{
	return _WICImagingFactory;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
