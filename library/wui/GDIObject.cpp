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
Font::Font(
	const std::wstring& faceName,
	int pointSize
)
{
	_FontHandle = createFont(
		faceName, 
		pointSize
	);
	if (_FontHandle == nullptr)
	{
		throw std::runtime_error("Font::Font(): createFont() failed");
	}
}

//===========================================================================
Font::~Font()
{
	::DeleteObject(_FontHandle);
}

//===========================================================================
HFONT Font::createFont(
	const std::wstring& faceName,
	int pointSize
)
{
	//-----------------------------------------------------------------------
	HDC   hdc;
	POINT pt    = { 0, 0 };
	POINT ptOrg = { 0, 0 };
	int   lpsy;


	// 72 points/inch, 10 decipoints/point
	hdc  = ::GetDC(NULL);
	lpsy = ::GetDeviceCaps(hdc, LOGPIXELSY);
	pt.y = ::MulDiv(lpsy, pointSize, 720);
	::DPtoLP(hdc, &pt   , 1);
	::DPtoLP(hdc, &ptOrg, 1);
	::ReleaseDC(NULL, hdc);


	//-----------------------------------------------------------------------
	LOGFONTW logFont = {};


	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight  = -abs(pt.y - ptOrg.y);
	lstrcpyW(logFont.lfFaceName, faceName.c_str());

	return ::CreateFontIndirectW(&logFont);
}

//===========================================================================
HFONT Font::select(HDC hdc)
{
	return reinterpret_cast<HFONT>(::SelectObject(hdc, *this));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


