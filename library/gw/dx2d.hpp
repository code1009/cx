/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class dx2d
{
private:
	static ID2D1Factory*   _pDFactory;
	static IDWriteFactory* _pDWriteFactory;

public:
	static bool new_factory(void);
	static void delete_factory(void);

public:
	static ID2D1Factory*   getDFactory(void);
	static IDWriteFactory* getDWriteFactory(void);

public:
	dx2d(const dx2d&) = delete;
	dx2d& operator=(const dx2d&) = delete;

	dx2d(dx2d&&) = delete;
	dx2d& operator=(dx2d&&) = delete;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




