#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIManager
{
	//-------------------------------------------------------------------------
private:
	HWND                                         _hMainWindow{ nullptr };
	WebUIContentsMap                             _ContentsMap{};
	std::shared_ptr<WebUIMessageService>         _MessageService{};
	std::map<HWND, std::shared_ptr<WebUIWindow>> _WindowMap{};


	//-------------------------------------------------------------------------
public:
	explicit WebUIManager(HWND hMainWindow);
	virtual ~WebUIManager();


	//-------------------------------------------------------------------------
public:
	WebUIManager(const WebUIManager& other) = delete;
	WebUIManager& operator=(const WebUIManager& other) = delete;

	WebUIManager(WebUIManager&& other) noexcept = delete;
	WebUIManager& operator=(WebUIManager&& other) noexcept = delete;


	//-------------------------------------------------------------------------
public:
	std::wstring getContentsDataFolder(void) const;
	std::wstring getContentsHost(void) const;
	std::wstring getContentsURI(const std::wstring& urn) const;
	std::wstring parseContentsURN(const std::wstring& uri) const;


	//-------------------------------------------------------------------------
public:
	void registerContentsMap(void);
	WebUIContentsMap* getContentsMap(void);

public:
	WebUIMessageService* getMessageService(void);

	//-------------------------------------------------------------------------
public:
	std::shared_ptr<WebUIWindow> newPopupWindow(HWND hParentWindow, std::wstring uri);
	std::shared_ptr<WebUIWindow> newPopupWindow(HWND hParentWindow, std::wstring uri, const RECT& rect);
	std::shared_ptr<WebUIWindow> newChildWindow(HWND hParentWindow, std::wstring uri);

	void onDestroyWindow(HWND hWindow);
	void deleteWindow(HWND hWindow);
	void deleteAndDestroyAllWindows(void);
	void moveWindow(HWND hParentWindow, const RECT& rect);

	WebUIWindow* findWindow(HWND hWindow);

	void postBroadcastWebMessageAsJson(const std::wstring& msg);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




