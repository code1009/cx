#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class PaintDC : public DC
{
public:
	HWND        _hwnd { nullptr };
	PAINTSTRUCT _ps   { };

public:
	explicit PaintDC(HWND hwnd);

public:
	~PaintDC();
};





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class ClientDC : public DC
{
public:
	HWND _hwnd { nullptr };

public:
	explicit ClientDC(HWND hwnd);

public:
	~ClientDC();
};





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class WindowDC : public DC
{
public:
	HWND _hwnd { nullptr };

public:
	explicit WindowDC(HWND hwnd);

public:
	~WindowDC();
};





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class MemoryDC : public DC
{
public:
	HDC     _hOriginalDC{ nullptr };
	RECT    _rcPaint    { 0, 0, 0, 0 };
	HBITMAP _hBitmap    { nullptr };
	HBITMAP _hOldBitmap { nullptr };

public:
	explicit MemoryDC(HDC hDC, const RECT& rcPaint);

public:
	~MemoryDC();
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


