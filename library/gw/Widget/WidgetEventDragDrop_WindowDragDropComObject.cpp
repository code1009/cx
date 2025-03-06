/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../runtime/runtime.hpp"

//===========================================================================
#include "WidgetEventDragDrop_WindowDragDropComObject.hpp"
#include "WidgetEventDragDrop_HGlobal.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{






/////////////////////////////////////////////////////////////////////////////
// 
// class WindowDropSource
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// IUnknown
STDMETHODIMP WindowDropSource::QueryInterface(REFIID riid, void** ppvObject)
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

STDMETHODIMP_(ULONG) WindowDropSource::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}

STDMETHODIMP_(ULONG) WindowDropSource::Release()
{
	if (InterlockedDecrement(&_RefCount) == 0)
	{
		delete this;
		return 0;
	}

	return _RefCount;
}

//===========================================================================
// IDropSource
STDMETHODIMP WindowDropSource::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
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

STDMETHODIMP WindowDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}





/////////////////////////////////////////////////////////////////////////////
// 
// class WindowDataObject
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDataObject::WindowDataObject(UINT clipboard_format)
{
	_RefCount = 1;

	_hGlobal = nullptr;

	_EnumCount = 0;

	_ClipboardFormat = clipboard_format;
}

WindowDataObject::~WindowDataObject()
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
STDMETHODIMP WindowDataObject::QueryInterface(REFIID riid, void** ppvObject)
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

STDMETHODIMP_(ULONG) WindowDataObject::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}

STDMETHODIMP_(ULONG) WindowDataObject::Release()
{
	if (InterlockedDecrement(&_RefCount) == 0)
	{
		delete this;
		return 0;
	}

	return _RefCount;
}

//===========================================================================
// IDataObject
STDMETHODIMP WindowDataObject::GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium)
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

STDMETHODIMP WindowDataObject::GetDataHere(FORMATETC* pformatetc, STGMEDIUM* pmedium)
{
	return E_NOTIMPL;
}

STDMETHODIMP WindowDataObject::QueryGetData(FORMATETC* pformatetc)
{
	if (pformatetc->cfFormat == _ClipboardFormat)
	{
		return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP WindowDataObject::GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut)
{
	return E_NOTIMPL;
}

STDMETHODIMP WindowDataObject::SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease)
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

STDMETHODIMP WindowDataObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc)
{
	if (dwDirection == DATADIR_GET)
	{
		return QueryInterface(IID_PPV_ARGS(ppenumFormatEtc));
	}

	return E_NOTIMPL;
}

STDMETHODIMP WindowDataObject::DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP WindowDataObject::DUnadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

STDMETHODIMP WindowDataObject::EnumDAdvise(IEnumSTATDATA** ppenumAdvise)
{
	return E_NOTIMPL;
}

//===========================================================================
// IEnumFORMATETC
STDMETHODIMP WindowDataObject::Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched)
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

STDMETHODIMP WindowDataObject::Skip(ULONG celt)
{
	return E_NOTIMPL;
}

STDMETHODIMP WindowDataObject::Reset(VOID)
{
	_EnumCount = 0;

	return S_OK;
}

STDMETHODIMP WindowDataObject::Clone(IEnumFORMATETC** ppenum)
{
	return E_NOTIMPL;
}





/////////////////////////////////////////////////////////////////////////////
// 
// class WindowDropTarget
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDropTarget::WindowDropTarget(UINT clipboard_format)
{
	_RefCount = 1;

	_SupportFormat = false;
	_hGlobal = nullptr;

	_ClipboardFormat = clipboard_format;
}

WindowDropTarget::~WindowDropTarget()
{
}

//===========================================================================
HGLOBAL WindowDropTarget::getDataObjectHGlobal(IDataObject* pDataObj, FORMATETC* formatetc)
{
	//-----------------------------------------------------------------------
	HRESULT   hr;
	STGMEDIUM medium;


	hr = pDataObj->GetData(formatetc, &medium);
	if (FAILED(hr))
	{
		return nullptr;
	}


	//-----------------------------------------------------------------------
	HGLOBAL hGlobal;


	hGlobal = medium.hGlobal;


	//-----------------------------------------------------------------------
	/*
	void* source_pointer;
	std::size_t source_size;

		
	source_pointer = GlobalLock(hGlobal);
	source_size    = GlobalSize(hGlobal);

	GlobalUnlock(medium.hGlobal);
	*/

	return hGlobal;
}

//===========================================================================
// IUnknown
STDMETHODIMP WindowDropTarget::QueryInterface(REFIID riid, void** ppvObject)
{
	*ppvObject = nullptr;

	if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
	{
		*ppvObject = static_cast<IDropTarget*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) WindowDropTarget::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}

STDMETHODIMP_(ULONG) WindowDropTarget::Release()
{
	if (InterlockedDecrement(&_RefCount) == 0)
	{
		delete this;
		return 0;
	}

	return _RefCount;
}

//===========================================================================
// IDropTarget
STDMETHODIMP WindowDropTarget::DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	//-----------------------------------------------------------------------
	HRESULT   hr;
	FORMATETC formatetc;


	*pdwEffect = DROPEFFECT_COPY;

	formatetc.cfFormat = _ClipboardFormat;
	formatetc.ptd      = nullptr;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex   = -1;
	formatetc.tymed    = TYMED_HGLOBAL;

	hr = pDataObj->QueryGetData(&formatetc);
	if (hr != S_OK)
	{
		*pdwEffect = DROPEFFECT_NONE;

		_SupportFormat = false;

		_hGlobal = nullptr;

		return S_OK;
	}


	//-----------------------------------------------------------------------
	_SupportFormat = true;

	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< L" : "
		<< pt.x << L", " << pt.y
		<< std::endl;

	_hGlobal = getDataObjectHGlobal(pDataObj, &formatetc);

	if (_hGlobal == nullptr)
	{
		_SupportFormat = FALSE;

		CX_RUNTIME_LOG(cxLDebug) << L"_hGlobal is null!" << std::endl;

		*pdwEffect = DROPEFFECT_NONE;

		_hGlobal = nullptr;

		return S_OK;
	}


	//-----------------------------------------------------------------------
	if (onDragEnter)
	{
		onDragEnter(grfKeyState, pt, _hGlobal);
	}
	

	return S_OK;
}

STDMETHODIMP WindowDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	//-----------------------------------------------------------------------
	*pdwEffect = DROPEFFECT_COPY;

	if (!_SupportFormat)
	{
		*pdwEffect = DROPEFFECT_NONE;

		_hGlobal = nullptr;

		return S_OK;
	}


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< L" : "
		<< pt.x << L", " << pt.y
		<< std::endl;

	if (_hGlobal == nullptr)
	{
		CX_RUNTIME_LOG(cxLDebug) << L"_hGlobal is null!" << std::endl;
		*pdwEffect = DROPEFFECT_NONE;

		_hGlobal = nullptr;

		return S_OK;
	}


	//-----------------------------------------------------------------------
	if (onDragOver)
	{
		onDragOver(grfKeyState, pt);
	}


	return S_OK;
}

STDMETHODIMP WindowDropTarget::DragLeave()
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< std::endl;


	_hGlobal = nullptr;


	//-----------------------------------------------------------------------
	if (onDragLeave)
	{
		onDragLeave();
	}

	return S_OK;
}

STDMETHODIMP WindowDropTarget::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	//-----------------------------------------------------------------------
	HRESULT   hr;
	FORMATETC formatetc;


	*pdwEffect = DROPEFFECT_COPY;

	formatetc.cfFormat = _ClipboardFormat;
	formatetc.ptd      = nullptr;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex   = -1;
	formatetc.tymed    = TYMED_HGLOBAL;

	hr = pDataObj->QueryGetData(&formatetc);
	if (hr != S_OK)
	{
		*pdwEffect = DROPEFFECT_NONE;

		_hGlobal = nullptr;

		return E_FAIL;
	}


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< L" : "
		<< pt.x << L", " << pt.y
		<< std::endl;


	//	_hGlobal = getDataObjectHGlobal(pDataObj, &formatetc);
	if (_hGlobal == nullptr)
	{
		CX_RUNTIME_LOG(cxLDebug) << L"_hGlobal is null!" << std::endl;

		*pdwEffect = DROPEFFECT_NONE;

		_hGlobal = nullptr;

		return E_FAIL;
	}


	//-----------------------------------------------------------------------
	if (onDrop)
	{
		onDrop(grfKeyState, pt);
	}

	
	//SendMessage(_hWnd, WM_MOUSEACTIVATE, 0, 0);


	return S_OK;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


