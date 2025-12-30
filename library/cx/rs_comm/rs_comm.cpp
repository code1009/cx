/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#if 0

//===========================================================================
#include <cinttypes>
#include <vector>

//===========================================================================
#include <stdio.h>
#include <assert.h>

//===========================================================================
#include <tchar.h>
#include <windows.h>

#endif

//===========================================================================
#include "rs_comm.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//  
// Class: rs_comm 
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rs_comm::rs_comm () 
	: _file_handle(INVALID_HANDLE_VALUE)
{
}

rs_comm::~rs_comm ()
{
	close ();
}

//===========================================================================
BOOL rs_comm::open (LPCWSTR lpFileName, DWORD dwFlagsAndAttributes)
{
	_file_handle = CreateFileW (
		lpFileName,
		GENERIC_READ | GENERIC_WRITE,
		0                           , //FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL                        ,
		OPEN_EXISTING               ,
		dwFlagsAndAttributes        ,
		NULL                        
	);

	if (_file_handle == INVALID_HANDLE_VALUE)
	{
		CX_RUNTIME_LOG(cxLError) << L"CreateFileW() failed : " << lpFileName;

		return FALSE;
	}

	CX_RUNTIME_LOG(cxLInfo) << L"CreateFileW() succeeds : " << lpFileName;

	return TRUE;
}

void rs_comm::close (void)
{
	if (_file_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle (_file_handle);

		_file_handle = INVALID_HANDLE_VALUE;
	}
}

//===========================================================================
BOOL rs_comm::read (LPVOID lpBuffer, DWORD dwBufSize, LPDWORD lpReadSize, LPOVERLAPPED lpOverlapped)
{
	CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL);

	BOOL bResult = ReadFile (_file_handle, lpBuffer, dwBufSize, lpReadSize, lpOverlapped);

	if (bResult==FALSE)
	{
		if (0==lpOverlapped)
		{
			CX_RUNTIME_LOG(cxLError) << L"ReadFile() failed : ";
		}
	}

	return bResult;
} 

BOOL rs_comm::write (LPCVOID lpBuffer, DWORD dwBufSize, LPDWORD lpWrittenSize, LPOVERLAPPED lpOverlapped)
{
	CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL);

	BOOL bResult = WriteFile (_file_handle, lpBuffer, dwBufSize, lpWrittenSize, lpOverlapped);

	if (bResult==FALSE)
	{
		if (0==lpOverlapped)
		{
			CX_RUNTIME_LOG(cxLError) << L"WriteFile() failed : ";
		}
	}

	return bResult;
}

//===========================================================================
/****************************************************************************
 # Win32 CommunicationPort API 
*****************************************************************************
BOOL GetCommConfig      (__in HANDLE hCommDev, __out_bcount_opt(*lpdwSize) LPCOMMCONFIG lpCC, __inout LPDWORD lpdwSize);
BOOL SetCommConfig      (__in HANDLE hCommDev, __in_bcount(dwSize)         LPCOMMCONFIG lpCC, __in    DWORD   dwSize  );

BOOL GetCommState       (__in HANDLE hFile, __out LPDCB lpDCB);
BOOL SetCommState       (__in HANDLE hFile, __in  LPDCB lpDCB);

BOOL GetCommTimeouts    (__in HANDLE hFile, __out LPCOMMTIMEOUTS lpCommTimeouts);
BOOL SetCommTimeouts    (__in HANDLE hFile, __in  LPCOMMTIMEOUTS lpCommTimeouts);

BOOL WaitCommEvent      (__in HANDLE hFile, __inout LPDWORD lpEvtMask, __inout_opt LPOVERLAPPED lpOverlapped);
BOOL SetCommMask        (__in HANDLE hFile, __in  DWORD   dwEvtMask);
BOOL GetCommMask        (__in HANDLE hFile, __out LPDWORD lpEvtMask);

BOOL ClearCommBreak     (__in HANDLE hFile);
BOOL ClearCommError     (__in HANDLE hFile, __out_opt LPDWORD lpErrors, __out_opt LPCOMSTAT lpStat);
BOOL EscapeCommFunction (__in HANDLE hFile, __in      DWORD dwFunc);
BOOL SetupComm          (__in HANDLE hFile, __in      DWORD dwInQueue, __in DWORD dwOutQueue);
BOOL PurgeComm          (__in HANDLE hFile, __in      DWORD dwFlags);
BOOL SetCommBreak       (__in HANDLE hFile);
BOOL TransmitCommChar   (__in HANDLE hFile, __in char cChar);

BOOL GetCommProperties  (__in HANDLE hFile, __inout   LPCOMMPROP lpCommProp);
BOOL GetCommModemStatus (__in HANDLE hFile, __out     LPDWORD lpModemStat);
*****************************************************************************/
BOOL rs_comm::get_state       (LPDCB lpDCB) { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::GetCommState (_file_handle, lpDCB); }
BOOL rs_comm::set_state       (LPDCB lpDCB) { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL);	return ::SetCommState (_file_handle, lpDCB); }

BOOL rs_comm::get_timeouts    (LPCOMMTIMEOUTS lpCommTimeouts) { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::GetCommTimeouts (_file_handle, lpCommTimeouts); }
BOOL rs_comm::set_timeouts    (LPCOMMTIMEOUTS lpCommTimeouts) { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::SetCommTimeouts (_file_handle, lpCommTimeouts); }

BOOL rs_comm::wait_event      (LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped) { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::WaitCommEvent(_file_handle, lpEvtMask, lpOverlapped); }
BOOL rs_comm::set_mask        (DWORD   dwEvtMask)                            { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::SetCommMask  (_file_handle, dwEvtMask); }
BOOL rs_comm::get_mask        (LPDWORD lpEvtMask)                            { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::GetCommMask  (_file_handle, lpEvtMask); }

BOOL rs_comm::clear_break     (void)                               { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::ClearCommBreak     (_file_handle);                        }
BOOL rs_comm::clear_error     (LPDWORD lpErrors, LPCOMSTAT lpStat) { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::ClearCommError     (_file_handle, lpErrors, lpStat);      }
BOOL rs_comm::escape_function (DWORD dwFunc)                       { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::EscapeCommFunction (_file_handle, dwFunc);                }
BOOL rs_comm::setup           (DWORD dwInQueue, DWORD dwOutQueue)  { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::SetupComm          (_file_handle, dwInQueue, dwOutQueue); }
BOOL rs_comm::purge           (DWORD dwFlags)                      { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::PurgeComm          (_file_handle, dwFlags);               }
BOOL rs_comm::set_break       (void)                               { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::SetCommBreak       (_file_handle);                        }
BOOL rs_comm::transmit_char   (char cChar)                         { CX_RUNTIME_ASSERT(_file_handle != INVALID_HANDLE_VALUE && _file_handle != NULL); return ::TransmitCommChar   (_file_handle, cChar);                 }

//===========================================================================
BOOL rs_comm::set_dcb 
(
	DWORD        dwBaudRate,
	BYTE         byByteSize,
	BYTE         byParity,  
	BYTE         byStopBits,
	FLOW_CONTROL fcFlowCtrl
)
{
	DCB dcb;

	ZeroMemory (&dcb, sizeof(DCB));

	// ----------------------------------------------------------------------
	dcb.DCBlength         = sizeof(DCB)        ;
	dcb.fBinary           = TRUE               ;                
	dcb.fOutxCtsFlow      = FALSE              ;               
	dcb.fOutxDsrFlow      = FALSE              ;               
	dcb.fDtrControl       = DTR_CONTROL_ENABLE ; 
	dcb.fDsrSensitivity   = FALSE              ;               
	dcb.fTXContinueOnXoff = FALSE              ;               
	dcb.fOutX             = FALSE              ;               
	dcb.fInX              = FALSE              ;               
	dcb.fErrorChar        = FALSE              ;               
	dcb.fNull             = FALSE              ;               
	dcb.fRtsControl       = RTS_CONTROL_ENABLE ; 
	dcb.fAbortOnError     = FALSE              ;               
	dcb.fDummy2           = FALSE              ;
	dcb.wReserved         = FALSE              ;
	dcb.XonLim            = 2048               ;
	dcb.XoffLim           = 512                ;
	dcb.XonChar           = 0                  ;
	dcb.XoffChar          = 0                  ;
	dcb.ErrorChar         = 0                  ;
	dcb.EofChar           = 0                  ;
	dcb.EvtChar           = 0                  ;
	dcb.wReserved1        = 0                  ;
	// ----------------------------------------------------------------------
	dcb.BaudRate          = dwBaudRate;          
	dcb.ByteSize          = byByteSize;          
	dcb.Parity            = byParity;            
	dcb.StopBits          = byStopBits;          
	// ----------------------------------------------------------------------

	switch (fcFlowCtrl)
	{
	case FC_NO:
		dcb.fOutxCtsFlow  = FALSE;
		dcb.fOutxDsrFlow  = FALSE;
		dcb.fOutX         = FALSE;
		dcb.fInX          = FALSE;
		break;

	case FC_CTSRTS:
		dcb.fOutxCtsFlow  = TRUE;
		dcb.fOutxDsrFlow  = FALSE;
		dcb.fOutX         = FALSE;
		dcb.fInX          = FALSE;
		dcb.fRtsControl   = RTS_CONTROL_HANDSHAKE;
		break;

	case FC_CTSDTR:
		dcb.fOutxCtsFlow  = TRUE;
		dcb.fOutxDsrFlow  = FALSE;
		dcb.fOutX         = FALSE;
		dcb.fInX          = FALSE;
		dcb.fDtrControl   = DTR_CONTROL_HANDSHAKE;
		break;

	case FC_DSRRTS:
		dcb.fOutxCtsFlow  = FALSE;
		dcb.fOutxDsrFlow  = TRUE;
		dcb.fOutX         = FALSE;
		dcb.fInX          = FALSE;
		dcb.fRtsControl   = RTS_CONTROL_HANDSHAKE;
		break;

	case FC_DSRDTR:
		dcb.fOutxCtsFlow  = FALSE;
		dcb.fOutxDsrFlow  = TRUE;
		dcb.fOutX         = FALSE;
		dcb.fInX          = FALSE;
		dcb.fDtrControl   = DTR_CONTROL_HANDSHAKE;
		break;

	case FC_XONXOFF:
		dcb.fOutxCtsFlow  = FALSE;
		dcb.fOutxDsrFlow  = FALSE;
		dcb.fOutX         = TRUE;
		dcb.fInX          = TRUE;
		dcb.XonChar       = 0x24;
		dcb.XoffChar      = 0x13;
		dcb.XoffLim       = 10000;
		dcb.XonLim        = 10000;
		break;

	default:
		dcb.fDtrControl = DTR_CONTROL_DISABLE ; 
		dcb.fRtsControl = DTR_CONTROL_DISABLE ; 
		dcb.XonLim      = 0;
		dcb.XoffLim     = 0;
		break;
	}

	return set_state (&dcb);		
}

BOOL rs_comm::set_baudrate
(
	DWORD dwBaudRate ,
	BYTE  byByteSize ,
	BYTE  byParity   , 
	BYTE  byStopBits 
)
{
	DCB dcb;


	ZeroMemory (&dcb, sizeof(DCB)) ;
	dcb.DCBlength = sizeof(DCB) ;

	if (FALSE==get_state (&dcb))
	{
		return FALSE;
	}

	dcb.BaudRate  = dwBaudRate;          
	dcb.ByteSize  = byByteSize;          
	dcb.Parity    = byParity;            
	dcb.StopBits  = byStopBits;          

	return set_state(&dcb);
}

BOOL rs_comm::set_timeouts
(
	DWORD dwReadIntervalTimeout        ,
    DWORD dwReadTotalTimeoutMultiplier ,
    DWORD dwReadTotalTimeoutConstant   ,
    DWORD dwWriteTotalTimeoutMultiplier,
    DWORD dwWriteTotalTimeoutConstant  
)
{
	COMMTIMEOUTS ct;


	ct.ReadIntervalTimeout         = dwReadIntervalTimeout;
	ct.ReadTotalTimeoutMultiplier  = dwReadTotalTimeoutMultiplier;
	ct.ReadTotalTimeoutConstant    = dwReadTotalTimeoutConstant;
	ct.WriteTotalTimeoutMultiplier = dwWriteTotalTimeoutMultiplier;
	ct.WriteTotalTimeoutConstant   = dwWriteTotalTimeoutConstant;

	return set_timeouts(&ct);
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



/****************************************************************************

# Tera-term 

DCB dcb;

dcb.DCBlength         = 28 ;
dcb.BaudRate          = baudrate ;
dcb.fBinary           = 1 ;
dcb.fParity           = 0 ;
dcb.fOutxCtsFlow      = 0 ;
dcb.fOutxDsrFlow      = 0 ;
dcb.fDtrControl       = 1 ;
dcb.fDsrSensitivity   = 0 ;
dcb.fTXContinueOnXoff = 0 ;
dcb.fOutX             = 0 ;
dcb.fInX              = 0 ;
dcb.fErrorChar        = 0 ;
dcb.fNull             = 0 ;
dcb.fRtsControl       = 1 ;
dcb.fAbortOnError     = 0 ;
dcb.fDummy2           = 0 ;
dcb.wReserved         = 0 ;
dcb.XonLim            = 2048; 
dcb.XoffLim           = 512 ; 
dcb.ByteSize          = 8  ;
dcb.Parity            = 0  ;
dcb.StopBits          = (BYTE)stopbit ;
dcb.XonChar           = 0 ;
dcb.XoffChar          = 0 ;
dcb.ErrorChar         = 0 ;
dcb.EofChar           = 0 ;
dcb.EvtChar           = 0 ;
dcb.wReserved1        = 0 ;


COMMTIMEOUTS ct;

ct.ReadIntervalTimeout         = -1 ;
ct.ReadTotalTimeoutMultiplier  = 0  ;
ct.ReadTotalTimeoutConstant    = 200;
ct.WriteTotalTimeoutMultiplier = 0  ;
ct.WriteTotalTimeoutConstant   = 0  ;

*****************************************************************************/


