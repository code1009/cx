/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "HGlobal.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
HGLOBAL createHGlobal(const void* source_pointer, std::size_t source_size)
{
	HGLOBAL hGlobal;
	LPVOID  pointer;


	hGlobal = GlobalAlloc(GHND | GMEM_SHARE, source_size);
	CX_RUNTIME_LOG(cxLDebug) << L"GlobalAlloc()" << std::endl;
	if (nullptr == hGlobal)
	{
#if 1
		CX_RUNTIME_LOG(cxLDebug) << L"GlobalAlloc() failed" << std::endl;
#endif
		return nullptr;
	}

	pointer = GlobalLock(hGlobal);
	if (nullptr == pointer)
	{
#if 1
		CX_RUNTIME_LOG(cxLDebug) << L"GlobalLock() failed" << std::endl;
#endif

		GlobalFree(hGlobal);
#if 1
		CX_RUNTIME_LOG(cxLDebug) << L"GlobalFree()" << std::endl;
#endif

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
#if 1
		CX_RUNTIME_LOG(cxLDebug) << L"GlobalLock() failed : "
			<< L"source_hglobal=" << source_hglobal
			<< L", "
			<< L"source_size=" << source_size
			<< std::endl;
#endif
		return nullptr;
	}

	hglobal = createHGlobal(source_pointer, source_size);

	GlobalUnlock(source_hglobal);

	return hglobal;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


