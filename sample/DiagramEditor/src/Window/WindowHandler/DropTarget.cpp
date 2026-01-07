/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "DropTarget.hpp"
#include "HGlobal.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
// 
// class DropTarget
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DropTarget::DropTarget(UINT clipboard_format)
{
	_RefCount = 1;

	_SupportFormat = false;
	_hGlobal = nullptr;

	_ClipboardFormat = clipboard_format;
}

DropTarget::~DropTarget()
{
}

//===========================================================================
HGLOBAL DropTarget::getDataObjectHGlobal(IDataObject* pDataObj, FORMATETC* formatetc)
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
	std::uint8_t* pointer;
	std::size_t size;

		
	pointer = GlobalLock(hGlobal);
	size    = GlobalSize(hGlobal);

	GlobalUnlock(hGlobal);
	*/

	return hGlobal;
}

//===========================================================================
// IUnknown
STDMETHODIMP DropTarget::QueryInterface(REFIID riid, void** ppvObject)
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

STDMETHODIMP_(ULONG) DropTarget::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}

STDMETHODIMP_(ULONG) DropTarget::Release()
{
	if (InterlockedDecrement(&_RefCount) == 0)
	{
		cpp_delete this;
		return 0;
	}

	return _RefCount;
}

//===========================================================================
// IDropTarget
STDMETHODIMP DropTarget::DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
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


	//-----------------------------------------------------------------------
#if 0
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< L" : "
		<< pt.x << L", " << pt.y
		<< std::endl;
#endif


	//-----------------------------------------------------------------------
	_hGlobal = getDataObjectHGlobal(pDataObj, &formatetc);
	if (_hGlobal == nullptr)
	{
		CX_RUNTIME_LOG(cxLDebug) << L"_hGlobal is nullptr!" << std::endl;

		*pdwEffect = DROPEFFECT_NONE;

		_SupportFormat = FALSE;
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

STDMETHODIMP DropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
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
#if 0
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< L" : "
		<< pt.x << L", " << pt.y
		<< std::endl;
#endif


	//-----------------------------------------------------------------------
	if (_hGlobal == nullptr)
	{
		CX_RUNTIME_LOG(cxLDebug) << L"_hGlobal is nullptr!" << std::endl;

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

STDMETHODIMP DropTarget::DragLeave()
{
	//-----------------------------------------------------------------------
#if 0
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< std::endl;
#endif


	//-----------------------------------------------------------------------
	_hGlobal = nullptr;


	//-----------------------------------------------------------------------
	if (onDragLeave)
	{
		onDragLeave();
	}

	return S_OK;
}

STDMETHODIMP DropTarget::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
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
#if 0
	CX_RUNTIME_LOG(cxLDebug)
		<< (_SupportFormat ? L"DROPEFFECT_COPY" : L"DROPEFFECT_NONE")
		<< L" : "
		<< pt.x << L", " << pt.y
		<< std::endl;
#endif


	//-----------------------------------------------------------------------
	//	_hGlobal = getDataObjectHGlobal(pDataObj, &formatetc);
	if (_hGlobal == nullptr)
	{
		CX_RUNTIME_LOG(cxLDebug) << L"_hGlobal is nullptr!" << std::endl;

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


