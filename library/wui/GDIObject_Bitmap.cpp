/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Bitmap::Bitmap(
	const std::wstring& filePath
)
{
	_BitmapHandle = loadBitmap(
		filePath
	);
	if (_BitmapHandle == nullptr)
	{
		throw std::runtime_error("Bitmap::Bitmap(): loadBitmap() failed");
	}
}

//===========================================================================
Bitmap::~Bitmap()
{
	::DeleteObject(_BitmapHandle);
}

//===========================================================================
HBITMAP Bitmap::loadBitmap(
	const std::wstring& filePath
)
{
	return (HBITMAP)::LoadImageW(NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

//===========================================================================
HBITMAP Bitmap::select(HDC hdc)
{
	return reinterpret_cast<HBITMAP>(::SelectObject(hdc, *this));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


