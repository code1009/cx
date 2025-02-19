#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDropSource : public IDropSource
{
private:
	// IUnknown
	LONG _RefCount{ 1 };

public:
	WindowDropSource() = default;
	virtual ~WindowDropSource() = default;

public:
	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IDropSource
	STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
	STDMETHODIMP GiveFeedback(DWORD dwEffect);
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDataObject : public IDataObject, public IEnumFORMATETC
{
private:
	// IUnknown
	LONG _RefCount{ 1 };

private:
	// IDataObject
	HGLOBAL _hGlobal{ nullptr };

private:
	// IEnumFORMATETC
	ULONG _EnumCount{ 0 };

private:
	UINT _ClipboardFormat;

public:
	explicit WindowDataObject(UINT clipboard_format);
	virtual ~WindowDataObject();

public:
	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IDataObject
	STDMETHODIMP GetData(FORMATETC* pformatetcIn, STGMEDIUM* pmedium);
	STDMETHODIMP GetDataHere(FORMATETC* pformatetc, STGMEDIUM* pmedium);
	STDMETHODIMP QueryGetData(FORMATETC* pformatetc);
	STDMETHODIMP GetCanonicalFormatEtc(FORMATETC* pformatectIn, FORMATETC* pformatetcOut);
	STDMETHODIMP SetData(FORMATETC* pformatetc, STGMEDIUM* pmedium, BOOL fRelease);
	STDMETHODIMP EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppenumFormatEtc);
	STDMETHODIMP DAdvise(FORMATETC* pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection);
	STDMETHODIMP DUnadvise(DWORD dwConnection);
	STDMETHODIMP EnumDAdvise(IEnumSTATDATA** ppenumAdvise);

	// IEnumFORMATETC
	STDMETHODIMP Next(ULONG celt, FORMATETC* rgelt, ULONG* pceltFetched);
	STDMETHODIMP Skip(ULONG celt);
	STDMETHODIMP Reset(VOID);
	STDMETHODIMP Clone(IEnumFORMATETC** ppenum);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDropTarget : public IDropTarget
{
private:
	// IUnknown
	LONG    _RefCount{ 1 };

private:
	// IDropTarget
	bool    _SupportFormat;
	HGLOBAL _hGlobal;

private:
	UINT _ClipboardFormat;

public:
	std::function<void(DWORD grfKeyState, POINTL pt, HGLOBAL hGlobal)> onDragEnter;
	std::function<void(DWORD grfKeyState, POINTL pt)                 > onDragOver ;
	std::function<void(void)                                         > onDragLeave;
	std::function<void(DWORD grfKeyState, POINTL pt)                 > onDrop;

public:
	explicit WindowDropTarget(UINT clipboard_format);
	virtual ~WindowDropTarget();

private:
	HGLOBAL getDataObjectHGlobal(IDataObject* pDataObj, FORMATETC* formatetc);

public:
	// IUnknown
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IDropTarget
	STDMETHODIMP DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	STDMETHODIMP DragLeave();
	STDMETHODIMP Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
