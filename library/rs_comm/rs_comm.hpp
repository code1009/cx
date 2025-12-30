#pragma once





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
class rs_comm
{
public:
	HANDLE _file_handle;

public:
	rs_comm(void);
	virtual ~rs_comm(void);

public:
	BOOL open  (LPCWSTR lpFileName=L"COM1:", DWORD dwFlagsAndAttributes=0);
	void close (void);

public:
	BOOL read  (LPVOID  lpBuffer, DWORD dwBufSize, LPDWORD lpReadSize   , LPOVERLAPPED lpOverlapped=NULL);
	BOOL write (LPCVOID lpBuffer, DWORD dwBufSize, LPDWORD lpWrittenSize, LPOVERLAPPED lpOverlapped=NULL);

public:
	// Win32 API Wrapping
	BOOL get_state       (LPDCB lpDCB);
	BOOL set_state       (LPDCB lpDCB);

	BOOL get_timeouts    (LPCOMMTIMEOUTS lpCommTimeouts);
	BOOL set_timeouts    (LPCOMMTIMEOUTS lpCommTimeouts);

	BOOL wait_event      (LPDWORD lpEvtMask, LPOVERLAPPED lpOverlapped=NULL);
	BOOL set_mask        (DWORD   dwEvtMask = EV_RXCHAR | EV_CTS | EV_DSR | EV_RING);
	BOOL get_mask        (LPDWORD lpEvtMask);

	BOOL clear_break     (void);
	BOOL clear_error     (LPDWORD lpErrors, LPCOMSTAT lpStat);
	BOOL escape_function (DWORD dwFunc);
	BOOL setup           (DWORD dwInQueue = 4096, DWORD dwOutQueue = 4096);
	BOOL purge           (DWORD dwFlags = PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	BOOL set_break       (void);
	BOOL transmit_char   (char cChar);

public:
	enum FLOW_CONTROL
	{
		FC_NO,
		FC_CTSRTS,
		FC_CTSDTR,
		FC_DSRRTS,
		FC_DSRDTR,
		FC_XONXOFF
	};

	BOOL set_timeouts (DWORD dwReadIntervalTimeout        = MAXDWORD ,
	                   DWORD dwReadTotalTimeoutMultiplier = MAXDWORD ,
	                   DWORD dwReadTotalTimeoutConstant   = 200      ,
	                   DWORD dwWriteTotalTimeoutMultiplier= 0        ,
	                   DWORD dwWriteTotalTimeoutConstant  = 0        );

	BOOL set_dcb (DWORD        dwBaudRate = CBR_9600   ,
	              BYTE         byByteSize = 8          ,
	              BYTE         byParity   = NOPARITY   ,
	              BYTE         byStopBits = ONESTOPBIT ,
	              FLOW_CONTROL fcFlowCtrl = FC_NO      );

	BOOL set_baudrate (DWORD dwBaudRate = CBR_9600   ,
	                   BYTE  byByteSize = 8          ,
	                   BYTE  byParity   = NOPARITY   ,
	                   BYTE  byStopBits = ONESTOPBIT );
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


