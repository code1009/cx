#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIManager;
class WebUIWindow;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIMessageService
{
private:
	WebUIManager* _Manager{ nullptr };
	HWND _hWindow{ nullptr };
	
	WebMessageQueue _WebMessageTxQueue{};
	WebMessageQueue _WebMessageRxQueue{};

public:
	explicit WebUIMessageService(WebUIManager* manager);

public:
	virtual ~WebUIMessageService();


	//-----------------------------------------------------------------------
public:
	WebUIManager* getManager(void);
	WebUIWindow* getWindow(void);
	void setWindow(HWND hWindow);


	//-----------------------------------------------------------------------
public:
	void onRuntimeExceptionThrown(WebUIWindow* window, const std::wstring& message);

public:
	void onWebMessage(WebUIWindow* window, const std::wstring& message);


	//-----------------------------------------------------------------------
protected:
	void clearWebMessageQueue(void);
	void clearWebMessageTxQueue(void);
	void clearWebMessageRxQueue(void);


	//-----------------------------------------------------------------------
public:
	void processWebMessageQueue(void);

protected:
	void pullWebMessageQueue(void);
	void postWebMessageQueue(void);
	void pullWebMessage(WebMessage* webMessage);
	void postWebMessage(WebMessage* webMessage);

public:
	void writeWebMessage(std::wstring command, std::wstring message);


	//-----------------------------------------------------------------------
protected:
	bool onCommand(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_Empty(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_PageNavigation(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_StringMessage(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_FileUpdate(WebUIWindow* window, web::json::value& jsonMessage);

	bool onCommand_Connect(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_Disconnect(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_Send(WebUIWindow* window, web::json::value& jsonMessage);


	//-----------------------------------------------------------------------
protected:
	void postStringMessage(std::wstring command, std::wstring message);
	void postStringMessage(void);


	//-----------------------------------------------------------------------
public:
	std::wstring getFile_Json(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




