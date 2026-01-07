/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "DropSource.hpp"
#include "HGlobal.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
// 
// class DataObject
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DataObject::DataObject(UINT clipboard_format)
{
	_RefCount = 1;

	_hGlobal = nullptr;

	_EnumCount = 0;

	_ClipboardFormat = clipboard_format;
}

DataObject::~DataObject()
{
	if (_hGlobal != nullptr)
	{
		GlobalFree(_hGlobal); 
		CX_RUNTIME_LOG(cxLDebug) << L"GlobalFree()" << std::endl;

		_hGlobal = nullptr;
	}
}

//===========================================================================
// IUnknown
STDMETHODIMP DataObject::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = nullptr;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDataObject))
	{
		*ppvObject = static_cast<IDataObject*>(this);
	}
	else if (IsEqualIID(riid, IID_IEnumFORMATETC))
	{
		*ppvObject = static_cast<IEnumFORMATETC*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) DataObject::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}

STDMETHODIMP_(ULONG) DataObject::Release()
{
	if (InterlockedDecrement(&_RefCount) == 0)
	{
		cpp_delete this;
		return 0;
	}

	return _RefCount;
}

//===========================================================================
// IDataObject
STDMETHODIMP DataObject::GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium)
{
	if (pformatetcIn->cfFormat == _ClipboardFormat)
	{
		pmedium->tymed          = TYMED_HGLOBAL;
		pmedium->hGlobal        = _hGlobal;
		pmedium->pUnkForRelease = nullptr;

		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP DataObject::GetDataHere(FORMATETC* pformatetc, STGMEDIUM* pmedium)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject::QueryGetData(FORMATETC* pformatetc)
{
	if (pformatetc->cfFormat == _ClipboardFormat)
	{
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP DataObject::GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease)
{
	if (pformatetc->cfFormat == _ClipboardFormat)
	{
		if (_hGlobal != nullptr)
		{
			GlobalFree(_hGlobal); 
			CX_RUNTIME_LOG(cxLDebug) << L"GlobalFree()" << std::endl;
			_hGlobal = nullptr;
		}


		_hGlobal = cloneHGlobal(pmedium->hGlobal);


		if (fRelease)
		{
			GlobalFree(pmedium->hGlobal); 
			CX_RUNTIME_LOG(cxLDebug) << L"GlobalFree()" << std::endl;
		}


		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP DataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc)
{
	if (dwDirection == DATADIR_GET)
	{
		return QueryInterface(IID_PPV_ARGS(ppenumFormatEtc));
	}

	return E_NOTIMPL;
}

STDMETHODIMP DataObject::DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject::DUnadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject::EnumDAdvise(IEnumSTATDATA** ppenumAdvise)
{
	return E_NOTIMPL;
}

//===========================================================================
// IEnumFORMATETC
STDMETHODIMP DataObject::Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched)
{
	FORMATETC formatetc[] =
	{
		{
			static_cast<std::uint16_t>(_ClipboardFormat), 
			nullptr, 
			DVASPECT_CONTENT, 
			-1, 
			TYMED_HGLOBAL
		}
	};


	if (_EnumCount >= 1)
	{
		return S_FALSE;
	}


	*rgelt = formatetc[_EnumCount];


	if (pceltFetched != nullptr)
	{
		*pceltFetched = 1;
	}


	_EnumCount++;


	return S_OK;
}

STDMETHODIMP DataObject::Skip(ULONG celt)
{
	return E_NOTIMPL;
}

STDMETHODIMP DataObject::Reset(VOID)
{
	_EnumCount = 0;

	return S_OK;
}

STDMETHODIMP DataObject::Clone(IEnumFORMATETC** ppenum)
{
	return E_NOTIMPL;
}





/////////////////////////////////////////////////////////////////////////////
// 
// class DropSource
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// IUnknown
STDMETHODIMP DropSource::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = nullptr;


	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource))
	{
		*ppvObject = static_cast<IDropSource*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) DropSource::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}

STDMETHODIMP_(ULONG) DropSource::Release()
{
	if (InterlockedDecrement(&_RefCount) == 0)
	{
		cpp_delete this;
		return 0;
	}

	return _RefCount;
}

//===========================================================================
// IDropSource
STDMETHODIMP DropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
	if (fEscapePressed)
	{
		return DRAGDROP_S_CANCEL;
	}

	if ((grfKeyState & MK_LBUTTON) == 0)
	{
		return DRAGDROP_S_DROP;
	}

	return S_OK;
}

STDMETHODIMP DropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


