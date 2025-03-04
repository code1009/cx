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
class WebUIView
{
	//-------------------------------------------------------------------------
private:
	WebUIWindow* _Window{ nullptr };
	std::wstring _URI{ };


	//-------------------------------------------------------------------------
private:
	wil::com_ptr<ICoreWebView2Environment> _WebView_Environment;
	wil::com_ptr<ICoreWebView2Controller>  _WebView_Controller;
	wil::com_ptr<ICoreWebView2>            _WebView;


	//-------------------------------------------------------------------------
private:
	EventRegistrationToken _WebView_WebResourceRequested_Token{};
	EventRegistrationToken _WebView_WebMessageReceived_Token{};
	EventRegistrationToken _WebView_DocumentTitleChanged_Token{};
	EventRegistrationToken _WebView_HistoryChanged_Token{};
	EventRegistrationToken _WebView_SourceChanged_Token{};
	EventRegistrationToken _WebView_NavigationStarting_Token{};
	EventRegistrationToken _WebView_NavigationCompleted_Token{};
	EventRegistrationToken _WebView_NewWindowRequested_Token{};

	EventRegistrationToken _WebView_ContextMenuRequested_Token{};


private:
	wil::com_ptr<ICoreWebView2DevToolsProtocolEventReceiver> _WebView_DevToolsProtocol_Security_securityStateChanged_EventReceiver;
	EventRegistrationToken                                   _WebView_DevToolsProtocol_Security_securityStateChanged_Token{};
	wil::com_ptr<ICoreWebView2DevToolsProtocolEventReceiver> _WebView_DevToolsProtocol_Log_entryAdded_EventReceiver;
	EventRegistrationToken                                   _WebView_DevToolsProtocol_Log_entryAdded_Token{};
	wil::com_ptr<ICoreWebView2DevToolsProtocolEventReceiver> _WebView_DevToolsProtocol_Runtime_consoleAPICalled_EventReceiver;
	EventRegistrationToken                                   _WebView_DevToolsProtocol_Runtime_consoleAPICalled_Token{};
	wil::com_ptr<ICoreWebView2DevToolsProtocolEventReceiver> _WebView_DevToolsProtocol_Runtime_exceptionThrown_EventReceiver;
	EventRegistrationToken                                   _WebView_DevToolsProtocol_Runtime_exceptionThrown_Token{};


	//-------------------------------------------------------------------------
public:
	explicit WebUIView(WebUIWindow* window, std::wstring uri);

public:
	virtual ~WebUIView();


	//-------------------------------------------------------------------------
public:
	void resize(RECT& rect);


	//-------------------------------------------------------------------------
public:
	void     createWebView                      (void);
private:
	HRESULT      onWebView_Environment_Completed(HRESULT errorCode, ICoreWebView2Environment* createdEnvironment);

public:
	void    destroyWebView (void);


	//-------------------------------------------------------------------------
private:
	HRESULT createWebView_Controller         (void);
	HRESULT     onWebView_Controller_Completed(HRESULT errorCode, ICoreWebView2Controller* createdController);

	HRESULT resizeWebView_Controller(void);
	HRESULT  setupWebView_Controller(void);


	//-------------------------------------------------------------------------
private:
	HRESULT setupWebView_Settings(void);


	//-------------------------------------------------------------------------
private:
	HRESULT setupWebView(void);
	
private:
	HRESULT setupWebView_WebResourceRequested(void);
	HRESULT    onWebView_WebResourceRequested(ICoreWebView2* sender, ICoreWebView2WebResourceRequestedEventArgs* args);
	HRESULT setupWebView_WebMessageReceived  (void);
	HRESULT    onWebView_WebMessageReceived  (ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);
	HRESULT setupWebView_DocumentTitleChanged(void);
	HRESULT    onWebView_DocumentTitleChanged(ICoreWebView2* sender, IUnknown* args);
	HRESULT setupWebView_HistoryChanged      (void);
	HRESULT    onWebView_HistoryChanged      (ICoreWebView2* sender, IUnknown* args);
	HRESULT setupWebView_SourceChanged       (void);
	HRESULT    onWebView_SourceChanged       (ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args);
	HRESULT setupWebView_NavigationStarting  (void);
	HRESULT    onWebView_NavigationStarting  (ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args);
	HRESULT setupWebView_NavigationCompleted (void);
	HRESULT    onWebView_NavigationCompleted (ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args);
	HRESULT setupWebView_NewWindowRequested  (void);
	HRESULT    onWebView_NewWindowRequested  (ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args);

private:
	HRESULT setupWebView_ContextMenuRequested(void);
	HRESULT    onWebView_ContextMenuRequested(ICoreWebView2* sender, ICoreWebView2ContextMenuRequestedEventArgs* args);

private:
	HRESULT AddContextMenuItems(HMENU hPopupMenu, wil::com_ptr<ICoreWebView2ContextMenuItemCollection> items);
	void onExecuteTask(void* ptr);
	void runAsync(std::function<void()> callback);
public:
	void onUser1(cx::wui::WindowMessage& windowMessage);

private:
	HRESULT setupWebView_DevToolsProtocol_Security_securityStateChanged(void);
	HRESULT    onWebView_DevToolsProtocol_Security_securityStateChanged(ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args);
	HRESULT setupWebView_DevToolsProtocol_Log_entryAdded               (void);
	HRESULT    onWebView_DevToolsProtocol_Log_entryAdded               (ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args);
	HRESULT setupWebView_DevToolsProtocol_Runtime_consoleAPICalled     (void);
	HRESULT    onWebView_DevToolsProtocol_Runtime_consoleAPICalled     (ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args);
	HRESULT setupWebView_DevToolsProtocol_Runtime_exceptionThrown      (void);
	HRESULT    onWebView_DevToolsProtocol_Runtime_exceptionThrown      (ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args);


	//-------------------------------------------------------------------------
private:
	void onWebMessage(const std::wstring& urn, const std::wstring& webMessage);

public:
	void postWebMessageAsJson(const std::wstring& msg);


	//-------------------------------------------------------------------------
public:
	void navigate(const std::wstring& uri);
	void navigateContents(const std::wstring& urn);
	void executeScript(const std::wstring& script);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




