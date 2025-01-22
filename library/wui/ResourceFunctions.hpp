#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring loadString(int id, HINSTANCE hInstance=nullptr);

//===========================================================================
LPCWSTR makeIntResource(int id);

//===========================================================================
HCURSOR loadCursor(int id, HINSTANCE hInstance = nullptr);
HICON loadIcon(int id, HINSTANCE hInstance = nullptr);
HBITMAP loadBitmap(int id, HINSTANCE hInstance = nullptr);
HACCEL loadAccelerators(int id, HINSTANCE hInstance = nullptr);

HCURSOR loadCursor(LPCWSTR id, HINSTANCE hInstance = nullptr);
HICON loadIcon(LPCWSTR id, HINSTANCE hInstance = nullptr);
HBITMAP loadBitmap(LPCWSTR id, HINSTANCE hInstance = nullptr);
HACCEL loadAccelerators(LPCWSTR id, HINSTANCE hInstance = nullptr);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


