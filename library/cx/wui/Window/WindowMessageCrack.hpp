#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowMessageCrack
{
public:
	WindowMessage& _WindowMessage;

public:
	WindowMessageCrack(WindowMessage& windowMessage);

public:
	WindowMessageCrack(const WindowMessageCrack&) = delete;
	WindowMessageCrack& operator=(const WindowMessageCrack&) = delete;

	WindowMessageCrack(WindowMessageCrack&&) = delete;
	WindowMessageCrack& operator=(WindowMessageCrack&&) = delete;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
From:
https://github.com/Win32-WTL/WTL/blob/main/Include/atlcrack.h
*/





/////////////////////////////////////////////////////////////////////////////
//
// Standard Windows message
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//int OnCreate(LPCREATESTRUCT lpCreateStruct)
/*
WM_CREATE
		lResult = (LRESULT)func((LPCREATESTRUCT)lParam);
*/
class WM_CREATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPCREATESTRUCT lpCreateStruct(void){return (LPCREATESTRUCT)(_WindowMessage.lParam);}

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
/*
WM_INITDIALOG
		lResult = (LRESULT)func((HWND)wParam, lParam);
*/
class WM_INITDIALOG_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND   wndFocus  (void){return (HWND)(_WindowMessage.wParam);}
	LPARAM lInitParam(void){return (_WindowMessage.lParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// BOOL OnCopyData(CWindow wnd, PCOPYDATASTRUCT pCopyDataStruct)
/*
WM_COPYDATA
		lResult = (LRESULT)func((HWND)wParam, (PCOPYDATASTRUCT)lParam);
*/
class WM_COPYDATA_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND            wnd            (void){return (HWND)(_WindowMessage.wParam);}
	PCOPYDATASTRUCT pCopyDataStruct(void){return (PCOPYDATASTRUCT)(_WindowMessage.lParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
//void OnDestroy()
/*
WM_DESTROY
	func(); \
	lResult = 0; \
*/
class WM_DESTROY_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMove(CPoint ptPos)
/*
WM_MOVE
		func(_WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_MOVE_WindowMessageCrack : public WindowMessageCrack
{
public:
	POINT point(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };

		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnSize(UINT nType, CSize size)
/*
WM_SIZE
		func((UINT)wParam, _WTYPES_NS::CSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_SIZE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nTYPE(void)
	{
		return (UINT)(_WindowMessage.wParam);
	}

	SIZE size(void)
	{
		SIZE size{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };

		return size;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
/*
WM_ACTIVATE
		func((UINT)LOWORD(wParam), (BOOL)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
class WM_ACTIVATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nState    (void){return (UINT)LOWORD(_WindowMessage.wParam);}
	BOOL bMinimized(void){return (BOOL)HIWORD(_WindowMessage.wParam);}
	HWND wndOther  (void){return (HWND)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnSetFocus(CWindow wndOld)
/*
WM_SETFOCUS
		func((HWND)wParam); \
		lResult = 0; \
*/
class WM_SETFOCUS_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndOld(void){return (HWND)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnKillFocus(CWindow wndFocus)
/*
WM_KILLFOCUS
		func((HWND)wParam); \
		lResult = 0; \
*/
class WM_KILLFOCUS_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndFocus(void){return (HWND)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnEnable(BOOL bEnable)
/*
WM_ENABLE
		func((BOOL)wParam); \
		lResult = 0; \
*/
class WM_ENABLE_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL bEnable(void){return (BOOL)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnPaint(CDCHandle dc)
/*
WM_PAINT
		func((HDC)wParam); \
		lResult = 0; \
*/
class WM_PAINT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC hdc(void){return (HDC)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnClose()
/*
WM_CLOSE
		func(); \
		lResult = 0; \
*/
class WM_CLOSE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnQueryEndSession(UINT nSource, UINT uLogOff)
/*
WM_QUERYENDSESSION
		lResult = (LRESULT)func((UINT)wParam, (UINT)lParam); \
*/
class WM_QUERYENDSESSION_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nSource(void){return (UINT)(_WindowMessage.wParam);}
	UINT uLogOff(void){return (UINT)(_WindowMessage.lParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// BOOL OnQueryOpen()
/*
WM_QUERYOPEN
		lResult = (LRESULT)func(); \
*/
class WM_QUERYOPEN_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// BOOL OnEraseBkgnd(CDCHandle dc)
/*
WM_ERASEBKGND
		lResult = (LRESULT)func((HDC)wParam); \
*/
class WM_ERASEBKGND_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC hdc(void){return (HDC)(_WindowMessage.wParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnSysColorChange()
/*
WM_SYSCOLORCHANGE
		func(); \
		lResult = 0; \
*/
class WM_SYSCOLORCHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnEndSession(BOOL bEnding, UINT uLogOff)
/*
WM_ENDSESSION
		func((BOOL)wParam, (UINT)lParam); \
		lResult = 0; \
*/
class WM_ENDSESSION_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL bEnding(void){return (BOOL)(_WindowMessage.wParam);}
	UINT uLogOff(void){return (UINT)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnShowWindow(BOOL bShow, UINT nStatus)
/*
WM_SHOWWINDOW
		func((BOOL)wParam, (int)lParam); \
		lResult = 0; \
*/
class WM_SHOWWINDOW_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL bShow  (void){return (BOOL)(_WindowMessage.wParam);}
	UINT nStatus(void){return (UINT)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit)
/*
WM_CTLCOLOREDIT
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
class WM_CTLCOLOREDIT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC  hdc(void){return (HDC) (_WindowMessage.wParam);}
	HWND ctl(void){return (HWND)(_WindowMessage.lParam);}

public:
	void Result(HBRUSH rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox)
/*
WM_CTLCOLORLISTBOX
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using WM_CTLCOLORLISTBOX_WindowMessageCrack = WM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnCtlColorBtn(CDCHandle dc, CButton button)
/*
WM_CTLCOLORBTN
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using WM_CTLCOLORBTN_WindowMessageCrack = WM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnCtlColorDlg(CDCHandle dc, CWindow wnd)
/*
WM_CTLCOLORDLG
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using WM_CTLCOLORDLG_WindowMessageCrack = WM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnCtlColorScrollBar(CDCHandle dc, CScrollBar scrollBar)
/*
WM_CTLCOLORSCROLLBAR
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using WM_CTLCOLORSCROLLBAR_WindowMessageCrack = WM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
/*
WM_CTLCOLORSTATIC
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using WM_CTLCOLORSTATIC_WindowMessageCrack = WM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// void OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
/*
WM_SETTINGCHANGE
		func((UINT)wParam, (LPCTSTR)lParam); \
		lResult = 0; \
*/
class WM_SETTINGCHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT    uFlags     (void){return (UINT)   (_WindowMessage.wParam);}
	LPCTSTR lpszSection(void){return (LPCTSTR)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// OnDevModeChange(LPCTSTR lpDeviceName)
/*
WM_DEVMODECHANGE
		func((LPCTSTR)lParam); \
		lResult = 0; \
*/
class WM_DEVMODECHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPCTSTR lpDeviceName(void){return (LPCTSTR)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnActivateApp(BOOL bActive, DWORD dwThreadID)
/*
WM_ACTIVATEAPP
		func((BOOL)wParam, (DWORD)lParam); \
		lResult = 0; \
*/
class WM_ACTIVATEAPP_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL  bActive   (void){return (BOOL) (_WindowMessage.wParam);}
	DWORD dwThreadID(void){return (DWORD)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnFontChange()
/*
WM_FONTCHANGE
		func(); \
		lResult = 0; \
*/
class WM_FONTCHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnTimeChange()
/*
WM_TIMECHANGE
		func(); \
		lResult = 0; \
*/
class WM_TIMECHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnCancelMode()
/*
WM_CANCELMODE
		func(); \
		lResult = 0; \
*/
class WM_CANCELMODE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
/*
WM_SETCURSOR
		lResult = (LRESULT)func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
*/
class WM_SETCURSOR_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wnd     (void){return (HWND)(_WindowMessage.wParam);}
	UINT nHitTest(void){return (UINT)LOWORD(_WindowMessage.lParam);}
	UINT message (void){return (UINT)HIWORD(_WindowMessage.lParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// int OnMouseActivate(CWindow wndTopLevel, UINT nHitTest, UINT message)
/*
WM_MOUSEACTIVATE
		lResult = (LRESULT)func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
*/
class WM_MOUSEACTIVATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndTopLevel(void){return (HWND)(_WindowMessage.wParam);}
	UINT nHitTest   (void){return (UINT)LOWORD(_WindowMessage.lParam);}
	UINT message    (void){return (UINT)HIWORD(_WindowMessage.lParam);}

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnChildActivate()
/*
WM_CHILDACTIVATE
		func(); \
		lResult = 0; \
*/
class WM_CHILDACTIVATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnGetMinMaxInfo(LPMINMAXINFO lpMMI)
/*
WM_GETMINMAXINFO
		func((LPMINMAXINFO)lParam); \
		lResult = 0; \
*/
class WM_GETMINMAXINFO_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPMINMAXINFO lpMMI(void){return (LPMINMAXINFO)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnIconEraseBkgnd(CDCHandle dc)
/*
WM_ICONERASEBKGND
		func((HDC)wParam); \
		lResult = 0; \
*/
class WM_ICONERASEBKGND_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC hdc(void){return (HDC)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnSpoolerStatus(UINT nStatus, UINT nJobs)
/*
WM_SPOOLERSTATUS
		func((UINT)wParam, (UINT)LOWORD(lParam)); \
		lResult = 0; \
*/
class WM_SPOOLERSTATUS_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nStatus(void) {return (UINT)(_WindowMessage.wParam);}
	UINT nJobs  (void) {return (UINT)LOWORD(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
/*
WM_DRAWITEM
		func((UINT)wParam, (LPDRAWITEMSTRUCT)lParam); \
		lResult = TRUE; \
*/
class WM_DRAWITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int              nIDCtl          (void) {return (UINT)(_WindowMessage.wParam);}
	LPDRAWITEMSTRUCT lpDrawItemStruct(void) {return (LPDRAWITEMSTRUCT)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
/*
WM_MEASUREITEM
		func((UINT)wParam, (LPMEASUREITEMSTRUCT)lParam); \
		lResult = TRUE; \
*/
class WM_MEASUREITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int                 nIDCtl             (void) {return (UINT)(_WindowMessage.wParam);}
	LPMEASUREITEMSTRUCT lpMeasureItemStruct(void) {return (LPMEASUREITEMSTRUCT)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
/*
WM_DELETEITEM
		func((UINT)wParam, (LPDELETEITEMSTRUCT)lParam); \
		lResult = TRUE; \
*/
class WM_DELETEITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int                nIDCtl            (void) {return (UINT)(_WindowMessage.wParam);}
	LPDELETEITEMSTRUCT lpDeleteItemStruct(void) {return (LPDELETEITEMSTRUCT)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
//int OnCharToItem(UINT nChar, UINT nIndex, CListBox listBox)
/*
WM_CHARTOITEM
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
*/
class WM_CHARTOITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nChar  (void) {return (UINT)LOWORD(_WindowMessage.wParam);}
	UINT nIndex (void) {return (UINT)HIWORD(_WindowMessage.wParam);}
	HWND listBox(void) {return (HWND)(_WindowMessage.lParam);}

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// int OnVKeyToItem(UINT nKey, UINT nIndex, CListBox listBox)
/*
WM_VKEYTOITEM
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
*/
class WM_VKEYTOITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nKey   (void) {return (UINT)LOWORD(_WindowMessage.wParam);}
	UINT nIndex (void) {return (UINT)HIWORD(_WindowMessage.wParam);}
	HWND listBox(void) {return (HWND)(_WindowMessage.lParam);}

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// HCURSOR OnQueryDragIcon()
/*
WM_QUERYDRAGICON
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
*/
class WM_QUERYDRAGICON_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(HCURSOR rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
/*
WM_COMPAREITEM
		lResult = (LRESULT)func((UINT)wParam, (LPCOMPAREITEMSTRUCT)lParam); \
*/
class WM_COMPAREITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int                 nIDCtl             (void) {return (UINT)(_WindowMessage.wParam);}
	LPCOMPAREITEMSTRUCT lpCompareItemStruct(void) {return (LPCOMPAREITEMSTRUCT)(_WindowMessage.lParam);}

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnCompacting(UINT nCpuTime)
/*
WM_COMPACTING
		func((UINT)wParam); \
		lResult = 0; \
*/
class WM_COMPACTING_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nCpuTime(void){return (UINT)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct)
/*
WM_NCCREATE
		lResult = (LRESULT)func((LPCREATESTRUCT)lParam);
*/
class WM_NCCREATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPCREATESTRUCT lpCreateStruct(void){return (LPCREATESTRUCT)(_WindowMessage.lParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnNcDestroy()
/*
WM_NCDESTROY
		func(); \
		lResult = 0; \
*/
using WM_NCDESTROY_WindowMessageCrack = WM_DESTROY_WindowMessageCrack;

//===========================================================================
// LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam)
/*
WM_NCCALCSIZE
		lResult = func((BOOL)wParam, lParam); \
*/
class WM_NCCALCSIZE_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL   bCalcValidRects(void){return (BOOL)  (_WindowMessage.wParam);}
	LPARAM lParam         (void){return (LPARAM)(_WindowMessage.lParam);}

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// UINT OnNcHitTest(CPoint point)
/*
WM_NCHITTEST
		lResult = (LRESULT)func(_WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
*/
class WM_NCHITTEST_WindowMessageCrack : public WindowMessageCrack
{
public:
	POINT point(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };

		return pt;
	}

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnNcPaint(CRgnHandle rgn)
/*
WM_NCPAINT
		func((HRGN)wParam); \
		lResult = 0; \
*/
class WM_NCPAINT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HRGN hrgn(void){return (HRGN)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnNcActivate(BOOL bActive)
/*
WM_NCACTIVATE
		lResult = (LRESULT)func((BOOL)wParam); \
*/
class WM_NCACTIVATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL bActive(void) {return (BOOL)(_WindowMessage.wParam);}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// UINT OnGetDlgCode(LPMSG lpMsg)
/*
WM_GETDLGCODE
		lResult = (LRESULT)func((LPMSG)lParam); \
*/
class WM_GETDLGCODE_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPMSG lpMsg(void){return (LPMSG)(_WindowMessage.lParam);}

public:
	void Result(UINT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnNcMouseMove(UINT nHitTest, CPoint point)
/*
WM_NCMOUSEMOVE
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_NCMOUSEMOVE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nHitTest(void)
	{
		return (UINT)(_WindowMessage.wParam);
	}

	POINT point(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };

		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnNcLButtonDown(UINT nHitTest, CPoint point)
/*
WM_NCLBUTTONDOW
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCLBUTTONDOW_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcLButtonUp(UINT nHitTest, CPoint point)
/*
WM_NCLBUTTONUP
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCLBUTTONUP_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcLButtonDblClk(UINT nHitTest, CPoint point)
/*
WM_NCLBUTTONDBLCLK
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCLBUTTONDBLCLK_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcRButtonDown(UINT nHitTest, CPoint point)
/*
WM_NCRBUTTONDOW
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCRBUTTONDOW_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcRButtonUp(UINT nHitTest, CPoint point)
/*
WM_NCRBUTTONUP
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCRBUTTONUP_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcRButtonDblClk(UINT nHitTest, CPoint point)
/*
WM_NCRBUTTONDBLCLK
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCRBUTTONDBLCLK_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcMButtonDown(UINT nHitTest, CPoint point)
/*
WM_NCMBUTTONDOW
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCMBUTTONDOW_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcMButtonUp(UINT nHitTest, CPoint point)
/*
WM_NCMBUTTONUP
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCMBUTTONUP_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnNcMButtonDblClk(UINT nHitTest, CPoint point)
/*
WM_NCMBUTTONDBLCLK
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCMBUTTONDBLCLK_WindowMessageCrack = WM_NCMOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
WM_KEYDOWN
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
class WM_KEYDOWN_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nChar(void)
	{
		return (TCHAR)(_WindowMessage.wParam);
	}

	UINT nRepCnt(void)
	{
		return (UINT)((_WindowMessage.lParam) & 0xFFFF);
	}

	UINT nFlags(void)
	{
		return (UINT)(((_WindowMessage.lParam) & 0xFFFF0000) >> 16);
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
WM_KEYUP
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using WM_KEYUP_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
WM_CHAR
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using WM_CHAR_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
WM_DEADCHAR
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using WM_DEADCHAR_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
WM_SYSKEYDOWN
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using WM_SYSKEYDOWN_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
 WM_SYSKEYUP
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using  WM_SYSKEYUP_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
 WM_SYSCHAR
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using  WM_SYSCHAR_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags)
/*
 WM_SYSDEADCHAR
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		lResult = 0; \
*/
using  WM_SYSDEADCHAR_WindowMessageCrack = WM_KEYDOWN_WindowMessageCrack;

//===========================================================================
// void OnSysCommand(UINT nID, CPoint point)
/*
WM_SYSCOMMAND
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_SYSCOMMAND_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nID(void)
	{
		return (TCHAR)(_WindowMessage.wParam);
	}

	POINT point(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };

		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnTCard(UINT idAction, DWORD dwActionData)
/*
WM_TCARD
		func((UINT)wParam, (DWORD)lParam); \
		lResult = 0; \
*/
class WM_TCARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT idAction     (void) {return (UINT) (_WindowMessage.wParam);}
	DWORD dwActionData(void) {return (DWORD)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnTimer(UINT_PTR nIDEvent)
/*
WM_TIMER
		func((UINT_PTR)wParam); \
		lResult = 0; \
*/
class WM_TIMER_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT_PTR nIDEvent(void) {return (UINT_PTR)(_WindowMessage.wParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
/*
WM_HSCROLL
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
class WM_HSCROLL_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nSBCode     (void) {return (int)  LOWORD(_WindowMessage.wParam);}
	UINT nPos        (void) {return (short)HIWORD(_WindowMessage.wParam);}
	HWND wndScrollBar(void) {return (HWND)(_WindowMessage.lParam);}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
/*
WM_VSCROLL
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
using  WM_VSCROLL_WindowMessageCrack = WM_HSCROLL_WindowMessageCrack;

//===========================================================================
// void OnInitMenu(CMenuHandle menu)
/*
WM_INITMENU
		func((HMENU)wParam); \
		lResult = 0; \
*/
class WM_INITMENU_WindowMessageCrack : public WindowMessageCrack
{
public:
	HMENU menu(void) { return (HMENU)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnInitMenuPopup(CMenuHandle menuPopup, UINT nIndex, BOOL bSysMenu)
/*
WM_INITMENUPOPUP
		func((HMENU)wParam, (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)); \
		lResult = 0; \
*/
class WM_INITMENUPOPUP_WindowMessageCrack : public WindowMessageCrack
{
public:
	HMENU menuPopup(void) { return (HMENU)(_WindowMessage.wParam); }
	UINT  nIndex   (void) { return (UINT)LOWORD(_WindowMessage.lParam); }
	BOOL  bSysMenu (void) { return (BOOL)HIWORD(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMenuSelect(UINT nItemID, UINT nFlags, CMenuHandle menu)
/*
WM_MENUSELECT
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
		lResult = 0; \
*/
class WM_MENUSELECT_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT  nItemID(void) { return (UINT)LOWORD(_WindowMessage.wParam); }
	UINT  nFlags (void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	HMENU menu   (void) { return (HMENU)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenuHandle menu)
/*
WM_MENUCHAR
		lResult = func((TCHAR)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam); \
*/
class WM_MENUCHAR_WindowMessageCrack : public WindowMessageCrack
{
public:
	TCHAR nChar(void) { return (TCHAR)LOWORD(_WindowMessage.wParam); }
	UINT nFlags(void) { return (UINT) HIWORD(_WindowMessage.wParam); }
	HMENU menu (void) { return (HMENU)(_WindowMessage.lParam); }

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// LRESULT OnNotify(int idCtrl, LPNMHDR pnmh)
/*
WM_NOTIFY
		lResult = func((int)wParam, (LPNMHDR)lParam); \
*/
class WM_NOTIFY_WindowMessageCrack : public WindowMessageCrack
{
public:
	int     idCtrl(void) { return (int)    (_WindowMessage.wParam); }
	LPNMHDR pnmh  (void) { return (LPNMHDR)(_WindowMessage.lParam); }

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnEnterIdle(UINT nWhy, CWindow wndWho)
/*
WM_ENTERIDLE
		func((UINT)wParam, (HWND)lParam); \
		lResult = 0; \
*/
class WM_ENTERIDLE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nWhy  (void) { return (UINT)(_WindowMessage.wParam); }
	HWND wndWho(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMouseMove(UINT nFlags, CPoint point)
/*
WM_MOUSEMOVE
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_MOUSEMOVE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nFlags(void) 
	{
		return (UINT)(_WindowMessage.wParam);
	}

	POINT point(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
/*
WM_MOUSEWHEEL
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
*/
class WM_MOUSEWHEEL_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nFlags(void) 
	{
		return (UINT)LOWORD(_WindowMessage.wParam);
	}

	short zDelta(void) 
	{
		return (short)HIWORD(_WindowMessage.wParam);
	}

	POINT pt(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return pt;
	}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnLButtonDown(UINT nFlags, CPoint point)
/*
WM_LBUTTONDOWN
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_LBUTTONDOWN_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnLButtonUp(UINT nFlags, CPoint point)
/*
WM_LBUTTONUP
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_LBUTTONUP_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnLButtonDblClk(UINT nFlags, CPoint point)
/*
WM_LBUTTONDBLCLK
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_LBUTTONDBLCLK_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnRButtonDown(UINT nFlags, CPoint point)
/*
WM_RBUTTONDOWN
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_RBUTTONDOWN_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnRButtonUp(UINT nFlags, CPoint point)
/*
WM_RBUTTONUP
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_RBUTTONUP_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnRButtonDblClk(UINT nFlags, CPoint point)
/*
WM_RBUTTONDBLCLK
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_RBUTTONDBLCLK_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnMButtonDown(UINT nFlags, CPoint point)
/*
WM_MBUTTONDOWN
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_MBUTTONDOWN_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnMButtonUp(UINT nFlags, CPoint point)
/*
WM_MBUTTONUP
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_MBUTTONUP_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnMButtonDblClk(UINT nFlags, CPoint point)
/*
WM_MBUTTONDBLCLK
		func((UINT)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_MBUTTONDBLCLK_WindowMessageCrack = WM_MOUSEMOVE_WindowMessageCrack;

//===========================================================================
// void OnParentNotify(UINT message, UINT nChildID, LPARAM lParam)
/*
WM_PARENTNOTIFY
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), lParam); \
		lResult = 0; \
*/
class WM_PARENTNOTIFY_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT   message (void) { return (UINT)LOWORD(_WindowMessage.wParam); }
	UINT   nChildID(void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	LPARAM lParam  (void) { return (_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMDIActivate(CWindow wndActivate, CWindow wndDeactivate)
/*
WM_MDIACTIVATE
		func((HWND)wParam, (HWND)lParam); \
		lResult = 0; \
*/
class WM_MDIACTIVATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndActivate  (void) { return (HWND)(_WindowMessage.wParam); }
	HWND wndDeactivate(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnRenderFormat(UINT nFormat)
/*
WM_RENDERFORMAT
		func((UINT)wParam); \
		lResult = 0; \
*/
class WM_RENDERFORMAT_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nFormat(void) { return (UINT)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnRenderAllFormats()
/*
WM_RENDERALLFORMATS
		func(); \
		lResult = 0; \
*/
class WM_RENDERALLFORMATS_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnDestroyClipboard()
/*
WM_DESTROYCLIPBOARD
		func(); \
		lResult = 0; \
*/
class WM_DESTROYCLIPBOARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnDrawClipboard()
/*
WM_DRAWCLIPBOARD
		func(); \
		lResult = 0; \
*/
class WM_DRAWCLIPBOARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnPaintClipboard(CWindow wndViewer, const LPPAINTSTRUCT lpPaintStruct)
/*
WM_PAINTCLIPBOARD
		func((HWND)wParam, (const LPPAINTSTRUCT)::GlobalLock((HGLOBAL)lParam)); \
		::GlobalUnlock((HGLOBAL)lParam); \
		lResult = 0; \
*/
class WM_PAINTCLIPBOARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndViewer (void) 
	{
		return (HWND)(_WindowMessage.wParam); 
	}

	const LPPAINTSTRUCT lpPaintStruct(void) 
	{
		return (const LPPAINTSTRUCT)::GlobalLock((HGLOBAL)(_WindowMessage.lParam)); 
	}

public:
	void release_lpPaintStruct(void)
	{
		::GlobalUnlock((HGLOBAL)(_WindowMessage.lParam));
	}

public:
	void Result(void)
	{
		::GlobalUnlock((HGLOBAL)(_WindowMessage.lParam));
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnVScrollClipboard(CWindow wndViewer, UINT nSBCode, UINT nPos)
/*
WM_VSCROLLCLIPBOARD
		func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
*/
class WM_VSCROLLCLIPBOARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndViewer(void) { return (HWND)(_WindowMessage.wParam); }
	UINT nSBCode  (void) { return (UINT)LOWORD(_WindowMessage.lParam); }
	UINT nPos     (void) { return (UINT)HIWORD(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnContextMenu(CWindow wnd, CPoint point)
/*
WM_CONTEXTMENU
		func((HWND)wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_CONTEXTMENU_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wnd(void)
	{ 
		return (HWND)(_WindowMessage.wParam); 
	}

	POINT point(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnSizeClipboard(CWindow wndViewer, const LPRECT lpRect)
/*
WM_SIZECLIPBOARD
		func((HWND)wParam, (const LPRECT)::GlobalLock((HGLOBAL)lParam)); \
		::GlobalUnlock((HGLOBAL)lParam); \
		lResult = 0; \
*/
class WM_SIZECLIPBOARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndViewer(void) 
	{
		return (HWND)(_WindowMessage.wParam); 
	}

	const LPRECT lpRect(void) 
	{ 
		return (const LPRECT)::GlobalLock((HGLOBAL)(_WindowMessage.lParam)); 
	}

public:
	void release_lpRect(void)
	{
		::GlobalUnlock((HGLOBAL)(_WindowMessage.lParam));
	}

public:
	void Result(void)
	{
		::GlobalUnlock((HGLOBAL)(_WindowMessage.lParam));
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString)
/*
WM_ASKCBFORMATNAME
		func((UINT)wParam, (LPTSTR)lParam); \
		lResult = 0; \
*/
class WM_ASKCBFORMATNAME_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT   nMaxCount (void) { return (UINT)  (_WindowMessage.wParam); }
	LPTSTR lpszString(void) { return (LPTSTR)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnChangeCbChain(CWindow wndRemove, CWindow wndAfter)
/*
WM_CHANGECBCHAIN
		func((HWND)wParam, (HWND)lParam); \
		lResult = 0; \
*/
class WM_CHANGECBCHAIN_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndRemove(void) { return (HWND)(_WindowMessage.wParam); }
	HWND wndAfter (void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnHScrollClipboard(CWindow wndViewer, UINT nSBCode, UINT nPos)
/*
WM_HSCROLLCLIPBOARD
		func((HWND)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
*/
class WM_HSCROLLCLIPBOARD_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndViewer(void) { return (HWND)(_WindowMessage.wParam); }
	UINT nSBCode  (void) { return (UINT)LOWORD(_WindowMessage.lParam); }
	UINT nPos     (void) { return (UINT)HIWORD(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnQueryNewPalette()
/*
WM_QUERYNEWPALETTE
		lResult = (LRESULT)func(); \
*/
class WM_QUERYNEWPALETTE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnPaletteChanged(CWindow wndFocus)
/*
WM_PALETTECHANGED
		func((HWND)wParam); \
		lResult = 0; \
*/
class WM_PALETTECHANGED_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndFocus(void) { return (HWND)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnPaletteIsChanging(CWindow wndPalChg)
/*
WM_PALETTEISCHANGING
		func((HWND)wParam); \
		lResult = 0; \
*/
class WM_PALETTEISCHANGING_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndPalChg(void) { return (HWND)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnDropFiles(HDROP hDropInfo)
/*
WM_DROPFILES
		func((HDROP)wParam); \
		lResult = 0; \
*/
class WM_DROPFILES_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDROP hDropInfo(void) { return (HDROP)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnWindowPosChanging(LPWINDOWPOS lpWndPos)
/*
WM_WINDOWPOSCHANGING
		func((LPWINDOWPOS)lParam); \
		lResult = 0; \
*/
class WM_WINDOWPOSCHANGING_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPWINDOWPOS lpWndPos(void) { return (LPWINDOWPOS)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnWindowPosChanged(LPWINDOWPOS lpWndPos)
/*
WM_WINDOWPOSCHANGED
		func((LPWINDOWPOS)lParam); \
		lResult = 0; \
*/
class WM_WINDOWPOSCHANGED_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPWINDOWPOS lpWndPos(void) { return (LPWINDOWPOS)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnExitMenuLoop(BOOL fIsTrackPopupMenu)
/*
WM_EXITMENULOOP
		func((BOOL)wParam); \
		lResult = 0; \
*/
class WM_EXITMENULOOP_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL fIsTrackPopupMenu(void) { return (BOOL)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnEnterMenuLoop(BOOL fIsTrackPopupMenu)
/*
WM_ENTERMENULOOP
		func((BOOL)wParam); \
		lResult = 0; \
*/
class WM_ENTERMENULOOP_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL fIsTrackPopupMenu(void) { return (BOOL)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
/*
WM_STYLECHANGED
		func((UINT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
*/
class WM_STYLECHANGED_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT          nStyleType   (void) { return (UINT)(_WindowMessage.wParam); }
	LPSTYLESTRUCT lpStyleStruct(void) { return (LPSTYLESTRUCT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
/*
WM_STYLECHANGING
		func((UINT)wParam, (LPSTYLESTRUCT)lParam); \
		lResult = 0; \
*/
class WM_STYLECHANGING_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT          nStyleType   (void) { return (UINT)(_WindowMessage.wParam); }
	LPSTYLESTRUCT lpStyleStruct(void) { return (LPSTYLESTRUCT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnSizing(UINT fwSide, LPRECT pRect)
/*
WM_SIZING
		func((UINT)wParam, (LPRECT)lParam); \
		lResult = TRUE; \
*/
class WM_SIZING_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT   fwSide(void) { return (UINT)  (_WindowMessage.wParam); }
	LPRECT pRect (void) { return (LPRECT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnMoving(UINT fwSide, LPRECT pRect)
/*
WM_MOVING
		func((UINT)wParam, (LPRECT)lParam); \
		lResult = TRUE; \
*/
class WM_MOVING_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT   fwSide(void) { return (UINT)  (_WindowMessage.wParam); }
	LPRECT pRect (void) { return (LPRECT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnCaptureChanged(CWindow wnd)
/*
WM_CAPTURECHANGED
		func((HWND)lParam); \
		lResult = 0; \
*/
class WM_CAPTURECHANGED_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wnd(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
/*
WM_DEVICECHANGE
		lResult = (LRESULT)func((UINT)wParam, (DWORD_PTR)lParam); \
*/
class WM_DEVICECHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT      nEventType(void) { return (UINT)     (_WindowMessage.wParam); }
	DWORD_PTR dwData    (void) { return (DWORD_PTR)(_WindowMessage.lParam); }

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl)
/*
WM_COMMAND
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
class WM_COMMAND_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT uNotifyCode(void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	int  nID        (void) { return (int) LOWORD(_WindowMessage.wParam); }
	HWND wndCtl     (void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnDisplayChange(UINT uBitsPerPixel, CSize sizeScreen)
/*
WM_DISPLAYCHANGE
		func((UINT)wParam, _WTYPES_NS::CSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_DISPLAYCHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT uBitsPerPixel(void)
	{
		return (UINT)(_WindowMessage.wParam); 
	}

	SIZE sizeScreen(void)
	{
		SIZE size{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return size;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnEnterSizeMove()
/*
WM_ENTERSIZEMOVE
		func(); \
		lResult = 0; \
*/
class WM_ENTERSIZEMOVE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnExitSizeMove()
/*
WM_EXITSIZEMOVE
		func(); \
		lResult = 0; \
*/
class WM_EXITSIZEMOVE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// HFONT OnGetFont()
/*
WM_GETFONT
		lResult = (LRESULT)func(); \
*/
class WM_GETFONT_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(HFONT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// LRESULT OnGetHotKey()
/*
WM_GETHOTKEY
		lResult = func(); \
*/
class WM_GETHOTKEY_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// HICON OnGetIcon()
/*
WM_GETICON
		lResult = (LRESULT)func((UINT)wParam); \
*/
class WM_GETICON_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nType(void) { return (UINT)(_WindowMessage.wParam); }

public:
	void Result(HICON rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// int OnGetText(int cchTextMax, LPTSTR lpszText)
/*
WM_GETTEXT
		lResult = (LRESULT)func((int)wParam, (LPTSTR)lParam); \
*/
class WM_GETTEXT_WindowMessageCrack : public WindowMessageCrack
{
public:
	int    cchTextMax(void) { return (int)   (_WindowMessage.wParam); }
	LPTSTR lpszText  (void) { return (LPTSTR)(_WindowMessage.lParam); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// int OnGetTextLength()
/*
WM_GETTEXTLENGTH
		lResult = (LRESULT)func(); \
*/
class WM_GETTEXTLENGTH_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnHelp(LPHELPINFO lpHelpInfo)
/*
WM_HELP
		func((LPHELPINFO)lParam); \
		lResult = TRUE; \
*/
class WM_HELP_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPHELPINFO lpHelpInfo(void) { return (LPHELPINFO)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
/*
WM_HOTKEY
		func((int)wParam, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)); \
		lResult = 0; \
*/
class WM_HOTKEY_WindowMessageCrack : public WindowMessageCrack
{
public:
	int  nHotKeyID (void) { return (int)(_WindowMessage.wParam); }
	UINT uModifiers(void) { return (UINT)LOWORD(_WindowMessage.lParam); }
	UINT uVirtKey  (void) { return (UINT)HIWORD(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnInputLangChange(DWORD dwCharSet, HKL hKbdLayout)
/*
WM_INPUTLANGCHANGE
		func((DWORD)wParam, (HKL)lParam); \
		lResult = TRUE; \
*/
class WM_INPUTLANGCHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	DWORD dwCharSet (void) { return (DWORD)(_WindowMessage.wParam); }
	HKL   hKbdLayout(void) { return (HKL)  (_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnInputLangChangeRequest(BOOL bSysCharSet, HKL hKbdLayout)
/*
WM_INPUTLANGCHANGEREQUEST
		func((BOOL)wParam, (HKL)lParam); \
		lResult = 0; \
*/
class WM_INPUTLANGCHANGEREQUEST_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL bSysCharSet(void) { return (BOOL)(_WindowMessage.wParam); }
	HKL  hKbdLayout (void) { return (HKL) (_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnNextDlgCtl(BOOL bHandle, WPARAM wCtlFocus)
/*
WM_NEXTDLGCTL
		func((BOOL)LOWORD(lParam), wParam); \
		lResult = 0; \
*/
class WM_NEXTDLGCTL_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL   bHandle  (void) { return (BOOL)LOWORD(_WindowMessage.lParam); }
	WPARAM wCtlFocus(void) { return (_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnNextMenu(int nVirtKey, LPMDINEXTMENU lpMdiNextMenu)
/*
WM_NEXTMENU
		func((int)wParam, (LPMDINEXTMENU)lParam); \
		lResult = 0; \
*/
class WM_NEXTMENU_WindowMessageCrack : public WindowMessageCrack
{
public:
	int           nVirtKey     (void) { return (int)(_WindowMessage.wParam); }
	LPMDINEXTMENU lpMdiNextMenu(void) { return (LPMDINEXTMENU)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// int OnNotifyFormat(CWindow wndFrom, int nCommand)
/*
WM_NOTIFYFORMAT
		lResult = (LRESULT)func((HWND)wParam, (int)lParam); \
*/
class WM_NOTIFYFORMAT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND wndFrom (void) { return (HWND)(_WindowMessage.wParam); }
	int  nCommand(void) { return (int) (_WindowMessage.lParam); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// BOOL OnPowerBroadcast(DWORD dwPowerEvent, DWORD_PTR dwData)
/*
WM_POWERBROADCAST
		lResult = (LRESULT)func((DWORD)wParam, (DWORD_PTR)lParam); \
*/
class WM_POWERBROADCAST_WindowMessageCrack : public WindowMessageCrack
{
public:
	DWORD     dwPowerEvent(void) { return (DWORD)    (_WindowMessage.wParam); }
	DWORD_PTR dwData      (void) { return (DWORD_PTR)(_WindowMessage.lParam); }

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnPrint(CDCHandle dc, UINT uFlags)
/*
WM_PRINT
		func((HDC)wParam, (UINT)lParam); \
		lResult = 0; \
*/
class WM_PRINT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC  hdc   (void) { return (HDC) (_WindowMessage.wParam); }
	UINT uFlags(void) { return (UINT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnPrintClient(CDCHandle dc, UINT uFlags)
/*
WM_PRINTCLIENT
		func((HDC)wParam, (UINT)lParam); \
		lResult = 0; \
*/
class WM_PRINTCLIENT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC  hdc   (void) { return (HDC) (_WindowMessage.wParam); }
	UINT uFlags(void) { return (UINT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
#if 0
// void OnRasDialEvent(RASCONNSTATE rasconnstate, DWORD dwError)
/*
WM_RASDIALEVENT
		func((RASCONNSTATE)wParam, (DWORD)lParam); \
		lResult = TRUE; \
*/
class WM_RASDIALEVENT_WindowMessageCrack : public WindowMessageCrack
{
public:
	RASCONNSTATE rasconnstate(void) { return (RASCONNSTATE)(_WindowMessage.wParam); }
	DWORD        dwError     (void) { return (DWORD)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};
#endif

//===========================================================================
// void OnSetFont(CFontHandle font, BOOL bRedraw)
/*
WM_SETFONT
		func((HFONT)wParam, (BOOL)LOWORD(lParam)); \
		lResult = 0; \
*/
class WM_SETFONT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HFONT font   (void) { return (HFONT)(_WindowMessage.wParam); }
	BOOL  bRedraw(void) { return (BOOL)LOWORD(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// int OnSetHotKey(int nVirtKey, UINT uFlags)
/*
WM_SETHOTKEY
		lResult = (LRESULT)func((int)LOBYTE(LOWORD(wParam)), (UINT)HIBYTE(LOWORD(wParam))); \
*/
class WM_SETHOTKEY_WindowMessageCrack : public WindowMessageCrack
{
public:
	int  nVirtKey(void) { return (int) LOBYTE(LOWORD(_WindowMessage.wParam)); }
	UINT uFlags  (void) { return (UINT)HIBYTE(LOWORD(_WindowMessage.wParam)); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// HICON OnSetIcon(UINT uType, HICON hIcon)
/*
WM_SETICON
		lResult = (LRESULT)func((UINT)wParam, (HICON)lParam); \
*/
class WM_SETICON_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT  uType(void) { return (UINT) (_WindowMessage.wParam); }
	HICON hIcon(void) { return (HICON)(_WindowMessage.lParam); }

public:
	void Result(HICON rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnSetRedraw(BOOL bRedraw)
/*
WM_SETREDRAW
		func((BOOL)wParam); \
		lResult = 0; \
*/
class WM_SETREDRAW_WindowMessageCrack : public WindowMessageCrack
{
public:
	BOOL bRedraw(void) { return (BOOL)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// int OnSetText(LPCTSTR lpstrText)
/*
WM_SETTEXT
		lResult = (LRESULT)func((LPCTSTR)lParam); \
*/
class WM_SETTEXT_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPCTSTR lpstrText(void) { return (LPCTSTR)(_WindowMessage.lParam); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnUserChanged()
/*
WM_USERCHANGED
		func(); \
		lResult = 0; \
*/
class WM_USERCHANGED_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};





/////////////////////////////////////////////////////////////////////////////
//
// New NT4 & NT5 messages
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// void OnMouseHover(WPARAM wParam, CPoint ptPos)
/*
WM_MOUSEHOVER
		func(wParam, _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_MOUSEHOVER_WindowMessageCrack : public WindowMessageCrack
{
public:
	WPARAM wParam(void) 
	{ 
		return _WindowMessage.wParam; 
	}

	POINT ptPos(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMouseLeave()
/*
WM_MOUSELEAVE
		func(); \
		lResult = 0; \
*/
class WM_MOUSELEAVE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMenuRButtonUp(WPARAM wParam, CMenuHandle menu)
/*
WM_MENURBUTTONUP
		func(wParam, (HMENU)lParam); \
		lResult = 0; \
*/
class WM_MENURBUTTONUP_WindowMessageCrack : public WindowMessageCrack
{
public:
	WPARAM wParam(void) { return (_WindowMessage.wParam); }
	HMENU  menu  (void) { return (HMENU)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// LRESULT OnMenuDrag(WPARAM wParam, CMenuHandle menu)
/*
WM_MENUDRAG
		lResult = func(wParam, (HMENU)lParam); \
*/
class WM_MENUDRAG_WindowMessageCrack : public WindowMessageCrack
{
public:
	WPARAM wParam(void) { return (_WindowMessage.wParam); }
	HMENU  menu  (void) { return (HMENU)(_WindowMessage.lParam); }

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// LRESULT OnMenuGetObject(PMENUGETOBJECTINFO info)
/*
WM_MENUGETOBJECT
		lResult = func((PMENUGETOBJECTINFO)lParam); \
*/
class WM_MENUGETOBJECT_WindowMessageCrack : public WindowMessageCrack
{
public:
	PMENUGETOBJECTINFO info(void) { return (PMENUGETOBJECTINFO)(_WindowMessage.lParam); }

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnUnInitMenuPopup(UINT nID, CMenuHandle menu)
/*
WM_UNINITMENUPOPUP
		func((UINT)HIWORD(lParam), (HMENU)wParam); \
		lResult = 0; \
*/
class WM_UNINITMENUPOPUP_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT  nID (void) { return (UINT)HIWORD(_WindowMessage.lParam); }
	HMENU menu(void) { return (HMENU)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnMenuCommand(WPARAM nIndex, CMenuHandle menu)
/*
WM_MENUCOMMAND
		func(wParam, (HMENU)lParam); \
		lResult = 0; \
*/
class WM_MENUCOMMAND_WindowMessageCrack : public WindowMessageCrack
{
public:
	WPARAM nIndex(void) { return (_WindowMessage.wParam); }
	HMENU  menu  (void) { return (HMENU)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnAppCommand(CWindow wndFocus, short cmd, WORD uDevice, int dwKeys)
/*
WM_APPCOMMAND
		lResult = (LRESULT)func((HWND)wParam, GET_APPCOMMAND_LPARAM(lParam), GET_DEVICE_LPARAM(lParam), GET_KEYSTATE_LPARAM(lParam)); \
*/
class WM_APPCOMMAND_WindowMessageCrack : public WindowMessageCrack
{
public:
	HWND  wndFocus(void) { return (HWND)(_WindowMessage.wParam); }
	short cmd     (void) { return GET_APPCOMMAND_LPARAM(_WindowMessage.lParam); }
	WORD  uDevice (void) { return GET_DEVICE_LPARAM    (_WindowMessage.lParam); }
	int   dwKeys  (void) { return GET_KEYSTATE_LPARAM  (_WindowMessage.lParam); }

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnNCXButtonDown(int fwButton, short nHittest, CPoint ptPos)
/*
WM_NCXBUTTONDOWN
		func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
class WM_NCXBUTTONDOWN_WindowMessageCrack : public WindowMessageCrack
{
public:
	int   fwButton(void) { return GET_XBUTTON_WPARAM  (_WindowMessage.wParam); }
	short nHittest(void) { return GET_NCHITTEST_WPARAM(_WindowMessage.wParam); }
	POINT ptPos(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return pt;
	}

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnNCXButtonUp(int fwButton, short nHittest, CPoint ptPos)
/*
WM_NCXBUTTONUP
		func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCXBUTTONUP_WindowMessageCrack = WM_NCXBUTTONDOWN_WindowMessageCrack;

//===========================================================================
// void OnNCXButtonDblClk(int fwButton, short nHittest, CPoint ptPos)
/*
WM_NCXBUTTONDBLCLK
		func(GET_XBUTTON_WPARAM(wParam), GET_NCHITTEST_WPARAM(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_NCXBUTTONDBLCLK_WindowMessageCrack = WM_NCXBUTTONDOWN_WindowMessageCrack;

//===========================================================================
// void OnXButtonDown(int fwButton, int dwKeys, CPoint ptPos)
/*
WM_XBUTTONDOWN
		func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_XBUTTONDOWN_WindowMessageCrack = WM_NCXBUTTONDOWN_WindowMessageCrack;

//===========================================================================
// void OnXButtonUp(int fwButton, int dwKeys, CPoint ptPos)
/*
WM_XBUTTONUP
		func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_XBUTTONUP_WindowMessageCrack = WM_NCXBUTTONDOWN_WindowMessageCrack;

//===========================================================================
// void OnXButtonDblClk(int fwButton, int dwKeys, CPoint ptPos)
/*
WM_XBUTTONDBLCLK
		func(GET_XBUTTON_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
		lResult = 0; \
*/
using WM_XBUTTONDBLCLK_WindowMessageCrack = WM_NCXBUTTONDOWN_WindowMessageCrack;

//===========================================================================
// void OnChangeUIState(WORD nAction, WORD nState)
/*
WM_CHANGEUISTATE
		func(LOWORD(wParam), HIWORD(wParam)); \
		lResult = 0; \
*/
class WM_CHANGEUISTATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	WORD nAction(void) { return LOWORD(_WindowMessage.wParam); }
	WORD nState (void) { return HIWORD(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnUpdateUIState(WORD nAction, WORD nState)
/*
WM_UPDATEUISTATE
		func(LOWORD(wParam), HIWORD(wParam)); \
		lResult = 0; \
*/
class WM_UPDATEUISTATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	WORD nAction(void) { return LOWORD(_WindowMessage.wParam); }
	WORD nState (void) { return HIWORD(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// LRESULT OnQueryUIState()
/*
WM_QUERYUISTATE
		lResult = func(); \
*/
class WM_QUERYUISTATE_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnInput(WPARAM RawInputCode, HRAWINPUT hRawInput)
/*
WM_INPUT
		func(GET_RAWINPUT_CODE_WPARAM(wParam), (HRAWINPUT)lParam); \
		lResult = 0; \
*/
class WM_INPUT_WindowMessageCrack : public WindowMessageCrack
{
public:
	WPARAM    RawInputCode(void) { return GET_RAWINPUT_CODE_WPARAM(_WindowMessage.wParam); }
	HRAWINPUT hRawInput   (void) { return (HRAWINPUT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnUniChar(TCHAR nChar, UINT nRepCnt, UINT nFlags)
/*
WM_UNICHAR
		func((TCHAR)wParam, (UINT)lParam & 0xFFFF, (UINT)((lParam & 0xFFFF0000) >> 16)); \
		if(IsMsgHandled()) \
		{ \
			lResult = (wParam == UNICODE_NOCHAR) ? TRUE : FALSE; \
			return TRUE; \
		} \
*/
class WM_UNICHAR_WindowMessageCrack : public WindowMessageCrack
{
public:
	TCHAR nChar  (void) { return (TCHAR) (_WindowMessage.wParam); }
	UINT  nRepCnt(void) { return (UINT) ((_WindowMessage.lParam) & 0xFFFF); }
	UINT  nFlags (void) { return (UINT)(((_WindowMessage.lParam) & 0xFFFF0000) >> 16); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (_WindowMessage.wParam == UNICODE_NOCHAR) ? TRUE : FALSE;
	}
};

//===========================================================================
// void OnWTSSessionChange(WPARAM nStatusCode, PWTSSESSION_NOTIFICATION nSessionID)
/*
WM_WTSSESSION_CHANGE
		func(wParam, (PWTSSESSION_NOTIFICATION)lParam); \
		lResult = 0; \
*/
class WM_WTSSESSION_CHANGE_WindowMessageCrack : public WindowMessageCrack
{
public:
	WPARAM                   nStatusCode(void) { return (_WindowMessage.wParam); }
	PWTSSESSION_NOTIFICATION nSessionID (void) { return (PWTSSESSION_NOTIFICATION)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnThemeChanged()
/*
WM_THEMECHANGED
		func(); \
		lResult = 0; \
*/
class WM_THEMECHANGED_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
/*
WM_MOUSEHWHEEL
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (short)HIWORD(wParam), _WTYPES_NS::CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))); \
*/
class WM_MOUSEHWHEEL_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nFlags(void)
	{
		return (UINT)LOWORD(_WindowMessage.wParam);
	}

	short zDelta(void)
	{
		return (short)HIWORD(_WindowMessage.wParam);
	}

	POINT pt(void)
	{
		POINT pt{ GET_X_LPARAM(_WindowMessage.lParam), GET_Y_LPARAM(_WindowMessage.lParam) };
		return pt;
	}

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};





/////////////////////////////////////////////////////////////////////////////
//
// ATL defined messages
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// BOOL OnForwardMsg(LPMSG Msg, DWORD nUserData)
/*
WM_FORWARDMSG
		lResult = (LRESULT)func((LPMSG)lParam, (DWORD)wParam); \
*/
class WM_FORWARDMSG_WindowMessageCrack : public WindowMessageCrack
{
public:
	LPMSG Msg      (void) { return (LPMSG)(_WindowMessage.lParam); }
	DWORD nUserData(void) { return (DWORD)(_WindowMessage.wParam); }

public:
	void Result(BOOL rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};





/////////////////////////////////////////////////////////////////////////////
//
// Dialog specific messages
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// LRESULT OnDMGetDefID()
/*
DM_GETDEFID
		lResult = func(); \
*/
class DM_GETDEFID_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnDMSetDefID(UINT DefID)
/*
DM_SETDEFID
		func((UINT)wParam); \
		lResult = TRUE; \
*/
class DM_SETDEFID_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT DefID(void) { return (UINT)(_WindowMessage.wParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnDMReposition()
/*
DM_REPOSITION
		func(); \
		lResult = 0; \
*/
class DM_REPOSITION_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};





/////////////////////////////////////////////////////////////////////////////
//
// Reflected messages
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// void OnReflectedCommand(UINT uNotifyCode, int nID, CWindow wndCtl)
/*
OCM_COMMAND
		func((UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
class OCM_COMMAND_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT uNotifyCode(void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	int  nID        (void) { return (int) LOWORD(_WindowMessage.wParam); }
	HWND wndCtl     (void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// LRESULT OnReflectedNotify(int idCtrl, LPNMHDR pnmh)
/*
OCM_NOTIFY
		lResult = func((int)wParam, (LPNMHDR)lParam); \
*/
class OCM_NOTIFY_WindowMessageCrack : public WindowMessageCrack
{
public:
	int     idCtrl(void) { return (int)    (_WindowMessage.wParam); }
	LPNMHDR pnmh  (void) { return (LPNMHDR)(_WindowMessage.lParam); }

public:
	void Result(LRESULT rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnReflectedParentNotify(UINT message, UINT nChildID, LPARAM lParam)
/*
OCM_PARENTNOTIFY
		func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), lParam); \
		lResult = 0; \
*/
class OCM_PARENTNOTIFY_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT   message (void) { return (UINT)LOWORD(_WindowMessage.wParam); }
	UINT   nChildID(void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	LPARAM lParam  (void) { return (_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnReflectedDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
/*
OCM_DRAWITEM
		func((UINT)wParam, (LPDRAWITEMSTRUCT)lParam); \
		lResult = TRUE; \
*/
class OCM_DRAWITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int              nIDCtl          (void) { return (UINT)(_WindowMessage.wParam); }
	LPDRAWITEMSTRUCT lpDrawItemStruct(void) { return (LPDRAWITEMSTRUCT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// void OnReflectedMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
/*
OCM_MEASUREITEM
		func((UINT)wParam, (LPMEASUREITEMSTRUCT)lParam); \
		lResult = TRUE; \
*/
class OCM_MEASUREITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int                 nIDCtl             (void) { return (UINT)(_WindowMessage.wParam); }
	LPMEASUREITEMSTRUCT lpMeasureItemStruct(void) { return (LPMEASUREITEMSTRUCT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// int OnReflectedCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct)
/*
OCM_COMPAREITEM
		lResult = (LRESULT)func((UINT)wParam, (LPCOMPAREITEMSTRUCT)lParam); \
*/
class OCM_COMPAREITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int                 nIDCtl             (void) { return (UINT)(_WindowMessage.wParam); }
	LPCOMPAREITEMSTRUCT lpCompareItemStruct(void) { return (LPCOMPAREITEMSTRUCT)(_WindowMessage.lParam); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnReflectedDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
/*
OCM_DELETEITEM
		func((UINT)wParam, (LPDELETEITEMSTRUCT)lParam); \
		lResult = TRUE; \
*/
class OCM_DELETEITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	int                nIDCtl            (void) { return (UINT)(_WindowMessage.wParam); }
	LPDELETEITEMSTRUCT lpDeleteItemStruct(void) { return (LPDELETEITEMSTRUCT)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)TRUE;
	}
};

//===========================================================================
// int OnReflectedVKeyToItem(UINT nKey, UINT nIndex, CListBox listBox)
/*
OCM_VKEYTOITEM
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
*/
class OCM_VKEYTOITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nKey   (void) { return (UINT)LOWORD(_WindowMessage.wParam); }
	UINT nIndex (void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	HWND listBox(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// int OnReflectedCharToItem(UINT nChar, UINT nIndex, CListBox listBox)
/*
OCM_CHARTOITEM
		lResult = (LRESULT)func((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HWND)lParam); \
*/
class OCM_CHARTOITEM_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nChar  (void) { return (UINT)LOWORD(_WindowMessage.wParam); }
	UINT nIndex (void) { return (UINT)HIWORD(_WindowMessage.wParam); }
	HWND listBox(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(int rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// void OnReflectedHScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
/*
OCM_HSCROLL
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
class OCM_HSCROLL_WindowMessageCrack : public WindowMessageCrack
{
public:
	UINT nSBCode     (void) { return (int)  LOWORD(_WindowMessage.wParam); }
	UINT nPos        (void) { return (short)HIWORD(_WindowMessage.wParam); }
	HWND wndScrollBar(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnReflectedVScroll(UINT nSBCode, UINT nPos, CScrollBar pScrollBar)
/*
OCM_VSCROLL
		func((int)LOWORD(wParam), (short)HIWORD(wParam), (HWND)lParam); \
		lResult = 0; \
*/
using OCM_VSCROLL_WindowMessageCrack = OCM_HSCROLL_WindowMessageCrack;

//===========================================================================
// HBRUSH OnReflectedCtlColorEdit(CDCHandle dc, CEdit edit)
/*
OCM_CTLCOLOREDIT
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
class OCM_CTLCOLOREDIT_WindowMessageCrack : public WindowMessageCrack
{
public:
	HDC  hdc(void) { return (HDC) (_WindowMessage.wParam); }
	HWND ctl(void) { return (HWND)(_WindowMessage.lParam); }

public:
	void Result(HBRUSH rv)
	{
		_WindowMessage.lResult = (LRESULT)rv;
	}
};

//===========================================================================
// HBRUSH OnReflectedCtlColorListBox(CDCHandle dc, CListBox listBox)
/*
OCM_CTLCOLORLISTBOX
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using OCM_CTLCOLORLISTBOX_WindowMessageCrack = OCM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnReflectedCtlColorBtn(CDCHandle dc, CButton button)
/*
OCM_CTLCOLORBTN
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using OCM_CTLCOLORBTN_WindowMessageCrack = OCM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnReflectedCtlColorDlg(CDCHandle dc, CWindow wnd)
/*
OCM_CTLCOLORDLG
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using OCM_CTLCOLORDLG_WindowMessageCrack = OCM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnReflectedCtlColorScrollBar(CDCHandle dc, CScrollBar scrollBar)
/*
OCM_CTLCOLORSCROLLBAR
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using OCM_CTLCOLORSCROLLBAR_WindowMessageCrack = OCM_CTLCOLOREDIT_WindowMessageCrack;

//===========================================================================
// HBRUSH OnReflectedCtlColorStatic(CDCHandle dc, CStatic wndStatic)
/*
OCM_CTLCOLORSTATIC
		lResult = (LRESULT)func((HDC)wParam, (HWND)lParam); \
*/
using OCM_CTLCOLORSTATIC_WindowMessageCrack = OCM_CTLCOLOREDIT_WindowMessageCrack;





/////////////////////////////////////////////////////////////////////////////
//
// Edit specific messages
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// void OnClear()
/*
WM_CLEAR
		func(); \
		lResult = 0; \
*/
class WM_CLEAR_WindowMessageCrack : public WindowMessageCrack
{
public:
	void Result(void)
	{
		_WindowMessage.lResult = (LRESULT)0;
	}
};

//===========================================================================
// void OnCopy()
/*
WM_COPY
		func(); \
		lResult = 0; \
*/
using WM_COPY_WindowMessageCrack = WM_CLEAR_WindowMessageCrack;

//===========================================================================
// void OnCut()
/*
WM_CUT
		func(); \
		lResult = 0; \
*/
using WM_CUT_WindowMessageCrack = WM_CLEAR_WindowMessageCrack;

//===========================================================================
// void OnPaste()
/*
WM_PASTE
		func(); \
		lResult = 0; \
*/
using WM_PASTE_WindowMessageCrack = WM_CLEAR_WindowMessageCrack;

//===========================================================================
// void OnUndo()
/*
WM_UNDO
		func(); \
		lResult = 0; \
*/
using WM_UNDO_WindowMessageCrack = WM_CLEAR_WindowMessageCrack;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




