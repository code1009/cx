#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma pack(push, 4)

typedef struct _DLGITEMTEMPLATEEX
{
	DWORD  helpID;
	DWORD  exStyle;
	DWORD  style;
	short  x;
	short  y;
	short  cx;
	short  cy;
	DWORD id;
} DLGITEMTEMPLATEEX;

typedef struct _DLGTEMPLATEEX
{
	WORD  dlgVer;
	WORD  signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD  cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;

#pragma pack(pop)





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MemoryDialogTemplate final
{
public:
	enum StdCtrlType
	{
		CTRL_BUTTON    = 0x0080,
		CTRL_EDIT      = 0x0081,
		CTRL_STATIC    = 0x0082,
		CTRL_LISTBOX   = 0x0083,
		CTRL_SCROLLBAR = 0x0084,
		CTRL_COMBOBOX  = 0x0085
	};

private:
	std::vector<std::uint8_t> _DataBuffer;
	std::uint8_t* _DataPtr;

public:
	MemoryDialogTemplate(const MemoryDialogTemplate&) = delete;
	MemoryDialogTemplate& operator=(const MemoryDialogTemplate&) = delete;

	MemoryDialogTemplate(MemoryDialogTemplate&&) = delete;
	MemoryDialogTemplate& operator=(MemoryDialogTemplate&&) = delete;

public:
	MemoryDialogTemplate();
	virtual ~MemoryDialogTemplate();

public:
	bool isValid() const;
	bool isTemplateEx() const;
	DLGTEMPLATE* getTemplatePtr();
	DLGTEMPLATEEX* getTemplateExPtr();

public:
	void reset(void);
	void addData(const void* pData, std::size_t nData);
	void addString(LPCWSTR lpszStr);


public:
	void create(
		bool bDlgEx,
		LPCWSTR lpszCaption,
		const RECT& rc,
		DWORD dwStyle = 0,
		DWORD dwExStyle = 0,
		LPCWSTR lpstrFontName = NULL,
		WORD wFontSize = 0,
		WORD wWeight = 0,
		BYTE bItalic = 0,
		BYTE bCharset = 0,
		DWORD dwHelpID = 0,
		ResourceName ClassName = 0U,
		ResourceName Menu = 0U
	);
	void create(
		bool bDlgEx,
		LPCWSTR lpszCaption,
		short nX,
		short nY,
		short nWidth,
		short nHeight,
		DWORD dwStyle = 0,
		DWORD dwExStyle = 0,
		LPCWSTR lpstrFontName = NULL,
		WORD wFontSize = 0,
		WORD wWeight = 0,
		BYTE bItalic = 0,
		BYTE bCharset = 0,
		DWORD dwHelpID = 0,
		ResourceName ClassName = 0U,
		ResourceName Menu = 0U
	);

public:
	void addControl(
		ResourceName ClassName,
		WORD wId,
		const RECT& rc,
		DWORD dwStyle,
		DWORD dwExStyle,
		ResourceName Text,
		const WORD* pCreationData = NULL,
		WORD nCreationData = 0,
		DWORD dwHelpID = 0
	);
	void addControl(
		ResourceName ClassName,
		WORD wId,
		short nX,
		short nY,
		short nWidth,
		short nHeight,
		DWORD dwStyle,
		DWORD dwExStyle,
		ResourceName Text,
		const WORD* pCreationData = NULL,
		WORD nCreationData = 0,
		DWORD dwHelpID = 0
	);

public:
	void addStdControl(
		StdCtrlType CtrlType,
		WORD wId,
		short nX,
		short nY,
		short nWidth,
		short nHeight,
		DWORD dwStyle,
		DWORD dwExStyle,
		ResourceName Text,
		const WORD* pCreationData = NULL,
		WORD nCreationData = 0,
		DWORD dwHelpID = 0
	);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MemoryDialogTemplateWriter final
{
public:
	MemoryDialogTemplate& _MemoryDialogTemplate;

	bool    bExTemplate = false;

	short   nX         = 0; 
	short   nY         = 0; 
	short   nWidth     = 0;
	short   nHeight    = 0;
	LPCWSTR szCaption  = NULL;
	DWORD   dwStyle    = WS_POPUP | WS_BORDER | WS_SYSMENU;
	DWORD   dwExStyle  = 0;
	LPCWSTR szFontName = NULL;
	WORD    wFontSize  = 0;
	WORD    wWeight    = 0;
	BYTE    bItalic    = 0;
	BYTE    bCharset   = 0;
	DWORD   dwHelpID   = 0;
	ResourceName Menu       = 0U;
	ResourceName ClassName  = 0U;

public:
	MemoryDialogTemplateWriter(MemoryDialogTemplate& memoryDialogTemplate);

public:
	MemoryDialogTemplateWriter(const MemoryDialogTemplateWriter&) = delete;
	MemoryDialogTemplateWriter& operator=(const MemoryDialogTemplateWriter&) = delete;

	MemoryDialogTemplateWriter(MemoryDialogTemplateWriter&&) = delete;
	MemoryDialogTemplateWriter& operator=(MemoryDialogTemplateWriter&&) = delete;

public:
	void BEGIN_DIALOG(short x, short y, short width, short height);
	void BEGIN_DIALOG_EX(short x, short y, short width, short height, DWORD helpID);
	void END_DIALOG(void);

public:
	void DIALOG_CAPTION(LPCWSTR caption);
	void DIALOG_STYLE(DWORD style);
	void DIALOG_EXSTYLE(DWORD exStyle=0);
	void DIALOG_FONT(WORD pointSize, LPCWSTR typeFace);
	void DIALOG_FONT_EX(WORD pointSize, LPCWSTR typeFace, WORD weight, BYTE italic, BYTE charset);
	void DIALOG_MENU(ResourceName menuName);
	void DIALOG_CLASS(ResourceName className);

public:
	void BEGIN_CONTROLS_MAP();
	void END_CONTROLS_MAP();

public:
	void CONTROL_LTEXT          (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_CTEXT          (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_RTEXT          (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_PUSHBUTTON     (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_DEFPUSHBUTTON  (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_PUSHBOX        (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_STATE3         (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_AUTO3STATE     (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_CHECKBOX       (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_AUTOCHECKBOX   (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_RADIOBUTTON    (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_AUTORADIOBUTTON(LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_COMBOBOX       (              WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_EDITTEXT       (              WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_GROUPBOX       (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_LISTBOX        (              WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_SCROLLBAR      (              WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_ICON           (LPCWSTR text, WORD id, short x, short y, short width, short height, DWORD style=0, DWORD exStyle=0);
	void CONTROL_CONTROL        (LPCWSTR text, WORD id, ResourceName className, DWORD style,
	                                                    short x, short y, short width, short height,              DWORD exStyle=0);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


