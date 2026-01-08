/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "ClipboardFormat.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ClipboardFormat::registerClipboardFormat(std::wstring name)
{
	_CustomClipboardFormat = RegisterClipboardFormatW(name.c_str());
	if (0 == _CustomClipboardFormat)
	{
		CX_RUNTIME_LOG(cxLError) << L"RegisterClipboardFormat(): " << name << " failed";
		return false;
	}

#if 1
	CX_RUNTIME_LOG(cxLTrace) << L"RegisterClipboardFormat(): " << name;
#endif

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
