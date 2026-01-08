/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "ClipboardFormat.hpp"

//===========================================================================
#include "WindowDragDropClipboardFormat.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool WindowDragDropClipboardFormat::initialize(void)
{
	static const wchar_t* name = L"cx.wui.dragdrop={4EF1BA30-54D7-4F6F-A169-0B10005E9F08}";

	bool rv;
	rv = _ClipboardFormat.registerClipboardFormat(name);
	if (false == rv)
	{
		return false;
	}

	return true;
}

std::uint32_t WindowDragDropClipboardFormat::getClipboardFormat(void)
{
	return _ClipboardFormat.getClipboardFormat();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDragDropClipboardFormat* getWindowDragDropClipboardFormat(void)
{
	static WindowDragDropClipboardFormat instance;
	return &instance;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




