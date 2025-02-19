/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../runtime/runtime.hpp"

//===========================================================================
#include "WidgetEventDragDrop_HGlobal.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
HGLOBAL createHGlobal(const void* source_pointer, std::size_t source_size)
{
	HGLOBAL hGlobal;
	LPVOID  pointer;


	hGlobal = GlobalAlloc(GHND | GMEM_SHARE, source_size);
	CX_RUNTIME_LOG(LDbug) << L"GlobalAlloc()" << std::endl;
	if (nullptr == hGlobal)
	{
		CX_RUNTIME_LOG(LDbug) << L"GlobalAlloc() failed" << std::endl;
		return nullptr;
	}

	pointer = GlobalLock(hGlobal);
	if (nullptr == pointer)
	{
		GlobalFree(hGlobal);
		CX_RUNTIME_LOG(LDbug) << L"GlobalLock() failed" << std::endl;
		CX_RUNTIME_LOG(LDbug) << L"GlobalFree()" << std::endl;
		return nullptr;
	}

	memcpy(pointer, source_pointer, source_size);

	GlobalUnlock(hGlobal);


	return hGlobal;
}

HGLOBAL cloneHGlobal(HGLOBAL source_hglobal)
{
	HGLOBAL hglobal;

	std::size_t source_size;
	void* source_pointer;


	source_size = GlobalSize(source_hglobal);
	source_pointer = GlobalLock(source_hglobal);
	if (nullptr == source_pointer)
	{
		CX_RUNTIME_LOG(LDbug) << L"GlobalLock() failed : "
			<< L"source_hglobal=" << source_hglobal
			<< L", "
			<< L"source_size=" << source_size
			<< std::endl;
		return nullptr;
	}

	hglobal = createHGlobal(source_pointer, source_size);

	GlobalUnlock(source_hglobal);

	return hglobal;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


