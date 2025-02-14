/////////////////////////////////////////////////////////////////////////////
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
	static ID2D1Factory*   _pDFactory;
	static IDWriteFactory* _pDWriteFactory;

public:
	static bool createFactory(void);
	static void destroyFactory(void);

public:
	static ID2D1Factory*   getDFactory(void);
	static IDWriteFactory* getDWriteFactory(void);

public:
	DirectX2DGraphic(const DirectX2DGraphic&) = delete;
	DirectX2DGraphic& operator=(const DirectX2DGraphic&) = delete;

	DirectX2DGraphic(DirectX2DGraphic&&) = delete;
	DirectX2DGraphic& operator=(DirectX2DGraphic&&) = delete;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




