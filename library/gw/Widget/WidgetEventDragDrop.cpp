/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../gw.hpp"

#include "../../runtime/runtime.hpp"

//===========================================================================
#include "WidgetEventDragDrop_ClipboardFormat.hpp"
#include "WidgetEventDragDrop_WindowDragDropComObject.hpp"
#include "WidgetEventDragDrop_HGlobal.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static ClipboardFormat* getWidgetEventDragDropClipboardFormat(void)
{
	static ClipboardFormat instance;


	return &instance;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool initializeWidgetEventDragDrop(void)
{
	constexpr wchar_t clipboard_format_name[] =
	{
		L"gw.widget={4EF1BA30-54D7-4F6F-A169-0B10005E9F08}"
	};


	bool rv;
	rv = getWidgetEventDragDropClipboardFormat()->
		registerClipboardFormat(clipboard_format_name);
	if (false == rv)
	{
		return false;
	}


	return true;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetEventDragDropNotifier::Impl
{
public:
	IDropSource* _pDropSource { nullptr };
	IDataObject* _pDataObject { nullptr };

public:
	Impl();

public:
	virtual ~Impl();

public:
	Impl(const Impl&) = delete;
	Impl& operator=(const Impl&) = delete;

	Impl(Impl&&) = delete;
	Impl& operator=(Impl&&) = delete;

public:
	bool setData(
		IDataObject* pDataObject, 
		const void* source_pointer,
		std::size_t source_size, 
		std::uint32_t cf
	);
	bool doDragDrop(DWORD& dwEffect, bool& drop);
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetEventDragDropNotifier::Impl::Impl()
{
	_pDropSource = new WindowDropSource();
	_pDataObject = new WindowDataObject(getWidgetEventDragDropClipboardFormat()->getClipboardFormat());
}

//===========================================================================
WidgetEventDragDropNotifier::Impl::~Impl()
{
	if(_pDropSource)
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
bool WidgetEventDragDropNotifier::Impl::setData(
	IDataObject* pDataObject,
	const void* source_pointer,
	std::size_t source_size,
	std::uint32_t cf
)
{
	//-----------------------------------------------------------------------
	HGLOBAL hGlobal;
	hGlobal = createHGlobal(source_pointer, source_size);
	if (nullptr == hGlobal)
	{
		CX_RUNTIME_LOG(LDbug) << L"createHGlobal() failed : "
			<< L"source_pointer=" << source_pointer
			<< L", "
			<< L"source_size=" << source_size
			<< std::endl;
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
		CX_RUNTIME_LOG(LDbug) << L"SetData() OK" << std::endl;
		return true;
	}
	CX_RUNTIME_LOG(LDbug) << L"SetData() failed" << std::endl;


	GlobalFree(hGlobal);
	CX_RUNTIME_LOG(LDbug) << L"GlobalFree()" << std::endl;

	return false;
}

bool WidgetEventDragDropNotifier::Impl::doDragDrop(DWORD& dwEffect, bool& drop)
{
	HRESULT hr;


	hr = DoDragDrop(_pDataObject, _pDropSource, DROPEFFECT_COPY, &dwEffect);
	if (FAILED(hr))
	{
		CX_RUNTIME_LOG(LDbug) << L"DoDragDrop() failed : "
			<< L"hr=" << hr
			<< std::endl;
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
		CX_RUNTIME_LOG(LDbug) << L"DoDragDrop() failed : "
			<< L"hr=" << hr
			<< std::endl;
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetEventDragDropNotifier::WidgetEventDragDropNotifier()
{
}

WidgetEventDragDropNotifier::~WidgetEventDragDropNotifier()
{
}

//===========================================================================
void WidgetEventDragDropNotifier::setDragDropData(const WidgetEventDragDropData& data)
{
	_DragDropData = data;
}

bool WidgetEventDragDropNotifier::doDragDrop(void)
{
	//-----------------------------------------------------------------------
	_Impl = std::make_unique<WidgetEventDragDropNotifier::Impl>();


	//-----------------------------------------------------------------------
	_Impl->setData(
		_Impl->_pDataObject,
		reinterpret_cast<const void*>(_DragDropData._String.c_str()),
		(_DragDropData._String.size() + 1) * sizeof(wchar_t),
		getWidgetEventDragDropClipboardFormat()->getClipboardFormat()
	);


	//-----------------------------------------------------------------------
	bool  rv;
	DWORD dwEffect;
	bool drop;


	rv = _Impl->doDragDrop(dwEffect, drop);
	if (false == rv)
	{
		_Impl.reset();
		return false;
	}


	switch (dwEffect)
	{
	case DROPEFFECT_COPY:
		CX_RUNTIME_LOG(LDbug) << L"DROPEFFECT_COPY : "
			<< L"dwEffect=" << dwEffect
			<< std::endl;
		break;

	case DROPEFFECT_MOVE:
		CX_RUNTIME_LOG(LDbug) << L"DROPEFFECT_MOVE : "
			<< L"dwEffect=" << dwEffect
			<< std::endl;
		break;

	case DROPEFFECT_NONE:
		CX_RUNTIME_LOG(LDbug) << L"DROPEFFECT_NONE : "
			<< L"dwEffect=" << dwEffect
			<< std::endl;
		break;

	default:
		CX_RUNTIME_LOG(LDbug) << L"? : "
			<< L"dwEffect=" << dwEffect
			<< std::endl;
		break;
	}


	_Impl.reset();


	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetEventDragDropHandler::Impl
{
public:
	IDropTarget* _pDropTarget{ nullptr };

public:
	std::uint32_t _DataSeq{ 0 };

public:
	WidgetDocument* _WidgetDocument{ nullptr };
	Viewport* _Viewport{ nullptr };

public:
	explicit Impl(WidgetDocument* doc, Viewport* viewport);

public:
	virtual ~Impl();

public:
	Impl(const Impl&) = default;
	Impl& operator=(const Impl&) = default;

	Impl(Impl&&) = default;
	Impl& operator=(Impl&&) = default;

public:
	void toDocumentPoint(POINTL pt, coord_t& x, coord_t& y);

public:
	void onDragEnter(DWORD grfKeyState, POINTL pt, HGLOBAL hGlobal);
	void onDragOver(DWORD grfKeyState, POINTL pt);
	void onDragLeave(void);
	void onDrop(DWORD grfKeyState, POINTL pt);
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetEventDragDropHandler::Impl::Impl(WidgetDocument* doc, Viewport* viewport) :
	_WidgetDocument{ doc },
	_Viewport{ viewport }
{
	WindowDropTarget* pDropTarget;
	pDropTarget = new WindowDropTarget(
		getWidgetEventDragDropClipboardFormat()->getClipboardFormat()
	);
	pDropTarget->onDragEnter = std::bind(&WidgetEventDragDropHandler::Impl::onDragEnter, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	pDropTarget->onDragOver  = std::bind(&WidgetEventDragDropHandler::Impl::onDragOver , this, std::placeholders::_1, std::placeholders::_2);
	pDropTarget->onDragLeave = std::bind(&WidgetEventDragDropHandler::Impl::onDragLeave, this);
	pDropTarget->onDrop      = std::bind(&WidgetEventDragDropHandler::Impl::onDrop     , this, std::placeholders::_1, std::placeholders::_2);


	_pDropTarget = pDropTarget;


	HRESULT hr;
	hr = RegisterDragDrop(_Viewport->getWindowHandle(), _pDropTarget);
	if (FAILED(hr))
	{
		CX_RUNTIME_LOG(LDbug) << L"RegisterDragDrop() failed : "
			<< L"hr=" << hr
			<< std::endl;
	}
}

WidgetEventDragDropHandler::Impl::~Impl()
{
	HRESULT hr;


	hr = RevokeDragDrop(_Viewport->getWindowHandle());
	if (FAILED(hr))
	{
		CX_RUNTIME_LOG(LDbug) << L"RevokeDragDrop() failed : "
			<< L"hr=" << hr
			<< std::endl;
	}


	if (_pDropTarget)
	{
		_pDropTarget->Release();
		_pDropTarget = nullptr;
	}
}

//===========================================================================
void WidgetEventDragDropHandler::Impl::toDocumentPoint(POINTL pt, coord_t& x, coord_t& y)
{
	//-----------------------------------------------------------------------
	POINT client_pt;
	client_pt.x = pt.x;
	client_pt.y = pt.y;
	ScreenToClient(_Viewport->getWindowHandle(), &client_pt);


	//-----------------------------------------------------------------------
	double _document_mouse_x;
	double _document_mouse_y;
	std::int64_t _mouse_x;
	std::int64_t _mouse_y;
	_mouse_x = client_pt.x;
	_mouse_y = client_pt.y;
	_Viewport->WindowToDocument(_mouse_x, _mouse_y, _document_mouse_x, _document_mouse_y);
	x = static_cast<coord_t>(_document_mouse_x);
	y = static_cast<coord_t>(_document_mouse_y);
}

//===========================================================================
void WidgetEventDragDropHandler::Impl::onDragEnter(DWORD grfKeyState, POINTL pt, HGLOBAL hGlobal)
{
	//-----------------------------------------------------------------------
	_DataSeq++;


	//-----------------------------------------------------------------------
	std::wstring s;

	void* source_pointer;
	std::size_t source_size;


	source_pointer = GlobalLock(hGlobal);
	source_size = GlobalSize(hGlobal);

	if ((nullptr != source_pointer) && (source_size > 0))
	{
		wchar_t* pointer;
		std::size_t length;


		pointer = static_cast<wchar_t*>(source_pointer);
		length = source_size / sizeof(wchar_t);
		if (length > 1)
		{
			length--;

			s.insert(s.begin(),
				pointer,
				pointer + length
			);
		}
	}

	GlobalUnlock(hGlobal);


	//-----------------------------------------------------------------------
	WidgetMouseDragEnterEventParam param;


	param._DataSeq = _DataSeq;
	toDocumentPoint(
		pt,
		param._MousePosition._x,
		param._MousePosition._y
	);
	param._String = s;


	//-----------------------------------------------------------------------
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseDragEnterEventParam&>(
		WidgetEventType::MouseDragEnter,
		nullptr,
		param
	);
}

void WidgetEventDragDropHandler::Impl::onDragOver(DWORD grfKeyState, POINTL pt)
{
	//-----------------------------------------------------------------------
	WidgetMouseDragOverEventParam param;


	param._DataSeq = _DataSeq;
	toDocumentPoint(
		pt,
		param._MousePosition._x,
		param._MousePosition._y
	);


	//-----------------------------------------------------------------------
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseDragOverEventParam&>(
		WidgetEventType::MouseDragOver,
		nullptr,
		param
	);
}

void WidgetEventDragDropHandler::Impl::onDragLeave(void)
{
	//-----------------------------------------------------------------------
	WidgetMouseDragLeaveEventParam param;


	param._DataSeq = _DataSeq;


	//-----------------------------------------------------------------------
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseDragLeaveEventParam&>(
		WidgetEventType::MouseDragLeave,
		nullptr,
		param
	);
}

void WidgetEventDragDropHandler::Impl::onDrop(DWORD grfKeyState, POINTL pt)
{
	//-----------------------------------------------------------------------
	WidgetMouseDropEventParam param;


	param._DataSeq = _DataSeq;
	toDocumentPoint(
		pt,
		param._MousePosition._x,
		param._MousePosition._y
	);


	//-----------------------------------------------------------------------
	_WidgetDocument->getWidgetEventDispatcher()->dispatch<const WidgetMouseDropEventParam&>(
		WidgetEventType::MouseDrop,
		nullptr,
		param
	);


	//-----------------------------------------------------------------------
	SendMessage(_Viewport->getWindowHandle(), WM_MOUSEACTIVATE, 0, 0);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetEventDragDropHandler::WidgetEventDragDropHandler(WidgetDocument* doc, Viewport* viewport) :
	_WidgetDocument{ doc },
	_Viewport{ viewport }
{
}

WidgetEventDragDropHandler::~WidgetEventDragDropHandler()
{
	unregisterDragDrop();
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void WidgetEventDragDropHandler::registerDragDrop(void)
{
	_Impl = std::make_unique<WidgetEventDragDropHandler::Impl>(_WidgetDocument, _Viewport);
}

void WidgetEventDragDropHandler::unregisterDragDrop(void)
{
	if (_Impl)
	{
		_Impl.reset();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




