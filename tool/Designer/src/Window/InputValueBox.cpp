/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "InputValueBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
InputValueBox::InputValueBox()
{
}

//===========================================================================
LRESULT InputValueBox::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLTrace) <<
		L"InputValueBox::onWindowMessage(): uMsg=" << windowMessage.uMsg
		;
#endif
	return cx::wui::MessageMapWindowT<InputValueBox, cx::wui::ModalIndirectBaseDialog>::onWindowMessage(windowMessage);
}

//===========================================================================
void InputValueBox::setupInputBox(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy)
{
	//-----------------------------------------------------------------------
	setPosition(x, y, cx, cy);


	//-----------------------------------------------------------------------
	initializeDialogTemplate();

	registerWindowMessageMap();
}

void InputValueBox::setPosition(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy)
{
	_X = x;
	_Y = y;
	_CX = cx;
	_CY = cy;
}

void InputValueBox::setWindowPosition(void)
{
	MoveWindow(*this, _X, _Y, _CX, _CY, FALSE);
}

//===========================================================================
void InputValueBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);


	w.BEGIN_DIALOG(0, 0, _CX, _CY);
		w.DIALOG_STYLE  (DS_SETFONT | WS_POPUP | WS_BORDER);
		w.DIALOG_FONT   (9, L"Segoe UI");
	w.END_DIALOG();
	w.BEGIN_CONTROLS_MAP();
	w.END_CONTROLS_MAP();


	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void InputValueBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &InputValueBox::onInitDialog;
	_WindowMessageMap[WM_COMMAND   ] = &InputValueBox::onCommand;
	_WindowMessageMap[WM_NCACTIVATE] = &InputValueBox::onNcActivate;
	_WindowMessageMap[WM_ACTIVATE  ] = &InputValueBox::onActivate;
	_WindowMessageMap[WM_SETFOCUS] = &InputValueBox::onSetFocus;
	_WindowMessageMap[WM_KILLFOCUS] = &InputValueBox::onKillFocus;
	_WindowMessageMap[WM_USER+1] = &InputValueBox::onUser1;
//	_WindowMessageMap[WM_PAINT] = &InputValueBox::onPaint;
}

void InputValueBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	setWindowPosition();
	windowMessage.setResult(TRUE);
}

void InputValueBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDOK:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_COMMAND - IDOK"
			;

		onEndDialog(IDOK);
		::EndDialog(*this, IDOK);
		windowMessage.setResult(TRUE);
		break;

	case IDCANCEL:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_COMMAND - IDCANCEL"
			;

		onEndDialog(IDCANCEL);
		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
		break;

	default:
		windowMessage.setResult(FALSE);
		break;
	}
}

void InputValueBox::onNcActivate(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_NCACTIVATE_WindowMessageCrack wm{ windowMessage };

	BOOL active = wm.bActive();
	if (active)
	{
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_NCACTIVATE - active"
			;
	}
	else
	{
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_NCACTIVATE - no active"
			;

		//::EndDialog(*this, IDCANCEL);
		PostMessage(*this, WM_USER+1, 1009, 1);
	}
}

void InputValueBox::onActivate(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_ACTIVATE_WindowMessageCrack wm{ windowMessage };

	UINT state = wm.nState();
	switch (state)
	{
	case WA_ACTIVE:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_ACTIVATE - WA_ACTIVE"
			;
		break;

	case WA_CLICKACTIVE:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_ACTIVATE - WA_CLICKACTIVE"
			;
		break;

	case WA_INACTIVE:
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_ACTIVATE - WA_INACTIVE"
			;
		break;

	default:
		break;
	}
}

void InputValueBox::onSetFocus(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_SETFOCUS_WindowMessageCrack wm{ windowMessage };

	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_SETFOCUS"
		;
}

void InputValueBox::onKillFocus(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_KILLFOCUS_WindowMessageCrack wm{ windowMessage };

	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_KILLFOCUS"
		;
}

void InputValueBox::onUser1(cx::wui::WindowMessage& windowMessage)
{
	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_USER+1:"
		<< L" wParam=" << windowMessage.wParam
		<< L" lParam=" << windowMessage.lParam
		;

	if (windowMessage.wParam = 1009 && windowMessage.lParam == 1)
	{
		CX_RUNTIME_LOG(cxLTrace) <<
			L"WM_USER+1 : EndDialog"
			;
		
		onEndDialog(IDOK);
		::EndDialog(*this, IDOK);
	}
}

void InputValueBox::onPaint(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::PaintDC paintDC(*this);


	RECT rc;
	::GetClientRect(*this, &rc);


	::FillRect(paintDC, &rc, (HBRUSH)(COLOR_WINDOW + 1));


	rc.right--;
	rc.bottom--;


	HPEN lightPen = ::CreatePen(PS_SOLID, 1, RGB(240, 240, 255));
	HPEN darkPen = ::CreatePen(PS_SOLID, 1, RGB(64, 64, 128));
	
	HPEN lPen;
	HPEN rPen;
	HPEN tPen;
	HPEN bPen;

	HPEN oldPen;
	HPEN firstPen;

	lPen = lightPen;
	tPen = lightPen;
	rPen = darkPen;
	bPen = lightPen;

	
	firstPen = rPen;
	oldPen = (HPEN)::SelectObject(paintDC, rPen);
	::MoveToEx(paintDC, rc.right, rc.top, nullptr);
	::LineTo(paintDC, rc.right, rc.bottom);

	if (firstPen != bPen)
	{
		::SelectObject(paintDC, bPen);
	}
	::LineTo(paintDC, rc.left, rc.bottom);

	if (firstPen != lPen)
	{
		::SelectObject(paintDC, lPen);
	}
	::LineTo(paintDC, rc.left, rc.top);

	if (firstPen != tPen)
	{
		::SelectObject(paintDC, tPen);
	}
	::LineTo(paintDC, rc.right, rc.top);


	::SelectObject(paintDC, oldPen);


	::DeleteObject(lightPen);
	::DeleteObject(darkPen);
}

void InputValueBox::onEndDialog(INT_PTR result)
{
	CX_RUNTIME_LOG(cxLTrace) <<
		L"InputValueBox::onEndDialog"
		;
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
InputStringBox::InputStringBox()
{
}

//===========================================================================
void InputStringBox::setupInputStringBox(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy)
{
	//-----------------------------------------------------------------------
	setPosition(x, y, cx, cy);


	//-----------------------------------------------------------------------
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void InputStringBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);


	w.BEGIN_DIALOG(0, 0, _CX, _CY);
		w.DIALOG_STYLE(DS_SETFONT | WS_POPUP | WS_BORDER);
		w.DIALOG_FONT(9, L"Segoe UI");
	w.END_DIALOG();
	w.BEGIN_CONTROLS_MAP();
	w.END_CONTROLS_MAP();


	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void InputStringBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	setWindowPosition();


	//-----------------------------------------------------------------------
	_Font = std::make_unique<cx::wui::Font>(L"맑은 고딕", 100);


	//-----------------------------------------------------------------------
	_EditControlHandle = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		nullptr,
		WS_CHILD | WS_VISIBLE | ES_LEFT,
		0,
		0,
		_CX,
		_CY,
		*this,
		reinterpret_cast<HMENU>(1001),
		nullptr,
		nullptr
	);
	::SendMessageW(
		_EditControlHandle, 
		WM_SETFONT, 
		reinterpret_cast<WPARAM>(_Font->getFontHandle()), 
		/*reinterpret_cast<LPARAM>*/(TRUE)
	);


	if (!_String.empty())
	{
		SetWindowTextW(_EditControlHandle, _String.c_str());
	}


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void InputStringBox::setString(std::wstring const& value)
{
	_String = value;
}

std::wstring InputStringBox::getString(void)
{
	return _String;
}

void InputStringBox::onEndDialog(INT_PTR result)
{
	CX_RUNTIME_LOG(cxLTrace) <<
		L"InputStringBox::onEndDialog"
		;

	if (_EditControlHandle)
	{
		_String = cx::wui::getWindowText(_EditControlHandle);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool showInputStringBox(HWND hwnd, std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy, std::wstring& value)
{
	InputStringBox box;
	box.setString(value);
	box.setupInputStringBox(x, y, cx, cy);
	if (IDOK == box.doModal(hwnd))
	{
		value = box.getString();
		return true;
	}
	return false;
}




