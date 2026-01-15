/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "PropertyValueBox.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
PropertyValueBox::PropertyValueBox(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy) :
	_X(x),
	_Y(y),
	_CX(cx),
	_CY(cy)
{
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void PropertyValueBox::initializeDialogTemplate(void)
{
	cx::wui::MemoryDialogTemplateWriter w(_DialogTemplate);


	w.BEGIN_DIALOG(0, 0, _CX, _CY);
		w.DIALOG_STYLE  (DS_SETFONT | WS_POPUP | WS_BORDER | WS_VSCROLL);
		w.DIALOG_FONT   (9, L"Segoe UI");
	w.END_DIALOG();
	w.BEGIN_CONTROLS_MAP();
	w.END_CONTROLS_MAP();


	setTemplate(_DialogTemplate.getTemplatePtr());
}

//===========================================================================
void PropertyValueBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &PropertyValueBox::onInitDialog;
	_WindowMessageMap[WM_COMMAND   ] = &PropertyValueBox::onCommand;
	_WindowMessageMap[WM_NCACTIVATE] = &PropertyValueBox::onNcActivate;
	_WindowMessageMap[WM_ACTIVATE  ] = &PropertyValueBox::onActivate;
	_WindowMessageMap[WM_SETFOCUS] = &PropertyValueBox::onSetFocus;
	_WindowMessageMap[WM_KILLFOCUS] = &PropertyValueBox::onKillFocus;
	_WindowMessageMap[WM_USER+1] = &PropertyValueBox::onUser1;
	_WindowMessageMap[WM_PAINT] = &PropertyValueBox::onPaint;
}

LRESULT PropertyValueBox::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLTrace) <<
		L"PropertyValueBox::onWindowMessage(): uMsg=" << windowMessage.uMsg
		;
#endif
	return cx::wui::MessageMapWindowT<PropertyValueBox, cx::wui::ModalIndirectBaseDialog>::onWindowMessage(windowMessage);
}

void PropertyValueBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	MoveWindow(*this, _X, _Y, _CX, _CY, TRUE);


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void PropertyValueBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDOK:
		::EndDialog(*this, IDOK);
		windowMessage.setResult(TRUE);
		break;

	case IDCANCEL:
		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
		break;

	default:
		windowMessage.setResult(FALSE);
		break;
	}
}

void PropertyValueBox::onNcActivate(cx::wui::WindowMessage& windowMessage)
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

void PropertyValueBox::onActivate(cx::wui::WindowMessage& windowMessage)
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

void PropertyValueBox::onSetFocus(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_SETFOCUS_WindowMessageCrack wm{ windowMessage };

	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_SETFOCUS"
		;
}

void PropertyValueBox::onKillFocus(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_KILLFOCUS_WindowMessageCrack wm{ windowMessage };

	CX_RUNTIME_LOG(cxLTrace) <<
		L"WM_KILLFOCUS"
		;
}

void PropertyValueBox::onUser1(cx::wui::WindowMessage& windowMessage)
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

		::EndDialog(*this, IDCANCEL);
	}
}

void PropertyValueBox::onPaint(cx::wui::WindowMessage& windowMessage)
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
