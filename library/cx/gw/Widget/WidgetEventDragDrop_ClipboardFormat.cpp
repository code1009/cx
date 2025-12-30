/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "WidgetEventDragDrop_ClipboardFormat.hpp"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ClipboardFormat::registerClipboardFormat(std::wstring name)
{
	_CustomClipboardFormat = RegisterClipboardFormatW(name.c_str());
	if (0 == _CustomClipboardFormat)
	{
		return false;
	}

	return true;
}

std::uint32_t ClipboardFormat::getClipboardFormat(void)
{
	return _CustomClipboardFormat;
}

void ClipboardFormat::setClipboardFormat(std::uint32_t cf)
{
	_CustomClipboardFormat = cf;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




