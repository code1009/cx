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
	
	WebMessageQueue _MessageQueue;

public:
	explicit WebUIMessageService(WebUIManager* manager);

public:
	virtual ~WebUIMessageService();

public:
	WebUIManager* getManager(void);
	WebUIWindow* getWindow(void);
	void setWindow(HWND hWindow);

public:
	void onRuntimeExceptionThrown(WebUIWindow* window, const std::wstring& message);

public:
	void onWebMessage(WebUIWindow* window, const std::wstring& message);
	bool onCommand(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_Empty(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_PageNavigation(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_StringMessage(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_FileUpdate(WebUIWindow* window, web::json::value& jsonMessage);

	bool onCommand_Connect(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_Disconnect(WebUIWindow* window, web::json::value& jsonMessage);
	bool onCommand_Send(WebUIWindow* window, web::json::value& jsonMessage);

public:
	std::wstring getFile_Json(void);
	void postWebMessage_StringMessage(void);
	void postWebMessage_StringMessage(std::wstring command, std::wstring message);
	void postWebMessageQueue(void);
	void postWebMessage(std::wstring command, std::wstring message);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




