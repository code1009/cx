﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DirectX2DGraphic
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
	DirectX2DGraphic(const DirectX2DGraphic&) = delete;
	DirectX2DGraphic& operator=(const DirectX2DGraphic&) = delete;

	DirectX2DGraphic(DirectX2DGraphic&&) = delete;
	DirectX2DGraphic& operator=(DirectX2DGraphic&&) = delete;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




