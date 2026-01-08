/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "DropSource.hpp"
#include "HGlobal.hpp"

//===========================================================================
#include "WindowDropSourceNotifier.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDropSourceData::WindowDropSourceData(std::uint32_t clipboardFormat) :
	_ClipboardFormat{ clipboardFormat }
{
}

//===========================================================================
void WindowDropSourceData::setData(std::uint8_t* pointer, std::size_t size)
{
	_DataBuffer.clear();
	_DataBuffer.insert(_DataBuffer.begin(),
		pointer,
		pointer + size
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDropSourceNotifier::WindowDropSourceNotifier()
{
}

WindowDropSourceNotifier::~WindowDropSourceNotifier()
{
}

//===========================================================================
bool WindowDropSourceNotifier::doDragDrop(WindowDropSourceData const& data)
{
	//-----------------------------------------------------------------------
	_Data = data;
	if (_Data._DataBuffer.empty())
	{
		return false;
	}


	//-----------------------------------------------------------------------
	bool  rv;


	//-----------------------------------------------------------------------
	rv = createCOMObject();
	if (!rv)
	{
		releaseCOMObject();
		return false;
	}


	//-----------------------------------------------------------------------
	std::uint8_t* pointer;
	std::size_t size;

	size = _Data._DataBuffer.size();
	pointer = _Data._DataBuffer.data();
	rv = setData(
		_pDataObject,
		pointer,
		size,
		_Data._ClipboardFormat
	);
	if (!rv)
	{
		releaseCOMObject();
		return false;
	}


	//-----------------------------------------------------------------------
	DWORD dwEffect;
	bool drop;


	rv = doDragDrop(dwEffect, drop);
	releaseCOMObject();
	if (false == rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
#if 0
	switch (dwEffect)
	{
	case DROPEFFECT_COPY:
		CX_RUNTIME_LOG(cxLDebug) << L"DROPEFFECT_COPY : "
			<< L"dwEffect=" << dwEffect
			;
		break;

	case DROPEFFECT_MOVE:
		CX_RUNTIME_LOG(cxLDebug) << L"DROPEFFECT_MOVE : "
			<< L"dwEffect=" << dwEffect
			;
		break;

	case DROPEFFECT_NONE:
		CX_RUNTIME_LOG(cxLDebug) << L"DROPEFFECT_NONE : "
			<< L"dwEffect=" << dwEffect
			;
		break;

	default:
		CX_RUNTIME_LOG(cxLDebug) << L"? : "
			<< L"dwEffect=" << dwEffect
			;
		break;
	}
#endif


	return true;
}

//===========================================================================
bool WindowDropSourceNotifier::createCOMObject(void)
{
	_pDropSource = cpp_new DropSource();
	if (!_pDropSource)
	{
		return false;
	}
	_pDataObject = cpp_new DataObject(_Data._ClipboardFormat);
	if (!_pDataObject)
	{
		return false;
	}

	return true;
}

void WindowDropSourceNotifier::releaseCOMObject(void)
{
	if (_pDropSource)
	{
		_pDropSource->Release();
		_pDropSource = nullptr;
	}
	if (_pDataObject)
	{
		_pDataObject->Release();
		_pDataObject = nullptr;
	}
}

//===========================================================================
bool WindowDropSourceNotifier::setData(
	IDataObject* pDataObject,
	const std::uint8_t* pointer,
	std::size_t size,
	std::uint32_t cf
)
{
	//-----------------------------------------------------------------------
	HGLOBAL hGlobal;
	hGlobal = createHGlobal(pointer, size);
	if (nullptr == hGlobal)
	{
		CX_RUNTIME_LOG(cxLDebug) << L"createHGlobal() failed : "
			<< L"pointer=" << pointer
			<< L", "
			<< L"size=" << size
			;
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;


	FORMATETC formatetc;
	STGMEDIUM medium;


	memset(&formatetc, 0, sizeof(formatetc));
	formatetc.cfFormat = cf;
	formatetc.ptd = nullptr;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex = -1;
	formatetc.tymed = TYMED_HGLOBAL;

	memset(&medium, 0, sizeof(medium));
	medium.tymed = TYMED_HGLOBAL;
	medium.hGlobal = hGlobal;
	medium.pUnkForRelease = nullptr;

	hr = pDataObject->SetData(&formatetc, &medium, TRUE);
	if (hr == S_OK)
	{
#if 0
		CX_RUNTIME_LOG(cxLDebug) << L"SetData() OK";
#endif
		return true;
	}
	CX_RUNTIME_LOG(cxLDebug) << L"SetData() failed";


	GlobalFree(hGlobal);
#if 0
	CX_RUNTIME_LOG(cxLDebug) << L"GlobalFree()";
#endif

	return false;
}

bool WindowDropSourceNotifier::doDragDrop(DWORD& dwEffect, bool& drop)
{
	HRESULT hr;


	hr = DoDragDrop(_pDataObject, _pDropSource, DROPEFFECT_COPY, &dwEffect);
	if (FAILED(hr))
	{
		CX_RUNTIME_LOG(cxLDebug) << L"DoDragDrop() failed : "
			<< L"hr=" << hr
			;
		return false;
	}


	/*
	hr =
		DRAGDROP_S_DROP
		DRAGDROP_S_CANCEL
		E_UNSPEC
	*/
	switch (hr)
	{
	case DRAGDROP_S_DROP:
		drop = true;
		break;

	default:
		drop = false;
		break;
	}


	if (DRAGDROP_S_DROP == hr || DRAGDROP_S_CANCEL == hr)
	{
		return true;
	}
	else
	{
		CX_RUNTIME_LOG(cxLDebug) << L"DoDragDrop() failed : "
			<< L"hr=" << hr
			;
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




