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
PaintDC::PaintDC(HWND hwnd) :
	_hwnd{ hwnd }
{
	setDCHandle(::BeginPaint(_hwnd, &_ps));
	if (getDCHandle() == nullptr)
	{
		throw std::runtime_error("PaintDC::PaintDC(): BeginPaint() failed");
	}
}

PaintDC::~PaintDC()
{
	::EndPaint(_hwnd, &_ps);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ClientDC::ClientDC(HWND hwnd) :
	_hwnd{ hwnd }
{
	setDCHandle(::GetDC(_hwnd));
	if (getDCHandle() == nullptr)
	{
		throw std::runtime_error("ClientDC::ClientDC(): GetDC() failed");
	}
}

ClientDC::~ClientDC()
{
	::ReleaseDC(_hwnd, *this);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDC::WindowDC(HWND hwnd) :
	_hwnd{ hwnd }
{
	setDCHandle(::GetWindowDC(_hwnd));
	if (getDCHandle() == nullptr)
	{
		throw std::runtime_error("WindowDC::WindowDC(): GetWindowDC() failed");
	}
}

WindowDC::~WindowDC()
{
	::ReleaseDC(_hwnd, *this);
}





//////////////////////////////////////////////////////////////////////////////
//============================================================================
MemoryDC::MemoryDC(HDC hDC, const RECT& rcPaint)
{
	//-----------------------------------------------------------------------
	_hOriginalDC = hDC;
	_rcPaint = rcPaint;


	//-----------------------------------------------------------------------
	setDCHandle(::CreateCompatibleDC(_hOriginalDC));
	if (getDCHandle() == nullptr)
	{
		throw std::runtime_error("MemoryDC::MemoryDC(): CreateCompatibleDC() failed");
	}


	//-----------------------------------------------------------------------
	_hBitmap = ::CreateCompatibleBitmap(_hOriginalDC,
		_rcPaint.right  - _rcPaint.left, 
		_rcPaint.bottom - _rcPaint.top
	);
	if (_hBitmap == nullptr)
	{
		::DeleteDC(*this);
		throw std::runtime_error("MemoryDC::MemoryDC(): CreateCompatibleBitmap() failed");
	}


	//-----------------------------------------------------------------------
	_hOldBitmap = (HBITMAP)::SelectObject(*this, _hBitmap);


	//-----------------------------------------------------------------------
	::SetViewportOrgEx(*this, -_rcPaint.left, -_rcPaint.top, NULL);
}

MemoryDC::~MemoryDC()
{
	//-----------------------------------------------------------------------
	::BitBlt(
		_hOriginalDC,
		_rcPaint.left, _rcPaint.top,
		_rcPaint.right - _rcPaint.left, _rcPaint.bottom - _rcPaint.top,
		*this,
		_rcPaint.left, _rcPaint.top, 
		SRCCOPY
	);
	

	//-----------------------------------------------------------------------
	::SelectObject(*this, _hOldBitmap);
	::DeleteObject(_hBitmap);


	//-----------------------------------------------------------------------
	::DeleteDC(*this);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


