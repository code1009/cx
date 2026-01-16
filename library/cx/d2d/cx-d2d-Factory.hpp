#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Factory
{
private:
	wil::com_ptr_nothrow<ID2D1Factory> _D2dFactory;
	wil::com_ptr_nothrow<IDWriteFactory> _DWriteFactory;
	wil::com_ptr_nothrow<IWICImagingFactory> _WICImagingFactory;

public:
	Factory();
	virtual ~Factory();

public:
	Factory(const Factory&) = delete;
	Factory& operator=(const Factory&) = delete;

	Factory(Factory&&) = delete;
	Factory& operator=(Factory&&) = delete;

private:
	bool createFactory(void);
	void destroyFactory(void);

public:
	wil::com_ptr_nothrow<ID2D1Factory> getD2dFactory(void);
	wil::com_ptr_nothrow<IDWriteFactory> getDWriteFactory(void);
	wil::com_ptr_nothrow<IWICImagingFactory> getWICImagingFactory(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
