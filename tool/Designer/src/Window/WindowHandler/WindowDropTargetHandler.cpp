/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "DropTarget.hpp"
#include "HGlobal.hpp"

//===========================================================================
#include "WindowDropTargetHandler.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDropTargetData::WindowDropTargetData(std::uint32_t clipboardFormat, std::uint32_t dataSeq) :
	_ClipboardFormat{ clipboardFormat },
	_DataSeq{ dataSeq }
{
}

WindowDropTargetData::WindowDropTargetData(std::uint32_t clipboardFormat, std::uint32_t dataSeq, std::uint8_t* pointer, std::size_t size):
	_ClipboardFormat{ clipboardFormat },
	_DataSeq{ dataSeq },
	_DataBuffer{ pointer, pointer + size }
{	
}

//===========================================================================
void WindowDropTargetData::setData(std::uint8_t* pointer, std::size_t size)
{
	_DataBuffer.clear();
	_DataBuffer.insert(_DataBuffer.begin(),
		pointer,
		pointer + size
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDropTargetHandler::WindowDropTargetHandler(HWND hwnd, std::uint32_t clipboardFormat) :
	_Hwnd{ hwnd },
	_ClipboardFormat{ clipboardFormat }
{
	DropTarget* pDropTarget;
	pDropTarget = cpp_new DropTarget(_ClipboardFormat);
	if (!pDropTarget)
	{
		throw std::bad_alloc();
	}


	pDropTarget->onDragEnter = std::bind(&WindowDropTargetHandler::onDragEnter, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	pDropTarget->onDragOver  = std::bind(&WindowDropTargetHandler::onDragOver , this, std::placeholders::_1, std::placeholders::_2);
	pDropTarget->onDragLeave = std::bind(&WindowDropTargetHandler::onDragLeave, this);
	pDropTarget->onDrop      = std::bind(&WindowDropTargetHandler::onDrop     , this, std::placeholders::_1, std::placeholders::_2);


	_pDropTarget = pDropTarget;


	HRESULT hr;
	hr = RegisterDragDrop(_Hwnd, _pDropTarget);
	if (FAILED(hr))
	{
		CX_RUNTIME_LOG(cxLError) << L"RegisterDragDrop() failed : "
			<< L"hr=" << hr
			;
		
		throw std::runtime_error("RegisterDragDrop() failed");
	}


	CX_RUNTIME_LOG(cxLInfo) << L"Registered DragDrop Event";
}

WindowDropTargetHandler::~WindowDropTargetHandler()
{
	HRESULT hr;


	hr = RevokeDragDrop(_Hwnd);
	if (FAILED(hr))
	{
		CX_RUNTIME_LOG(cxLDebug) << L"RevokeDragDrop() failed : "
			<< L"hr=" << hr
			;
	}


	if (_pDropTarget)
	{
		_pDropTarget->Release();
		_pDropTarget = nullptr;
	}


	CX_RUNTIME_LOG(cxLInfo) << L"Unregistered DragDrop Event";
}

//===========================================================================
void WindowDropTargetHandler::onDragEnter(DWORD grfKeyState, POINTL pt, HGLOBAL hGlobal)
{
	//-----------------------------------------------------------------------
	_DataSeq++;


	//-----------------------------------------------------------------------
	WindowDropTargetData data(_ClipboardFormat, _DataSeq);


	//-----------------------------------------------------------------------
	if (hGlobal)
	{
		std::uint8_t* pointer;
		std::size_t size;

		pointer = (std::uint8_t*)GlobalLock(hGlobal);
		size = GlobalSize(hGlobal);
		if ((nullptr != pointer) && (size > 0))
		{
			data.setData(pointer, size);
		}
		else
		{
			CX_RUNTIME_LOG(cxLDebug) << L"GlobalLock() failed or size is zero";
		}
		GlobalUnlock(hGlobal);
	}
	else
	{
		CX_RUNTIME_LOG(cxLDebug) << L"hGlobal is nullptr!";
	}


	//-----------------------------------------------------------------------
	POINT windowClientPoint;
	windowClientPoint.x = pt.x;
	windowClientPoint.y = pt.y;
	ScreenToClient(_Hwnd, &windowClientPoint);


	//-----------------------------------------------------------------------
	if (dragEnterHandler)
	{
		dragEnterHandler(_DataSeq, grfKeyState, windowClientPoint.x, windowClientPoint.y, data);
	}
}

void WindowDropTargetHandler::onDragOver(DWORD grfKeyState, POINTL pt)
{
	//-----------------------------------------------------------------------
	POINT windowClientPoint;
	windowClientPoint.x = pt.x;
	windowClientPoint.y = pt.y;
	ScreenToClient(_Hwnd, &windowClientPoint);


	//-----------------------------------------------------------------------
	if (dragOverHandler)
	{
		dragOverHandler(_DataSeq, grfKeyState, windowClientPoint.x, windowClientPoint.y);
	}
}

void WindowDropTargetHandler::onDragLeave(void)
{
	//-----------------------------------------------------------------------
	if (dragLeaveHandler)
	{
		dragLeaveHandler(_DataSeq);
	}
}

void WindowDropTargetHandler::onDrop(DWORD grfKeyState, POINTL pt)
{
	//-----------------------------------------------------------------------
	POINT windowClientPoint;
	windowClientPoint.x = pt.x;
	windowClientPoint.y = pt.y;
	ScreenToClient(_Hwnd, &windowClientPoint);


	//-----------------------------------------------------------------------
	if (dropHandler)
	{
		dropHandler(_DataSeq, grfKeyState, windowClientPoint.x, windowClientPoint.y);
	}


	//-----------------------------------------------------------------------
	SendMessage(_Hwnd, WM_MOUSEACTIVATE, 0, 0);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




