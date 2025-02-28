/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../wui.hpp"

//===========================================================================
#include <assert.h>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
From:
https://github.com/Win32-WTL/WTL/blob/main/Include/atldlgs.h :3194
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MemoryDialogTemplate::MemoryDialogTemplate() : 
	_DataPtr(nullptr)
{
}

MemoryDialogTemplate::~MemoryDialogTemplate()
{
	reset();
}

//===========================================================================
bool MemoryDialogTemplate::isValid() const
{
	return !_DataBuffer.empty();
}

bool MemoryDialogTemplate::isTemplateEx() const
{
	return (
			isValid() && 
			reinterpret_cast<const DLGTEMPLATEEX*>(_DataBuffer.data())->signature == 0xFFFF
		);
}

DLGTEMPLATE* MemoryDialogTemplate::getTemplatePtr()
{
	return reinterpret_cast<DLGTEMPLATE*>(_DataBuffer.data());
}

DLGTEMPLATEEX* MemoryDialogTemplate::getTemplateExPtr()
{
	return reinterpret_cast<DLGTEMPLATEEX*>(_DataBuffer.data());
}

void MemoryDialogTemplate::reset()
{
	_DataBuffer.clear();
	_DataPtr = nullptr;
}

//===========================================================================
void MemoryDialogTemplate::addData(const void* pData, std::size_t nData)
{
	const SIZE_T ALLOCATION_INCREMENT = 1024;


	assert(pData != NULL);
	if (_DataBuffer.empty())
	{
		_DataBuffer.resize(((nData / ALLOCATION_INCREMENT) + 1) * ALLOCATION_INCREMENT);
		_DataPtr = _DataBuffer.data();
	}
	else if ((_DataPtr - _DataBuffer.data() + nData) > _DataBuffer.size())
	{
		std::size_t ptrPos = _DataPtr - _DataBuffer.data();
		_DataBuffer.resize(_DataBuffer.size() + ((nData / ALLOCATION_INCREMENT) + 1) * ALLOCATION_INCREMENT);
		_DataPtr = _DataBuffer.data() + ptrPos;
	}

	std::memcpy(_DataPtr, pData, nData);
	_DataPtr += nData;
}

void MemoryDialogTemplate::addString(LPCWSTR lpszStr)
{
	if (lpszStr == NULL)
	{
		WCHAR szEmpty = 0;
		addData(&szEmpty, sizeof(szEmpty));
	}
	else
	{
		int nSize = lstrlenW(lpszStr) + 1;
		addData(lpszStr, nSize * sizeof(WCHAR));
	}
}

//===========================================================================
void MemoryDialogTemplate::create(
	bool bDlgEx, 
	LPCWSTR lpszCaption, 
	const RECT& rc, DWORD 
	dwStyle, 
	DWORD dwExStyle,
	LPCWSTR lpstrFontName, 
	WORD wFontSize, WORD 
	wWeight, 
	BYTE bItalic, 
	BYTE bCharset, 
	DWORD dwHelpID,
	ResourceName ClassName, 
	ResourceName Menu
)
{
	create(
		bDlgEx, 
		lpszCaption, 
		(short)rc.left, 
		(short)rc.top, 
		(short)(rc.right - rc.left), 
		(short)(rc.bottom - rc.top), 
		dwStyle, 
		dwExStyle,
		lpstrFontName, 
		wFontSize, 
		wWeight, 
		bItalic, 
		bCharset, 
		dwHelpID, 
		ClassName._Name, 
		Menu._Name
	);
}

void MemoryDialogTemplate::create(
	bool bDlgEx, 
	LPCWSTR lpszCaption, 
	short nX, 
	short nY, 
	short nWidth, 
	short nHeight, 
	DWORD dwStyle, 
	DWORD dwExStyle,
	LPCWSTR lpstrFontName, 
	WORD wFontSize, 
	WORD wWeight, 
	BYTE bItalic, 
	BYTE bCharset,
	DWORD dwHelpID,
	ResourceName ClassName, 
	ResourceName Menu
)
{
	if (lpstrFontName != NULL)
	{
		dwStyle |= DS_SETFONT;
	}
	else
	{
		dwStyle &= ~DS_SETFONT;
	}

	if (bDlgEx)
	{
		DLGTEMPLATEEX dlg = { 1, 0xFFFF, dwHelpID, dwExStyle, dwStyle, 0, nX, nY, nWidth, nHeight };
		addData(&dlg, sizeof(dlg));
	}
	else
	{
		DLGTEMPLATE dlg = { dwStyle, dwExStyle, 0, nX, nY, nWidth, nHeight };
		addData(&dlg, sizeof(dlg));
	}

#ifndef _WIN32_WCE
	if (Menu._Name == NULL)
	{
		WORD menuData = 0;
		addData(&menuData, sizeof(WORD));
	}
	else if (IS_INTRESOURCE(Menu._Name))
	{
		WORD menuData[] = { 0xFFFF, LOWORD(Menu._Name) };
		addData(menuData, sizeof(menuData));
	}
	else
	{
		addString(Menu._Name);
	}
#else
	assert(Menu._Name == NULL);
	Menu._Name;
	WORD menuData = 0;
	addData(&menuData, sizeof(WORD));
#endif

	if (ClassName._Name == NULL)
	{
		WORD classData = 0;
		addData(&classData, sizeof(WORD));
	}
	else if (IS_INTRESOURCE(ClassName._Name))
	{
		WORD classData[] = { 0xFFFF, LOWORD(ClassName._Name) };
		addData(classData, sizeof(classData));
	}
	else
	{
		addString(ClassName._Name);
	}

	addString(lpszCaption);

	if (lpstrFontName != NULL)
	{
		addData(&wFontSize, sizeof(wFontSize));

		if (bDlgEx)
		{
			addData(&wWeight, sizeof(wWeight));
			addData(&bItalic, sizeof(bItalic));
			addData(&bCharset, sizeof(bCharset));
		}

		addString(lpstrFontName);
	}
}

void MemoryDialogTemplate::addControl(
	ResourceName ClassName, 
	WORD wId, 
	const RECT& rc, 
	DWORD dwStyle, 
	DWORD dwExStyle,
	ResourceName Text, 
	const WORD* pCreationData, 
	WORD nCreationData, 
	DWORD dwHelpID
)
{
	addControl(
		ClassName._Name, 
		wId, 
		(short)rc.left, 
		(short)rc.top, 
		(short)(rc.right - rc.left), 
		(short)(rc.bottom - rc.top), 
		dwStyle, 
		dwExStyle,
		Text._Name, 
		pCreationData, 
		nCreationData, 
		dwHelpID
	);
}

void MemoryDialogTemplate::addControl(
	ResourceName ClassName, 
	WORD wId, 
	short nX, 
	short nY, 
	short nWidth, 
	short nHeight, 
	DWORD dwStyle, 
	DWORD dwExStyle,
	ResourceName Text, 
	const WORD* pCreationData, 
	WORD nCreationData, 
	DWORD dwHelpID
)
{
	assert(isValid());


	const DWORD_PTR dwDwordAlignBits = sizeof(DWORD) - 1;
	_DataPtr = 
		reinterpret_cast<std::uint8_t*>(
			(reinterpret_cast<std::uintptr_t>(_DataPtr) + dwDwordAlignBits) & (~dwDwordAlignBits)
		);


	if (isTemplateEx())
	{
		DLGTEMPLATEEX* dlg = reinterpret_cast<DLGTEMPLATEEX*>(_DataBuffer.data());
		dlg->cDlgItems++;

		DLGITEMTEMPLATEEX item = { dwHelpID, ControlWindowStyleEx | dwExStyle, ControlWindowStyle | dwStyle, nX, nY, nWidth, nHeight, wId };
		addData(&item, sizeof(item));
	}
	else
	{
		DLGTEMPLATE* dlg = reinterpret_cast<DLGTEMPLATE*>(_DataBuffer.data());
		dlg->cdit++;

		DLGITEMTEMPLATE item = { ControlWindowStyle | dwStyle, ControlWindowStyleEx | dwExStyle, nX, nY, nWidth, nHeight, wId };
		addData(&item, sizeof(item));
	}

	assert(ClassName._Name != NULL);
	if (IS_INTRESOURCE(ClassName._Name))
	{
		WORD wData[] = { 0xFFFF, LOWORD(ClassName._Name) };
		addData(wData, sizeof(wData));
	}
	else
	{
		addString(ClassName._Name);
	}

	if (Text._Name == NULL)
	{
		WORD classData = 0;
		addData(&classData, sizeof(WORD));
	}
	else if (IS_INTRESOURCE(Text._Name))
	{
		WORD wData[] = { 0xFFFF, LOWORD(Text._Name) };
		addData(wData, sizeof(wData));
	}
	else
	{
		addString(Text._Name);
	}

	addData(&nCreationData, sizeof(nCreationData));

	if ((nCreationData != 0))
	{
		assert(pCreationData != NULL);
		addData(pCreationData, nCreationData * sizeof(WORD));
	}
}

void MemoryDialogTemplate::addStdControl(
	StdCtrlType CtrlType, 
	WORD wId, 
	short nX, 
	short nY, 
	short nWidth, 
	short nHeight,
	DWORD dwStyle, 
	DWORD dwExStyle, 
	ResourceName Text, 
	const WORD* pCreationData, 
	WORD nCreationData, 
	DWORD dwHelpID
)
{
	addControl(
		CtrlType, 
		wId, 
		nX, 
		nY, 
		nWidth, 
		nHeight, 
		dwStyle, 
		dwExStyle, 
		Text, 
		pCreationData, 
		nCreationData, 
		dwHelpID
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MemoryDialogTemplateWriter::MemoryDialogTemplateWriter(MemoryDialogTemplate& memoryDialogTemplate) :
	_MemoryDialogTemplate(memoryDialogTemplate)
{
}

//===========================================================================
void MemoryDialogTemplateWriter::BEGIN_DIALOG(short x, short y, short width, short height)
{
	bExTemplate = false;

	nX = x, nY = y, nWidth = width, nHeight = height;
	szCaption = NULL;
	dwStyle = WS_POPUP | WS_BORDER | WS_SYSMENU;
	dwExStyle = 0;
	szFontName = NULL;
	wFontSize = 0;
	wWeight = 0;
	bItalic = 0;
	bCharset = 0;
	dwHelpID = 0;
	Menu = 0U;
	ClassName = 0U;
}

void MemoryDialogTemplateWriter::BEGIN_DIALOG_EX(short x, short y, short width, short height, DWORD helpID)
{ 
	bExTemplate = true;

	nX = x, nY = y, nWidth = width, nHeight = height; 
	szCaption = NULL; 
	dwStyle = WS_POPUP | WS_BORDER | WS_SYSMENU; 
	dwExStyle = 0; 
	szFontName = NULL; 
	wFontSize = 0; 
	wWeight = 0; 
	bItalic = 0; 
	bCharset = 0; 
	dwHelpID = helpID; 
	Menu = 0U; 
	ClassName = 0U;
}

void MemoryDialogTemplateWriter::END_DIALOG(void)
{
	_MemoryDialogTemplate.create(
		bExTemplate,

		szCaption,
		nX, nY,
		nWidth,
		nHeight,
		dwStyle,
		dwExStyle,
		szFontName,
		wFontSize,
		wWeight,
		bItalic,
		bCharset,
		dwHelpID,
		ClassName,
		Menu
	);
};

void MemoryDialogTemplateWriter::DIALOG_CAPTION(LPCWSTR caption)
{
	szCaption = caption;
}

void MemoryDialogTemplateWriter::DIALOG_STYLE(DWORD style)
{
	dwStyle = style;
}

void MemoryDialogTemplateWriter::DIALOG_EXSTYLE(DWORD exStyle)
{
	dwExStyle = exStyle;
}

void MemoryDialogTemplateWriter::DIALOG_FONT(WORD pointSize, LPCWSTR typeFace)
{
	wFontSize = pointSize;
	szFontName = typeFace;
}

void MemoryDialogTemplateWriter::DIALOG_FONT_EX(WORD pointSize, LPCWSTR typeFace, WORD weight, BYTE italic, BYTE charset)
{
	assert(bExTemplate);

	wFontSize = pointSize;
	szFontName = typeFace;
	wWeight = weight;
	bItalic = italic;
	bCharset = charset;
}

void MemoryDialogTemplateWriter::DIALOG_MENU(ResourceName menuName)
{
	Menu = menuName;
}

void MemoryDialogTemplateWriter::DIALOG_CLASS(ResourceName className)
{
	ClassName = className;
}

void MemoryDialogTemplateWriter::BEGIN_CONTROLS_MAP()
{
}

void MemoryDialogTemplateWriter::END_CONTROLS_MAP()
{
}

void MemoryDialogTemplateWriter::CONTROL_LTEXT(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_STATIC, 
		id, 
		x, 
		y, 
		width,
		height, 
		style | SS_LEFT | WS_GROUP, 
		exStyle, 
		text, 
		NULL, 
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_CTEXT(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_STATIC,
		id,
		x,
		y,
		width,
		height,
		style | SS_CENTER | WS_GROUP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_RTEXT(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_STATIC,
		id,
		x,
		y,
		width,
		height,
		style | SS_RIGHT | WS_GROUP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_PUSHBUTTON(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_PUSHBUTTON | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_DEFPUSHBUTTON(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_DEFPUSHBUTTON | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_PUSHBOX(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_PUSHBOX | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_STATE3(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_3STATE | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_AUTO3STATE(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_AUTO3STATE | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_CHECKBOX(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_CHECKBOX | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_AUTOCHECKBOX(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_AUTOCHECKBOX | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_RADIOBUTTON(
	LPCWSTR text,
	WORD id,
	short x,
	short y,
	short width,
	short height,
	DWORD style,
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_RADIOBUTTON | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_AUTORADIOBUTTON(
	LPCWSTR text, 
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_AUTORADIOBUTTON | WS_TABSTOP,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_COMBOBOX(
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_COMBOBOX,
		id,
		x,
		y,
		width,
		height,
		style | CBS_DROPDOWN | WS_TABSTOP,
		exStyle,
		(LPCWSTR)NULL,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_EDITTEXT(
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_EDIT,
		id,
		x,
		y,
		width,
		height,
		style | ES_LEFT | WS_BORDER | WS_TABSTOP,
		exStyle,
		(LPCWSTR)NULL,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_GROUPBOX(
	LPCWSTR text, 
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_BUTTON,
		id,
		x,
		y,
		width,
		height,
		style | BS_GROUPBOX,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_LISTBOX(
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_LISTBOX,
		id,
		x,
		y,
		width,
		height,
		style | LBS_NOTIFY | WS_BORDER,
		exStyle,
		(LPCWSTR)NULL,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_SCROLLBAR(
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_SCROLLBAR,
		id,
		x,
		y,
		width,
		height,
		style | SBS_HORZ,
		exStyle,
		(LPCWSTR)NULL,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_ICON(
	LPCWSTR text, 
	WORD id, 
	short x, 
	short y, 
	short width, 
	short height, 
	DWORD style, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addStdControl(
		MemoryDialogTemplate::CTRL_STATIC,
		id,
		x,
		y,
		width,
		height,
		style | SS_ICON,
		exStyle,
		text,
		NULL,
		0
	);
}

void MemoryDialogTemplateWriter::CONTROL_CONTROL(
	LPCWSTR text, 
	WORD id, 
	ResourceName className, 
	DWORD style,
	short x,
	short y, 
	short width, 
	short height, 
	DWORD exStyle
)
{
	_MemoryDialogTemplate.addControl(
		className,
		id,
		x,
		y,
		width,
		height,
		style,
		exStyle,
		text,
		NULL,
		0
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


