#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DataObject : public IDataObject, public IEnumFORMATETC
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
	explicit DataObject(UINT clipboard_format);
	virtual ~DataObject();

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
class DropSource : public IDropSource
{
private:
	// IUnknown
	LONG _RefCount{ 1 };

public:
	DropSource() = default;
	virtual ~DropSource() = default;

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
}
