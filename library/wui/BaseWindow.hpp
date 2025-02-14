#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class BaseWindow : public Window
{
	//-----------------------------------------------------------------------
	// BaseWindow
public:
	virtual HWND createWindow(
		LPCWSTR   lpClassName,
		HWND      hWndParent   = nullptr,
		LPCWSTR   lpWindowName = L"Window",
		DWORD     dwStyle      = FrameWindowStyle,
		DWORD     dwExStyle    = FrameWindowStyleEx,
		int       X            = CW_USEDEFAULT,
		int       Y            = CW_USEDEFAULT,
		int       nWidth       = CW_USEDEFAULT,
		int       nHeight      = CW_USEDEFAULT,
		HMENU     hMenu        = nullptr,
		HINSTANCE hInstance    = nullptr
	);

	virtual HWND createWindow(
		LPCWSTR     lpClassName,
		const RECT& rect,
		HWND        hWndParent   = nullptr,
		LPCWSTR     lpWindowName = L"Window",
		DWORD       dwStyle      = FrameWindowStyle,
		DWORD       dwExStyle    = FrameWindowStyleEx,
		HMENU       hMenu        = nullptr,
		HINSTANCE   hInstance    = nullptr
	);

	virtual void destroyWindow(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class SubclassBaseWindow : public Window
{
private:
	WNDPROC _ChainWindowProc{ nullptr };

	//-----------------------------------------------------------------------
	// Window
public:
	virtual LRESULT defaultWindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override;

	//-----------------------------------------------------------------------
	// SubclassBaseWindow
public:
	virtual WNDPROC subclassWindow(HWND hwnd);
	virtual WNDPROC unsubclassWindow(WNDPROC windowProc = nullptr);

public:
	virtual WNDPROC getChainWindowProc(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class BaseDialog : public Window
{
	//-----------------------------------------------------------------------
	// Window
public:
	virtual LRESULT defaultWindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ModalTemplateBaseDialog : public BaseDialog
{
private:
	LPCWSTR _DialogTemplateName{ nullptr };

	//-----------------------------------------------------------------------
	// ModalTemplateBaseDialog
public:
	virtual void setTemplateName(LPCWSTR templateName);
	virtual void setTemplateName(std::int32_t templateNameId);

public:
	virtual INT_PTR doModal(
		HWND hwndParent,
		HINSTANCE hinstance = nullptr
	);

	virtual void endDialog(INT_PTR result);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ModelessTemplateBaseDialog : public BaseDialog
{
private:
	LPCWSTR _DialogTemplateName{ nullptr };

	//-----------------------------------------------------------------------
	// ModelessTemplateBaseDialog
public:
	virtual void setTemplateName(LPCWSTR templateName);
	virtual void setTemplateName(std::int32_t templateNameId);

public:
	virtual HWND createDialog(HWND hwndParent, HINSTANCE hInstance = nullptr);
	virtual void destroyDialog(void);
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ModalIndirectBaseDialog : public BaseDialog
{
private:
	LPCDLGTEMPLATE _DialogTemplate{ nullptr };

	//-----------------------------------------------------------------------
	// ModalIndirectBaseDialog
public:
	virtual void setTemplate(LPCDLGTEMPLATE templatePtr);

public:
	virtual INT_PTR doModal(
		HWND hwndParent,
		HINSTANCE hinstance = nullptr
	);

	virtual void endDialog(INT_PTR result);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ModelessIndirectBaseDialog : public BaseDialog
{
private:
	LPCDLGTEMPLATE _DialogTemplate{ nullptr };

	//-----------------------------------------------------------------------
	// ModelessIndirectBaseDialog
public:
	virtual void setTemplate(LPCDLGTEMPLATE templatePtr);

public:
	virtual HWND createDialog(HWND hwndParent, HINSTANCE hInstance = nullptr);
	virtual void destroyDialog(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




