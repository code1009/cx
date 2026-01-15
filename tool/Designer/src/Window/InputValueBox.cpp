/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>
#include <cx/common/common.hpp>
#include <cx/wui/wui.hpp>

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
void InputValueBox::setup(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy)
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
	_WindowMessageMap[WM_COMMAND] = &InputValueBox::onCommand;
	_WindowMessageMap[WM_NCACTIVATE] = &InputValueBox::onNcActivate;
	_WindowMessageMap[WM_ACTIVATE] = &InputValueBox::onActivate;
	_WindowMessageMap[WM_SETFOCUS] = &InputValueBox::onSetFocus;
	_WindowMessageMap[WM_KILLFOCUS] = &InputValueBox::onKillFocus;
	_WindowMessageMap[WM_USER + 1] = &InputValueBox::onUser1;
	_WindowMessageMap[WM_PAINT] = &InputValueBox::onPaint;
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
InputTextBox::InputTextBox()
{
}

//===========================================================================
void InputTextBox::setup(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy, bool readOnly, TextType textType)
{
	//-----------------------------------------------------------------------
	_X = x;
	_Y = y;
	_CX = cx;
	_CY = cy;
	_ReadOnly = readOnly;
	

	//-----------------------------------------------------------------------
	_TextType = textType;


	//-----------------------------------------------------------------------
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void InputTextBox::initializeDialogTemplate(void)
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
void InputTextBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &InputTextBox::onInitDialog;
	_WindowMessageMap[WM_COMMAND] = &InputTextBox::onCommand;
	_WindowMessageMap[WM_NCACTIVATE] = &InputTextBox::onNcActivate;
	_WindowMessageMap[WM_USER + 1] = &InputTextBox::onUser1;
}

//===========================================================================
void InputTextBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	MoveWindow(*this, _X, _Y, _CX, _CY, FALSE);


	//-----------------------------------------------------------------------
	_Font = std::make_unique<cx::wui::Font>(L"맑은 고딕", 100);


	//-----------------------------------------------------------------------
	DWORD dwStyle;
	dwStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT;
	if (_ReadOnly)
	{
		dwStyle |= ES_READONLY;
	}

	_EditControlHandle = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		nullptr,
		dwStyle,
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
	if (!_Text.empty())
	{
		SetWindowTextW(_EditControlHandle, _Text.c_str());
	}


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void InputTextBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDOK:
		onEndDialog(IDOK);
		::EndDialog(*this, IDOK);
		windowMessage.setResult(TRUE);
		break;

	case IDCANCEL:
		onEndDialog(IDCANCEL);
		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
		break;

	default:
		windowMessage.setResult(FALSE);
		break;
	}
}

void InputTextBox::onNcActivate(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_NCACTIVATE_WindowMessageCrack wm{ windowMessage };

	BOOL active = wm.bActive();
	if (!active)
	{
		PostMessage(*this, WM_USER + 1, 1009, 1);
	}
}

void InputTextBox::onUser1(cx::wui::WindowMessage& windowMessage)
{
	if (windowMessage.wParam = 1009 && windowMessage.lParam == 1)
	{
		onEndDialog(IDOK);
		::EndDialog(*this, IDOK);
	}
}

void InputTextBox::setText(std::wstring const& text)
{
	_Text = text;
}

std::wstring InputTextBox::getText(void)
{
	return _Text;
}

void InputTextBox::onEndDialog(INT_PTR result)
{
	CX_RUNTIME_LOG(cxLTrace) <<
		L"InputIntegerBox::onEndDialog"
		;

	if (_EditControlHandle)
	{
		std::wstring text = cx::wui::getWindowText(_EditControlHandle);
		switch (_TextType)
		{
		case TextType::String:
			_Text = text;
			break;

		case TextType::Float:
			if (cx::is_float_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Double:
			if (cx::is_double_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt8:
			if (cx::is_uint8_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt16:
			if (cx::is_uint16_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt32:
			if (cx::is_uint32_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt64:
			if (cx::is_uint64_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int8:
			if (cx::is_int8_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int16:
			if (cx::is_int16_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int32:
			if (cx::is_int32_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int64:
			if (cx::is_int64_std_wstring(text))
			{
				_Text = text;
			}
			break;

		default:
			break;
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
InputTextListBox::InputTextListBox()
{
}

//===========================================================================
void InputTextListBox::setup(std::uint32_t x, std::uint32_t y, std::uint32_t cx, std::uint32_t cy, bool readOnly, TextType textType)
{
	//-----------------------------------------------------------------------
	_X = x;
	_Y = y;
	_CX = cx;
	_CY = cy;
	_ReadOnly = readOnly;


	//-----------------------------------------------------------------------
	_TextType = textType;


	//-----------------------------------------------------------------------
	initializeDialogTemplate();

	registerWindowMessageMap();
}

//===========================================================================
void InputTextListBox::initializeDialogTemplate(void)
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
void InputTextListBox::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_INITDIALOG] = &InputTextListBox::onInitDialog;
	_WindowMessageMap[WM_COMMAND] = &InputTextListBox::onCommand;
	_WindowMessageMap[WM_NCACTIVATE] = &InputTextListBox::onNcActivate;
	_WindowMessageMap[WM_USER + 1] = &InputTextListBox::onUser1;
}

//===========================================================================
void InputTextListBox::onInitDialog(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	MoveWindow(*this, _X, _Y, _CX, _CY, FALSE);


	//-----------------------------------------------------------------------
	_Font = std::make_unique<cx::wui::Font>(L"맑은 고딕", 100);


	//-----------------------------------------------------------------------
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_BORDER | WS_VSCROLL;
	if (_ReadOnly)
	{
		dwStyle |= LBS_NOSEL;
	}

	_ListBoxControlHandle = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"LISTBOX",
		nullptr,
		dwStyle,
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
		_ListBoxControlHandle,
		WM_SETFONT,
		reinterpret_cast<WPARAM>(_Font->getFontHandle()),
		/*reinterpret_cast<LPARAM>*/(TRUE)
	);
	for (const auto& item : _TextList)
	{
		::SendMessageW(_ListBoxControlHandle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(item.c_str()));
	}
	if (!_Text.empty())
	{
		int index = static_cast<int>(::SendMessageW(_ListBoxControlHandle, LB_FINDSTRINGEXACT, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(_Text.c_str())));
		if (index != LB_ERR)
		{
			::SendMessageW(_ListBoxControlHandle, LB_SETCURSEL, static_cast<WPARAM>(index), 0);
		}
	}


	//-----------------------------------------------------------------------
	windowMessage.setResult(TRUE);
}

void InputTextListBox::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case IDOK:
		onEndDialog(IDOK);
		::EndDialog(*this, IDOK);
		windowMessage.setResult(TRUE);
		break;

	case IDCANCEL:
		onEndDialog(IDCANCEL);
		::EndDialog(*this, IDCANCEL);
		windowMessage.setResult(TRUE);
		break;

	default:
		windowMessage.setResult(FALSE);
		break;
	}
}

void InputTextListBox::onNcActivate(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_NCACTIVATE_WindowMessageCrack wm{ windowMessage };

	BOOL active = wm.bActive();
	if (!active)
	{
		PostMessage(*this, WM_USER + 1, 1009, 1);
	}
}

void InputTextListBox::onUser1(cx::wui::WindowMessage& windowMessage)
{
	if (windowMessage.wParam = 1009 && windowMessage.lParam == 1)
	{
		onEndDialog(IDOK);
		::EndDialog(*this, IDOK);
	}
}

void InputTextListBox::setTextList(std::vector<std::wstring> const& textList)
{
	_TextList = textList;
}

void InputTextListBox::setText(std::wstring const& text)
{
	_Text = text;
}

std::wstring InputTextListBox::getText(void)
{
	return _Text;
}

void InputTextListBox::onEndDialog(INT_PTR result)
{
	CX_RUNTIME_LOG(cxLTrace) <<
		L"InputIntegerBox::onEndDialog"
		;

	if (_ListBoxControlHandle)
	{
		std::wstring text;
		int selectedIndex = static_cast<int>(::SendMessageW(_ListBoxControlHandle, LB_GETCURSEL, 0, 0));
		if (selectedIndex != LB_ERR)
		{
			int textLength = static_cast<int>(::SendMessageW(_ListBoxControlHandle, LB_GETTEXTLEN, static_cast<WPARAM>(selectedIndex), 0));
			if (textLength > 0)
			{
				std::vector<wchar_t> buffer(textLength + 1);
				::SendMessageW(_ListBoxControlHandle, LB_GETTEXT, static_cast<WPARAM>(selectedIndex), reinterpret_cast<LPARAM>(buffer.data()));
				text = std::wstring(buffer.data());
			}
		}
		switch (_TextType)
		{
		case TextType::String:
			_Text = text;
			break;

		case TextType::Float:
			if (cx::is_float_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Double:
			if (cx::is_double_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt8:
			if (cx::is_uint8_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt16:
			if (cx::is_uint16_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt32:
			if (cx::is_uint32_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::UInt64:
			if (cx::is_uint64_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int8:
			if (cx::is_int8_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int16:
			if (cx::is_int16_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int32:
			if (cx::is_int32_std_wstring(text))
			{
				_Text = text;
			}
			break;

		case TextType::Int64:
			if (cx::is_int64_std_wstring(text))
			{
				_Text = text;
			}
			break;

		default:
			break;
		}
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool showInputTextBox(
	HWND hwnd, 
	std::uint32_t x, std::uint32_t y, 
	std::uint32_t cx, std::uint32_t cy,
	bool readOnly,
	InputTextBox::TextType textType, 
	std::wstring& text
)
{
	InputTextBox box;
	box.setText(text);
	box.setup(x, y, cx, cy, readOnly, textType);
	if (IDOK == box.doModal(hwnd))
	{
		text = box.getText();
		return true;
	}
	return false;
}


bool showInputTextListBox(
	HWND hwnd,
	std::uint32_t x, std::uint32_t y,
	std::uint32_t cx, std::uint32_t cy,
	bool readOnly,
	InputTextListBox::TextType textType,
	std::vector<std::wstring> textList,
	std::wstring& text
)
{
	InputTextListBox box;
	box.setTextList(textList);
	box.setText(text);
	box.setup(x, y, cx, cy, readOnly, textType);
	if (IDOK == box.doModal(hwnd))
	{
		text = box.getText();
		return true;
	}
	return false;
}
