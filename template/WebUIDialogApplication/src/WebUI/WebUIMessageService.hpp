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

public:
	std::wstring getFile_Json(void);
	void postWebMessage_StringMessage(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




