/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
HWND BaseWindow::createWindow(
	LPCWSTR   lpClassName,
	HWND      hWndParent,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	DWORD     dwExStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HMENU     hMenu,
	HINSTANCE hInstance
)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getInstanceHandle();
	}


	//-----------------------------------------------------------------------
	HWND handle;


	handle = ::CreateWindowExW(
		dwExStyle,
		lpClassName,
		lpWindowName,
		dwStyle,
		X,
		Y,
		nWidth,
		nHeight,
		hWndParent,
		hMenu,
		hInstance,
		this
	);

	return handle;
}

HWND BaseWindow::createWindow(
	LPCWSTR     lpClassName,
	const RECT& rect,
	HWND        hWndParent,
	LPCWSTR     lpWindowName,
	DWORD       dwStyle,
	DWORD       dwExStyle,
	HMENU       hMenu,
	HINSTANCE   hInstance
)
{
	return createWindow(
		lpClassName,
		hWndParent,
		lpWindowName,
		dwStyle,
		dwExStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		hMenu,
		hInstance
	);
}

void BaseWindow::destroyWindow(void)
{
	HWND handle;
	BOOL rv;


	handle = getWindowHandle();
	if (handle)
	{
		rv = ::DestroyWindow(handle);
		if (FALSE == rv)
		{
			throw std::runtime_error("BasicWindow::destroyWindow(): DestroyWindow() failed");
		}

		setWindowHandle(nullptr);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LRESULT SubclassBaseWindow::defaultWindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	LRESULT lResult;


	if (_ChainWindowProc)
	{
		lResult = ::CallWindowProcW(
			_ChainWindowProc,
			hwnd,
			umsg,
			wparam,
			lparam
		);
	}
	else
	{
		lResult = 0;
		throw std::runtime_error("SubclassBaseWindow::defaultWindowProc(): _ChainWindowProc is null");
	}

	return lResult;
}

//===========================================================================
WNDPROC SubclassBaseWindow::subclassWindow(HWND hwnd)
{
	//-----------------------------------------------------------------------
#ifdef _DEBUG
	if (reinterpret_cast<void*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA)))
	{
		throw std::runtime_error("SubclassBaseWindow::subclassWindow(): GWLP_USERDATA is not null");
	}
#endif


	//-----------------------------------------------------------------------
	setWindowHandle(hwnd);


	//-----------------------------------------------------------------------
	LONG_PTR rv;
	WNDPROC oldWindowProc;


	rv = ::SetWindowLongPtrW(getWindowHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	rv = ::SetWindowLongPtrW(getWindowHandle(), GWLP_WNDPROC , reinterpret_cast<LONG_PTR>(WindowProc));
	oldWindowProc = reinterpret_cast<WNDPROC>(rv);


	//-----------------------------------------------------------------------
	_ChainWindowProc = oldWindowProc;


	return oldWindowProc;
}

WNDPROC SubclassBaseWindow::unsubclassWindow(WNDPROC windowProc)
{
	//-----------------------------------------------------------------------
	if (nullptr == windowProc)
	{
		windowProc = _ChainWindowProc;
	}


	//-----------------------------------------------------------------------
	LONG_PTR rv;
	WNDPROC oldWindowProc;


	rv = ::SetWindowLongPtrW(getWindowHandle(), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
	rv = ::SetWindowLongPtrW(getWindowHandle(), GWLP_WNDPROC , reinterpret_cast<LONG_PTR>(windowProc));
	oldWindowProc = reinterpret_cast<WNDPROC>(rv);


	//-----------------------------------------------------------------------
	setWindowHandle(nullptr);
	_ChainWindowProc = nullptr;


	return oldWindowProc;
}

WNDPROC SubclassBaseWindow::getChainWindowProc(void)
{
	return _ChainWindowProc;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LRESULT BaseDialog::defaultWindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ModalTemplateBaseDialog::setTemplateName(LPCWSTR templateName)
{
	_DialogTemplateName = templateName;
}

void ModalTemplateBaseDialog::setTemplateName(std::int32_t templateNameId)
{
	setTemplateName(MAKEINTRESOURCEW(templateNameId));
}

//===========================================================================
INT_PTR ModalTemplateBaseDialog::doModal(
	HWND hwndParent,
	HINSTANCE hInstance
)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getInstanceHandle();
	}


	//-----------------------------------------------------------------------
	std::int64_t rv;


	rv = ::DialogBoxParamW(
		hInstance,
		_DialogTemplateName,
		hwndParent,
		DialogProc,
		reinterpret_cast<LPARAM>(this)
	);

	return rv;
}

void ModalTemplateBaseDialog::endDialog(INT_PTR result)
{
	BOOL rv;


	rv = ::EndDialog(getWindowHandle(), result);
	if (FALSE == rv)
	{
		throw std::runtime_error("ModalTemplateBaseDialog::endDialog(): EndDialog() failed");
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ModelessTemplateBaseDialog::setTemplateName(LPCWSTR templateName)
{
	_DialogTemplateName = templateName;
}

void ModelessTemplateBaseDialog::setTemplateName(std::int32_t templateNameId)
{
	setTemplateName(MAKEINTRESOURCEW(templateNameId));
}

//===========================================================================
HWND ModelessTemplateBaseDialog::createDialog(HWND hwndParent, HINSTANCE hInstance)
{
	HWND hwnd;


	hwnd = ::CreateDialogParamW(
		hInstance,
		_DialogTemplateName,
		hwndParent,
		DialogProc,
		reinterpret_cast<LPARAM>(this)
	);

	return hwnd;
}

void ModelessTemplateBaseDialog::destroyDialog(void)
{
	HWND handle;
	BOOL rv;


	handle = getWindowHandle();
	if (handle)
	{
		rv = ::DestroyWindow(handle);
		if (FALSE == rv)
		{
			throw std::runtime_error("ModelessTemplateBaseDialog::destroyDialog(): DestroyWindow() failed");
		}

		setWindowHandle(nullptr);
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ModalIndirectBaseDialog::setTemplate(LPCDLGTEMPLATE templatePtr)
{
	_DialogTemplate = templatePtr;
}

//===========================================================================
INT_PTR ModalIndirectBaseDialog::doModal(
	HWND hwndParent,
	HINSTANCE hInstance
)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getInstanceHandle();
	}


	//-----------------------------------------------------------------------
	std::int64_t rv;


	rv = ::DialogBoxIndirectParamW(
		hInstance,
		_DialogTemplate,
		hwndParent,
		DialogProc,
		reinterpret_cast<LPARAM>(this)
	);

	return rv;
}

void ModalIndirectBaseDialog::endDialog(INT_PTR result)
{
	BOOL rv;


	rv = ::EndDialog(getWindowHandle(), result);
	if (FALSE == rv)
	{
		throw std::runtime_error("ModalIndirectBaseDialog::endDialog(): EndDialog() failed");
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ModelessIndirectBaseDialog::setTemplate(LPCDLGTEMPLATE templatePtr)
{
	_DialogTemplate = templatePtr;
}

//===========================================================================
HWND ModelessIndirectBaseDialog::createDialog(HWND hwndParent, HINSTANCE hInstance)
{
	HWND hwnd;


	hwnd = ::CreateDialogIndirectParamW(
		hInstance,
		_DialogTemplate,
		hwndParent,
		DialogProc,
		reinterpret_cast<LPARAM>(this)
	);

	return hwnd;
}

void ModelessIndirectBaseDialog::destroyDialog(void)
{
	HWND handle;
	BOOL rv;


	handle = getWindowHandle();
	if (handle)
	{
		rv = ::DestroyWindow(handle);
		if (FALSE == rv)
		{
			throw std::runtime_error("ModelessIndirectBaseDialog::destroyDialog(): DestroyWindow() failed");
		}

		setWindowHandle(nullptr);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




