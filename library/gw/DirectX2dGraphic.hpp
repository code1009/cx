/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DirectX2dGraphic
{
private:
	static ID2D1Factory*       _pD2dFactory;
	static IDWriteFactory*     _pDWriteFactory;
	static IWICImagingFactory* _pWICImagingFactory;

public:
	static bool createFactory(void);
	static void destroyFactory(void);

public:
	static ID2D1Factory*       getD2dFactory(void);
	static IDWriteFactory*     getDWriteFactory(void);
	static IWICImagingFactory* getWICImagingFactory(void);

public:
	DirectX2dGraphic(const DirectX2dGraphic&) = delete;
	DirectX2dGraphic& operator=(const DirectX2dGraphic&) = delete;

	DirectX2dGraphic(DirectX2dGraphic&&) = delete;
	DirectX2dGraphic& operator=(DirectX2dGraphic&&) = delete;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




