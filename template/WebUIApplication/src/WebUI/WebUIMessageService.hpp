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

public:
	explicit WebUIMessageService(WebUIManager* manager);
	virtual ~WebUIMessageService();

public:
	WebUIManager* getManager(void);
	WebUIWindow* getWindow(void);
	void setWindow(HWND hWindow);

public:
	void onRuntimeExceptionThrown(WebUIWindow* window, const std::wstring& message);

public:
	void onReceiveWebMessage(WebUIWindow* window, const std::wstring& message);
	bool onReceiveCommand(WebUIWindow* window, web::json::value& jsonMessage);
	bool onReceiveCommand_Empty(WebUIWindow* window, web::json::value& jsonMessage);
	bool onReceiveCommand_Navigate(WebUIWindow* window, web::json::value& jsonMessage);
	bool onReceiveCommand_Message(WebUIWindow* window, web::json::value& jsonMessage);
	bool onReceiveCommand_FileUpdate(WebUIWindow* window, web::json::value& jsonMessage);

public:
	std::wstring getFile_Json(void);
	void postWebMessage_Message(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




