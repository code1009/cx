#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DropTarget : public IDropTarget
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
	explicit DropTarget(UINT clipboard_format);
	virtual ~DropTarget();

public:
	DropTarget(const DropTarget& other) = delete;
	DropTarget& operator=(const DropTarget& other) = delete;
	DropTarget(DropTarget&& other) noexcept = delete;
	DropTarget& operator=(DropTarget&& other) noexcept = delete;

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
