/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wrl.h>

//===========================================================================
#include <wui/wui.hpp>

//===========================================================================
#include "WebUI.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//using namespace Microsoft::WRL;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIWindow::WebUIWindow(WebUIManager* manager, std::wstring uri, HWND hParentWindow, bool popupWindowStyle):
	_Manager{ manager },
	_URI{ uri },
	_PopupWindowStyle{ popupWindowStyle }
{
	//-----------------------------------------------------------------------
	cx::wui::WindowClass windowClass;


	windowClass.registerWindowClass(
		View_WindowClassName
	);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	RECT rect;
	std::wstring windowText;
	DWORD style;
	DWORD styleEx;
	HWND hwnd;


	//-----------------------------------------------------------------------
	if (hParentWindow)
	{
		GetClientRect(hParentWindow, &rect);
	}
	else
	{
		rect.left = 0;
		rect.top = 0;
		rect.right = 800;
		rect.bottom = 600;
	}


	//-----------------------------------------------------------------------
	windowText = L"WebUIWindow";


	//-----------------------------------------------------------------------
	if (popupWindowStyle)
	{
		style = wui::FrameWindowStyle;
		styleEx = wui::FrameWindowStyleEx;
	}
	else
	{
		style = WS_CHILD | WS_VISIBLE; // | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		styleEx = 0; // WS_EX_CLIENTEDGE;
	}


	//-----------------------------------------------------------------------
	hwnd = createWebUIWindow(
		hParentWindow,
		rect,
		windowText.c_str(),
		static_cast<DWORD>(style),
		static_cast<DWORD>(styleEx)
	);
	if (!hwnd)
	{
		throw std::wstring(L"WebUIWindow::WebView(): createWindow() failed");
	}


	//-----------------------------------------------------------------------
	WUI_TRACE(L"createWebView-begin");
	createWebView();
	WUI_TRACE(L"createWebView-end");


	//-----------------------------------------------------------------------
	::ShowWindow(getHandle(), SW_SHOW);
	::UpdateWindow(getHandle());
}

WebUIWindow::~WebUIWindow()
{
#ifdef _DEBUG
	//-------------------------------------------------------------------------
	std::wostringstream oss;


	oss << L"getHandle() = ";
	oss << getHandle();


	WUI_TRACE(oss.str().c_str());
#endif
}

void WebUIWindow::registerWindowMessageHandler(void)
{
	getWindowMessageHandler(WM_CREATE) = [this](wui::WindowMessage& windowMessage) { onCreate(windowMessage); };
	getWindowMessageHandler(WM_DESTROY) = [this](wui::WindowMessage& windowMessage) { onDestory(windowMessage); };
	getWindowMessageHandler(WM_CLOSE) = [this](wui::WindowMessage& windowMessage) { onClose(windowMessage); };
	getWindowMessageHandler(WM_NCDESTROY) = [this](wui::WindowMessage& windowMessage) { onNcDestory(windowMessage); };

	getWindowMessageHandler(WM_SIZE) = [this](wui::WindowMessage& windowMessage) { onSize(windowMessage); };
	getWindowMessageHandler(WM_DPICHANGED) = [this](wui::WindowMessage& windowMessage) { onDPIChanged(windowMessage); };
	getWindowMessageHandler(WM_COMMAND) = [this](wui::WindowMessage& windowMessage) { onCommand(windowMessage); };

	getWindowMessageHandler(WM_USER+1) = [this](wui::WindowMessage& windowMessage) { onUser1(windowMessage); };	
}

//===========================================================================
void WebUIWindow::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE    ) = &WebUIWindow::onCreate;
	_WindowMessageMap.handle(WM_DESTROY   ) = &WebUIWindow::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE     ) = &WebUIWindow::onClose;
	_WindowMessageMap.handle(WM_SIZE      ) = &WebUIWindow::onSize;
	_WindowMessageMap.handle(WM_COMMAND   ) = &WebUIWindow::onCommand;
}

void WebUIWindow::onCreate(wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	//SetWindowTextW(windowMessage.hWnd, L"WebUIWindow");


	defaultWindowMessageHandler(windowMessage);
}

void WebUIWindow::onDestroy(wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	WUI_TRACE(L"begin");


	//-----------------------------------------------------------------------
	WUI_TRACE(L"destroyWebView-begin");
	destroyWebView();
	WUI_TRACE(L"destroyWebView-end");


	//-----------------------------------------------------------------------
	_Manager->onDestroyWindow(getHandle());


	//-----------------------------------------------------------------------
	WUI_TRACE(L"end");
}

void WebUIWindow::onClose(wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	WUI_TRACE(L"begin");
	
	
	//-----------------------------------------------------------------------
	destroyWindow();


	//-----------------------------------------------------------------------
	WUI_TRACE(L"end");
}

void WebUIWindow::onSize(wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	wui::WM_SIZE_WindowMessageManipulator windowMessageManipulator(&windowMessage);


	//-----------------------------------------------------------------------
	RECT rect;


	if (_WebView_Controller != nullptr)
	{
		::GetClientRect(getHandle(), &rect);

		_WebView_Controller->put_Bounds(rect);
	}
}

void WebUIWindow::onDPIChanged(wui::WindowMessage& windowMessage)
{
	RECT* pWindowRect = reinterpret_cast<RECT*>(windowMessage.lParam);


	wui::setWindowPos(getHandle(), HWND_TOP, *pWindowRect, SWP_SHOWWINDOW);
}

void WebUIWindow::onCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageManipulator windowMessageManipulator(&windowMessage);


/*
	switch (windowMessageManipulator.nID())
	{
	case IDM_TEST1:
		onTest1(windowMessage);
		return;
		break;

	case IDM_TEST2:
		onTest2(windowMessage);
		return;
		break;

	default:
		break;
	}
*/

	defaultWindowMessageHandler(windowMessage);
}

//===========================================================================
int WebUIWindow::getDPIAwareBound(int bound) const
{
	constexpr int DEFAULT_DPI = 96;

	return (bound * GetDpiForWindow(getHandle()) / DEFAULT_DPI);
}

bool WebUIWindow::isPopupWindow(void)
{
	return _PopupWindowStyle;
}

//===========================================================================
#pragma region webview2

//===========================================================================
void WebUIWindow::createWebView(void)
{	
	//-----------------------------------------------------------------------
	HRESULT hr;


	hr = CreateCoreWebView2EnvironmentWithOptions(
		nullptr, 
		_Manager->getContentsDataFolder().c_str(),
		nullptr,
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>
			(this, &WebUIWindow::onWebView_Environment_Completed).Get()
	);
}

HRESULT WebUIWindow::onWebView_Environment_Completed(HRESULT errorCode, ICoreWebView2Environment* createdEnvironment)
{
	//-----------------------------------------------------------------------
	RETURN_IF_FAILED(errorCode);


	//-----------------------------------------------------------------------
	_WebView_Environment = createdEnvironment;


	//-----------------------------------------------------------------------
	HRESULT hr;


	hr = createWebView_Controller();

	return hr;
}

void WebUIWindow::destroyWebView(void)
{
	//_WebView->Release(); // DO NOT CALL!!!
	//_WebView_Controller->Release(); // DO NOT CALL!!!
	if (_WebView_Environment)
	{
		_WebView_Environment->Release();
	}

	_WebView.reset();
	_WebView_Controller.reset();
	_WebView_Environment.reset();
}

//===========================================================================
HRESULT WebUIWindow::createWebView_Controller(void)
{
	HRESULT hr;


	hr = _WebView_Environment->CreateCoreWebView2Controller(
		getHandle(),
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>
			(this, &WebUIWindow::onWebView_Controller_Completed).Get()
	);

	return hr;
}

HRESULT WebUIWindow::onWebView_Controller_Completed(HRESULT errorCode, ICoreWebView2Controller* createdController)
{
	RETURN_IF_FAILED(errorCode);


	if (createdController != nullptr)
	{
		//-----------------------------------------------------------------------
		HRESULT hr;


		//-----------------------------------------------------------------------
		_WebView_Controller = createdController;


		//-----------------------------------------------------------------------
		hr = setupWebView_Controller();
		RETURN_IF_FAILED(hr);


		//-----------------------------------------------------------------------
		hr = _WebView_Controller->get_CoreWebView2(&_WebView);
		RETURN_IF_FAILED(hr);

		hr = setupWebView_Settings();
		RETURN_IF_FAILED(hr);

		hr = setupWebView();
		RETURN_IF_FAILED(hr);


		//-----------------------------------------------------------------------
		hr = resizeWebView_Controller();
		RETURN_IF_FAILED(hr);
	}

	return S_OK;
}

HRESULT WebUIWindow::setupWebView_Controller(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2Controller4> controller4 = 
		_WebView_Controller.try_query<ICoreWebView2Controller4>();


	if (controller4)
	{
		hr = controller4->put_AllowExternalDrop(FALSE);
		RETURN_IF_FAILED(hr);
	}


	return S_OK;
}

HRESULT WebUIWindow::resizeWebView_Controller(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	RECT rect;


	::GetClientRect(getHandle(), &rect);


	hr = _WebView_Controller->put_Bounds(rect);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

//===========================================================================
HRESULT WebUIWindow::setupWebView_Settings(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2Settings>  _WebView_Settings;
	wil::com_ptr<ICoreWebView2Settings2> settings2;
	wil::com_ptr<ICoreWebView2Settings3> settings3;
	wil::com_ptr<ICoreWebView2Settings4> settings4;
	wil::com_ptr<ICoreWebView2Settings5> settings5;
	wil::com_ptr<ICoreWebView2Settings6> settings6;
	wil::com_ptr<ICoreWebView2Settings7> settings7;
	wil::com_ptr<ICoreWebView2Settings8> settings8;


	//-----------------------------------------------------------------------
	hr = _WebView->get_Settings(&_WebView_Settings);
	RETURN_IF_FAILED(hr);

	settings2 = _WebView_Settings.try_query<ICoreWebView2Settings2>();
	settings3 = _WebView_Settings.try_query<ICoreWebView2Settings3>();
	settings4 = _WebView_Settings.try_query<ICoreWebView2Settings4>();
	settings5 = _WebView_Settings.try_query<ICoreWebView2Settings5>();
	settings6 = _WebView_Settings.try_query<ICoreWebView2Settings6>();
	settings7 = _WebView_Settings.try_query<ICoreWebView2Settings7>();
	settings8 = _WebView_Settings.try_query<ICoreWebView2Settings8>();


	//-----------------------------------------------------------------------
	if (settings2)
	{
		//settings2->put_IsZoomControlEnabled(FALSE);
		//settings2->put_AreDefaultContextMenusEnabled(FALSE);
	}

	if (settings3)
	{
		settings3->put_AreBrowserAcceleratorKeysEnabled(FALSE);
	}


	//-----------------------------------------------------------------------
#if 0
	std::wstring script;


	script = L"window.addEventListener(\"contextmenu\", window => {window.preventDefault();});";
	hr = _WebView->AddScriptToExecuteOnDocumentCreated(script.c_str(), nullptr);
	RETURN_IF_FAILED(hr);
#endif


	return S_OK;
}

//===========================================================================
HRESULT WebUIWindow::setupWebView(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = setupWebView_WebResourceRequested();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_WebMessageReceived();
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = setupWebView_DocumentTitleChanged();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_HistoryChanged();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_SourceChanged();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_NavigationStarting();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_NavigationCompleted();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_NewWindowRequested();
	RETURN_IF_FAILED(hr);

	//-----------------------------------------------------------------------
	hr = setupWebView_ContextMenuRequested();
	RETURN_IF_FAILED(hr);

	//-----------------------------------------------------------------------
	hr = setupWebView_DevToolsProtocol_Security_securityStateChanged();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_DevToolsProtocol_Log_entryAdded();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_DevToolsProtocol_Runtime_consoleAPICalled();
	RETURN_IF_FAILED(hr);

	hr = setupWebView_DevToolsProtocol_Runtime_exceptionThrown();
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	navigate(_URI);


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_WebResourceRequested(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	std::wstring uri;


	uri = _Manager->getContentsHost() + L"/*";
	hr = _WebView->AddWebResourceRequestedFilter(uri.c_str(), COREWEBVIEW2_WEB_RESOURCE_CONTEXT_ALL);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView->add_WebResourceRequested(
		Microsoft::WRL::Callback<ICoreWebView2WebResourceRequestedEventHandler>
			(this, &WebUIWindow::onWebView_WebResourceRequested).Get(),
		&_WebView_WebResourceRequested_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_WebResourceRequested(ICoreWebView2* sender,ICoreWebView2WebResourceRequestedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	COREWEBVIEW2_WEB_RESOURCE_CONTEXT resourceContext;


	hr = args->get_ResourceContext(&resourceContext);
	RETURN_IF_FAILED(hr);


	/*
	if (resourceContext != COREWEBVIEW2_WEB_RESOURCE_CONTEXT_IMAGE)
	{
		//return E_INVALIDARG;
	}
	*/


	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2WebResourceRequest> webResourceRequest;


	hr = args->get_Request(&webResourceRequest);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsUri;


	hr = webResourceRequest->get_Uri(&ucsUri);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring uri(ucsUri.get());


	uri = web::uri::decode(uri);


	//-----------------------------------------------------------------------
	std::wstring urn = _Manager->parseContentsURN(uri);


	WUI_TRACE(urn);


	//-----------------------------------------------------------------------
	WebUIContents* o = _Manager->getContentsMap()->findContents(urn);


	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2WebResourceResponse> response;
	wil::com_ptr<ICoreWebView2Environment> environment;
	wil::com_ptr<ICoreWebView2_2> webview;


	hr = _WebView->QueryInterface(IID_PPV_ARGS(&webview));
	RETURN_IF_FAILED(hr);

	hr = webview->get_Environment(&environment);
	RETURN_IF_FAILED(hr);


	if (nullptr == o)
	{
		hr = environment->CreateWebResourceResponse(
			nullptr,
			404, L"Not found",
			nullptr,
			&response
		);
		RETURN_IF_FAILED(hr);
	}
	else
	{
		hr = environment->CreateWebResourceResponse(
			o->getStream(),
			200, L"OK",
			o->getHeaders().c_str(),
			&response
		);
		RETURN_IF_FAILED(hr);
	}


	hr = args->put_Response(response.get());
	RETURN_IF_FAILED(hr);

	return S_OK;
}

HRESULT WebUIWindow::setupWebView_WebMessageReceived(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_WebMessageReceived(
		Microsoft::WRL::Callback<ICoreWebView2WebMessageReceivedEventHandler>
			(this, &WebUIWindow::onWebView_WebMessageReceived).Get(),
		&_WebView_WebMessageReceived_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_WebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsUri;


	hr = args->get_Source(&ucsUri);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring uri(ucsUri.get());
	std::wstring urn = _Manager->parseContentsURN(uri);


	if (urn.empty())
	{
		return S_OK;
	}


#if 1
	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsWebMessage;


	hr = args->TryGetWebMessageAsString(&ucsWebMessage);
	RETURN_IF_FAILED(hr);
#else


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsWebMessage;


	hr = args->get_WebMessageAsJson(&ucsWebMessage);
	RETURN_IF_FAILED(hr);
#endif


	//-----------------------------------------------------------------------
	std::wstring webMessage(ucsWebMessage.get());


	//-----------------------------------------------------------------------
	onWebMessage(urn, webMessage);


	return S_OK;
}


//===========================================================================
HRESULT WebUIWindow::setupWebView_DocumentTitleChanged(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_DocumentTitleChanged(
		Microsoft::WRL::Callback<ICoreWebView2DocumentTitleChangedEventHandler>
		(this, &WebUIWindow::onWebView_DocumentTitleChanged).Get(),
		&_WebView_DocumentTitleChanged_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_DocumentTitleChanged(ICoreWebView2* sender, IUnknown* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsTitle;


	hr = sender->get_DocumentTitle(&ucsTitle);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring title(ucsTitle.get());


	WUI_TRACE(title);


	//-----------------------------------------------------------------------
	if (isPopupWindow())
	{
		wui::setWindowText(*this, title);
	}
	else
	{
		HWND hParent;

		hParent = GetParent(*this);
		wui::setWindowText(hParent, title);
	}


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_HistoryChanged(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_HistoryChanged(
		Microsoft::WRL::Callback<ICoreWebView2HistoryChangedEventHandler>
			(this, &WebUIWindow::onWebView_HistoryChanged).Get(),
		&_WebView_HistoryChanged_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_HistoryChanged(ICoreWebView2* sender, IUnknown* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsSource;


	hr = sender->get_Source(&ucsSource);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring source(ucsSource.get());


	WUI_TRACE(source);


	//-----------------------------------------------------------------------

	return S_OK;
}

HRESULT WebUIWindow::setupWebView_SourceChanged(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_SourceChanged(
		Microsoft::WRL::Callback<ICoreWebView2SourceChangedEventHandler>
			(this, &WebUIWindow::onWebView_SourceChanged).Get(),
		&_WebView_SourceChanged_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_SourceChanged(ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsSource;


	hr = sender->get_Source(&ucsSource);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring source(ucsSource.get());
	WUI_TRACE(source);


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_NavigationStarting(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_NavigationStarting(
		Microsoft::WRL::Callback<ICoreWebView2NavigationStartingEventHandler>
			(this, &WebUIWindow::onWebView_NavigationStarting).Get(),
		&_WebView_NavigationStarting_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_NavigationStarting(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsUri;


	hr = args->get_Uri(&ucsUri);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring uri(ucsUri.get());
	WUI_TRACE(uri);


	//-----------------------------------------------------------------------
	BOOL cancel = FALSE;


	hr = args->get_Cancel(&cancel);
	RETURN_IF_FAILED(hr);


	//hr = args->put_Cancel(cancel);
	//RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_NavigationCompleted(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_NavigationCompleted(
		Microsoft::WRL::Callback<ICoreWebView2NavigationCompletedEventHandler>
			(this, &WebUIWindow::onWebView_NavigationCompleted).Get(),
		&_WebView_NavigationCompleted_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_NavigationCompleted(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	BOOL navigationSucceeded = FALSE;


	hr = args->get_IsSuccess(&navigationSucceeded);
	if (SUCCEEDED(hr))
	{
		WUI_TRACE(L"navigationSucceeded");


		//executeScript(L"alert(\"navigationSucceeded\"); var win = window.open(\"/page1/list0.json\", \"PopupWin\", \"width=500,height=600\");");
	}
	else
	{
		WUI_TRACE(L"navigationFailed");
	}


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_NewWindowRequested(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->add_NewWindowRequested(
		Microsoft::WRL::Callback<ICoreWebView2NewWindowRequestedEventHandler>
			(this, &WebUIWindow::onWebView_NewWindowRequested).Get(),
		&_WebView_NewWindowRequested_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_NewWindowRequested(ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2WindowFeatures> windowFeatures;


	hr = args->get_WindowFeatures(&windowFeatures);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	BOOL hasPosition = FALSE;
	BOOL hasSize = FALSE;
	UINT32 left = 0;
	UINT32 top = 0;
	UINT32 height = 0;
	UINT32 width = 0;
	BOOL shouldHaveToolbar = true;


	hr = windowFeatures->get_HasPosition(&hasPosition);
	RETURN_IF_FAILED(hr);

	hr = windowFeatures->get_HasSize(&hasSize);
	RETURN_IF_FAILED(hr);

	if (hasPosition)
	{
		hr = windowFeatures->get_Left(&left);
		RETURN_IF_FAILED(hr);

		hr = windowFeatures->get_Top(&top);
		RETURN_IF_FAILED(hr);
	}
	else
	{
		left = 0;
		top = 0;
	}

	static constexpr int s_minNewWindowSize = 100;
	if (hasSize)
	{
		hr = windowFeatures->get_Height(&height);
		RETURN_IF_FAILED(hr);
		hr = windowFeatures->get_Width(&width);
		RETURN_IF_FAILED(hr);
	}
	else
	{
		width = s_minNewWindowSize;
		height = s_minNewWindowSize;
	}

	hr = windowFeatures->get_ShouldDisplayToolbar(&shouldHaveToolbar);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	RECT frameRect;


	GetClientRect(getHandle(), &frameRect);
	ClientToScreen(getHandle(), reinterpret_cast<POINT*>(&frameRect.left));


	//-----------------------------------------------------------------------
	RECT windowRect = { 0 };


	windowRect.left   = frameRect.left + left;
	windowRect.top    = frameRect.top  + top;

	windowRect.right  = frameRect.left + left + width;
	windowRect.bottom = frameRect.top  + top  + height;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsUri;


	hr = args->get_Uri(&ucsUri);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::shared_ptr<WebUIWindow> newWindow = _Manager->newPopupWindow(getHandle(), ucsUri.get(), windowRect);


	//-----------------------------------------------------------------------
	args->put_NewWindow(newWindow->_WebView.get());

	args->put_Handled(TRUE);


#if 0
	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2> senderWebView;
	wil::com_ptr<ICoreWebView2_2> webView;
	wil::com_ptr<ICoreWebView2Environment> environment;
	wil::com_ptr<ICoreWebView2> newWebView;


	senderWebView = sender;
	senderWebView->QueryInterface(IID_PPV_ARGS(&webView));


	hr = webView->get_Environment(&environment);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = environment->CreateCoreWebView2Controller(
		getHandle(), // 새 창을 호스팅할 부모 윈도우 핸들
		Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
			[this](HRESULT errorCode, ICoreWebView2Controller* createdController) -> HRESULT
			{
				if (createdController != nullptr)
				{
					wil::com_ptr<ICoreWebView2> newWebView;


					createdController->get_CoreWebView2(&newWebView);

					// 새 창에 대한 설정을 여기에서 수행할 수 있습니다.
					// 예: newWebView->Navigate(L"https://example.com");

					// 새 창의 WebView2 인스턴스를 이벤트 인자에 설정
					args->put_NewWindow(newWebView.get());

					// 새 창 생성을 처리했음을 알림
					args->put_Handled(TRUE);
				}
				return S_OK;
			}
		).Get()
	);
	RETURN_IF_FAILED(hr);
#endif


	return S_OK;
}

//===========================================================================
// https://github.com/MicrosoftEdge/WebView2Samples/blob/d1cf80d1bbd533f4dfe078a05638c3211e2dce75/SampleApps/WebView2APISample/SettingsComponent.cpp#L592
// https://github.com/MicrosoftEdge/WebView2Samples/blob/d1cf80d1bbd533f4dfe078a05638c3211e2dce75/SampleApps/WebView2APISample/AppWindow.cpp
// https://learn.microsoft.com/en-us/microsoft-edge/webview2/how-to/context-menus?tabs=cpp
HRESULT WebUIWindow::setupWebView_ContextMenuRequested(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	auto _WebView2_4 = _WebView.try_query<ICoreWebView2_11>();


	//-----------------------------------------------------------------------
	hr = _WebView2_4->add_ContextMenuRequested(
		Microsoft::WRL::Callback<ICoreWebView2ContextMenuRequestedEventHandler>
		(this, &WebUIWindow::onWebView_ContextMenuRequested).Get(),
		&_WebView_ContextMenuRequested_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_ContextMenuRequested(ICoreWebView2* sender, ICoreWebView2ContextMenuRequestedEventArgs* args)
{
	auto showMenu = [this, args = wil::com_ptr<ICoreWebView2ContextMenuRequestedEventArgs>(args)]() -> HRESULT
	{
		HRESULT hr;
		wil::com_ptr<ICoreWebView2ContextMenuItemCollection> items;


		hr = args->get_MenuItems(&items);
		RETURN_IF_FAILED(hr);


		hr = args->put_Handled(true);
		RETURN_IF_FAILED(hr);


		//-------------------------------------------------------------------
		HMENU hPopupMenu = CreatePopupMenu();


		hr = AddContextMenuItems(hPopupMenu, items);
		RETURN_IF_FAILED(hr);


		//-------------------------------------------------------------------
		POINT locationInControlCoordinates;


		hr = args->get_Location(&locationInControlCoordinates);
		RETURN_IF_FAILED(hr);


		//-------------------------------------------------------------------
		HWND hWnd;
		POINT locationInScreenCoordinates;


		_WebView_Controller->get_ParentWindow(&hWnd);
		SetForegroundWindow(hWnd);


		//-------------------------------------------------------------------
		RECT rect;
		POINT topLeft;


		GetClientRect(hWnd, &rect);
		topLeft.x = rect.left;
		topLeft.y = rect.top;
		ClientToScreen(hWnd, &topLeft);
		
		
		//-------------------------------------------------------------------
		RECT bounds;


		hr = _WebView_Controller->get_Bounds(&bounds);
		RETURN_IF_FAILED(hr);


		//-------------------------------------------------------------------
		auto _WebView_Controller3 = _WebView_Controller.try_query<ICoreWebView2Controller3>();
		if (_WebView_Controller3 == nullptr)
		{
			return E_FAIL;
		}


		//-------------------------------------------------------------------
		double scale;


		_WebView_Controller3->get_RasterizationScale(&scale);


		//-------------------------------------------------------------------
		locationInScreenCoordinates.x = bounds.left + topLeft.x + ((int)(locationInControlCoordinates.x * scale));
		locationInScreenCoordinates.y = bounds.top  + topLeft.y + ((int)(locationInControlCoordinates.y * scale));


		UINT32 selectedCommandId = TrackPopupMenu(
			hPopupMenu,
			TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD,
			locationInScreenCoordinates.x,
			locationInScreenCoordinates.y,
			0,
			hWnd,
			NULL);

		if (selectedCommandId != 0) 
		{
			hr = args->put_SelectedCommandId(selectedCommandId);
			RETURN_IF_FAILED(hr);
		}

		return S_OK;
	};


	//-----------------------------------------------------------------------
	wil::com_ptr<ICoreWebView2Deferral> deferral;
	HRESULT hr;
	

	hr = args->GetDeferral(&deferral);
	RETURN_IF_FAILED(hr);


	runAsync(
		[deferral, showMenu]() 
		{
			HRESULT hr;


			hr = showMenu();
			if (FAILED(hr))
			{
				
			}

			hr = deferral->Complete();
			if (FAILED(hr))
			{
				return;
			}
		}
	);

	return S_OK;
}

HRESULT WebUIWindow::AddContextMenuItems(HMENU hPopupMenu, wil::com_ptr<ICoreWebView2ContextMenuItemCollection> items)
{
	HRESULT hr;
	wil::com_ptr<ICoreWebView2ContextMenuItem> current;
	UINT32 itemsCount;


	hr = items->get_Count(&itemsCount);
	RETURN_IF_FAILED(hr);


	for (UINT32 i = 0; i < itemsCount; i++)
	{
		hr = items->GetValueAtIndex(i, &current);
		RETURN_IF_FAILED(hr);


		//-----------------------------------------------------------------------
		COREWEBVIEW2_CONTEXT_MENU_ITEM_KIND kind;


		hr = current->get_Kind(&kind);
		RETURN_IF_FAILED(hr);


		//-----------------------------------------------------------------------
		wil::unique_cotaskmem_string label;


		hr = current->get_Label(&label);
		RETURN_IF_FAILED(hr);

		std::wstring labelString = label.get();


		//-----------------------------------------------------------------------
#if 0
		wil::unique_cotaskmem_string shortcut;


		hr = current->get_ShortcutKeyDescription(&shortcut);
		RETURN_IF_FAILED(hr);


		std::wstring shortcutString = shortcut.get();
		if (!shortcutString.empty())
		{
			// L"\t" will right align the shortcut string
			labelString = labelString + L"\t" + shortcutString;
		}
#endif


		//-----------------------------------------------------------------------
		BOOL isEnabled;
		hr = current->get_IsEnabled(&isEnabled);
		RETURN_IF_FAILED(hr);


		BOOL isChecked;
		hr = current->get_IsChecked(&isChecked);
		RETURN_IF_FAILED(hr);


		INT32 commandId;
		hr = current->get_CommandId(&commandId);
		RETURN_IF_FAILED(hr);


		//-----------------------------------------------------------------------
		bool skip;


		skip = false;
		switch (commandId)
		{
		//case -1   : break; /* SEPARATOR                                      */ 
		//case 33000: break; /* "뒤로(&B)                \t Alt+왼쪽 화살표"   */ 
		//case 33001: break; /* "앞으로(&F)              \t Alt+오른쪽 화살표" */ 
		//case 33002: break; /* "새로 고침(&R)           \t Ctrl+R"            */ 
		//case 35004: break; /* "다른 이름으로 저장(&A)"                       */ 
		  case 35003: break; /* "인쇄(&P)                \t Ctrl+P"            */ 
		//case 50460: break; /* "공유(&S)"                                     */ 
		  case 52551: break; /* "&스크린샷               \t Ctrl+Shift+S"      */ 
		//case 50162: break; /* "검사(&N)"                                     */ 
		default:
			skip = true;
			break;
		}


		if (skip)
		{
			continue;
		}


		//-----------------------------------------------------------------------
		if (kind == COREWEBVIEW2_CONTEXT_MENU_ITEM_KIND_SEPARATOR)
		{
			AppendMenu(hPopupMenu, MF_SEPARATOR, 0, nullptr);
		}
		else if (kind == COREWEBVIEW2_CONTEXT_MENU_ITEM_KIND_SUBMENU)
		{
			HMENU newMenu = CreateMenu();
			

			wil::com_ptr<ICoreWebView2ContextMenuItemCollection> submenuItems;
			hr = current->get_Children(&submenuItems);
			RETURN_IF_FAILED(hr);

			
			hr = AddContextMenuItems(newMenu, submenuItems);
			RETURN_IF_FAILED(hr);


			AppendMenu(hPopupMenu, MF_POPUP, (UINT_PTR)newMenu, labelString.c_str());
		}
		else if (kind == COREWEBVIEW2_CONTEXT_MENU_ITEM_KIND_COMMAND)
		{
			if (isEnabled)
			{
				AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, commandId, labelString.c_str());
			}
			else
			{
				AppendMenu(hPopupMenu, MF_GRAYED | MF_STRING, commandId, labelString.c_str());
			}
		}
		else if (kind == COREWEBVIEW2_CONTEXT_MENU_ITEM_KIND_CHECK_BOX || kind == COREWEBVIEW2_CONTEXT_MENU_ITEM_KIND_RADIO)
		{
			if (isEnabled)
			{
				if (isChecked)
				{
					AppendMenu(hPopupMenu, MF_CHECKED | MF_STRING, commandId, labelString.c_str());
				}
				else
				{
					AppendMenu(hPopupMenu, MF_BYPOSITION | MF_STRING, commandId, labelString.c_str());
				}
			}
			else
			{
				if (isChecked)
				{
					AppendMenu(hPopupMenu, MF_CHECKED | MF_GRAYED | MF_STRING, commandId, labelString.c_str());
				}
				else
				{
					AppendMenu(hPopupMenu, MF_GRAYED | MF_STRING, commandId, labelString.c_str());
				}
			}
		}
	}

	return S_OK;
}

void WebUIWindow::runAsync(std::function<void()> callback)
{
	auto* task = new std::function<void()>(std::move(callback));

	PostMessage(getHandle(), WM_USER + 1, reinterpret_cast<WPARAM>(task), 0);
}

void WebUIWindow::onUser1(wui::WindowMessage& windowMessage)
{
	onExecuteTask(reinterpret_cast<void*>(windowMessage.wParam));
}

void WebUIWindow::onExecuteTask(void* ptr)
{
	auto* task = reinterpret_cast<std::function<void()>*>(ptr);


	(*task)();

	delete task;
}

//===========================================================================
HRESULT WebUIWindow::setupWebView_DevToolsProtocol_Security_securityStateChanged(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->CallDevToolsProtocolMethod(L"Security.enable", L"{}", nullptr);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView->GetDevToolsProtocolEventReceiver(L"Security.securityStateChanged", &_WebView_DevToolsProtocol_Security_securityStateChanged_EventReceiver);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView_DevToolsProtocol_Security_securityStateChanged_EventReceiver->add_DevToolsProtocolEventReceived(
		Microsoft::WRL::Callback<ICoreWebView2DevToolsProtocolEventReceivedEventHandler>
			(this, &WebUIWindow::onWebView_DevToolsProtocol_Security_securityStateChanged).Get(),
		&_WebView_DevToolsProtocol_Security_securityStateChanged_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_DevToolsProtocol_Security_securityStateChanged(ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsJsonArgs;


	hr = args->get_ParameterObjectAsJson(&ucsJsonArgs);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring jsonArgs(ucsJsonArgs.get());
	WUI_TRACE(jsonArgs);


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_DevToolsProtocol_Log_entryAdded(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->CallDevToolsProtocolMethod(L"Log.enable", L"{}", nullptr);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView->GetDevToolsProtocolEventReceiver(L"Log.entryAdded", &_WebView_DevToolsProtocol_Log_entryAdded_EventReceiver);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView_DevToolsProtocol_Log_entryAdded_EventReceiver->add_DevToolsProtocolEventReceived(
		Microsoft::WRL::Callback<ICoreWebView2DevToolsProtocolEventReceivedEventHandler>
			(this, &WebUIWindow::onWebView_DevToolsProtocol_Log_entryAdded).Get(),
		&_WebView_DevToolsProtocol_Log_entryAdded_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_DevToolsProtocol_Log_entryAdded(ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsJsonArgs;


	hr = args->get_ParameterObjectAsJson(&ucsJsonArgs);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring jsonArgs(ucsJsonArgs.get());
	//WUI_TRACE(jsonArgs);
	wui::debugPrintln(L"Log.entryAdded: ");
	wui::debugPrintln(jsonArgs);


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_DevToolsProtocol_Runtime_consoleAPICalled(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->CallDevToolsProtocolMethod(L"Runtime.enable", L"{}", nullptr);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView->GetDevToolsProtocolEventReceiver(L"Runtime.consoleAPICalled", &_WebView_DevToolsProtocol_Runtime_consoleAPICalled_EventReceiver);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView_DevToolsProtocol_Runtime_consoleAPICalled_EventReceiver->add_DevToolsProtocolEventReceived(
		Microsoft::WRL::Callback<ICoreWebView2DevToolsProtocolEventReceivedEventHandler>
			(this, &WebUIWindow::onWebView_DevToolsProtocol_Runtime_consoleAPICalled).Get(),
		&_WebView_DevToolsProtocol_Runtime_consoleAPICalled_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_DevToolsProtocol_Runtime_consoleAPICalled(ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsJsonArgs;


	hr = args->get_ParameterObjectAsJson(&ucsJsonArgs);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring jsonArgs(ucsJsonArgs.get());
	//WUI_TRACE(jsonArgs);
	wui::debugPrintln(L"Runtime.consoleAPICalled: ");
	wui::debugPrintln(jsonArgs);


	return S_OK;
}

HRESULT WebUIWindow::setupWebView_DevToolsProtocol_Runtime_exceptionThrown(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	hr = _WebView->GetDevToolsProtocolEventReceiver(L"Runtime.exceptionThrown", &_WebView_DevToolsProtocol_Runtime_exceptionThrown_EventReceiver);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	hr = _WebView_DevToolsProtocol_Runtime_exceptionThrown_EventReceiver->add_DevToolsProtocolEventReceived(
		Microsoft::WRL::Callback<ICoreWebView2DevToolsProtocolEventReceivedEventHandler>
			(this, &WebUIWindow::onWebView_DevToolsProtocol_Runtime_exceptionThrown).Get(),
		&_WebView_DevToolsProtocol_Runtime_exceptionThrown_Token
	);
	RETURN_IF_FAILED(hr);


	return S_OK;
}

HRESULT WebUIWindow::onWebView_DevToolsProtocol_Runtime_exceptionThrown(ICoreWebView2* sender, ICoreWebView2DevToolsProtocolEventReceivedEventArgs* args)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	wil::unique_cotaskmem_string ucsJsonArgs;


	hr = args->get_ParameterObjectAsJson(&ucsJsonArgs);
	RETURN_IF_FAILED(hr);


	//-----------------------------------------------------------------------
	std::wstring jsonArgs(ucsJsonArgs.get());
	//WUI_TRACE(jsonArgs);
	wui::debugPrintln(L"Runtime.exceptionThrown: ");
	wui::debugPrintln(jsonArgs);


	//-----------------------------------------------------------------------
	_Manager->getMessageService()->onRuntimeExceptionThrown(this, jsonArgs);


	return S_OK;
}

//===========================================================================
void WebUIWindow::postWebMessageAsJson(const std::wstring& msg)
{
	HRESULT hr;


	if (_WebView == nullptr)
	{
		return;
	}


	hr = _WebView->PostWebMessageAsJson(msg.c_str());
	if (FAILED(hr))
	{
		WUI_TRACE(L"failed");
	}
}

void WebUIWindow::onWebMessage(const std::wstring& urn, const std::wstring& webMessage)
{
#if 0
#if 1
	//------------------------------------------------------------------------
	// TryGetWebMessageAsString
	if (webMessage.compare(L"javascript-message") == 0)
	{
	}
#else


	//------------------------------------------------------------------------
	// get_WebMessageAsJson
	if (webMessage.compare(L"\"javascript-message\"") == 0)
	{
	}
#endif
#endif


	//------------------------------------------------------------------------
	WUI_TRACE(urn);
	WUI_TRACE(webMessage);


	//------------------------------------------------------------------------
	_Manager->getMessageService()->onWebMessage(this, webMessage);
}

//===========================================================================
void WebUIWindow::navigate(const std::wstring& uri)
{
	HRESULT hr;


	hr = _WebView->Navigate(uri.c_str());
	if (FAILED(hr))
	{
		WUI_TRACE(L"failed");
	}
}

void WebUIWindow::navigateContents(const std::wstring& urn)
{
	std::wstring uri;


	uri = _Manager->getContentsHost() + urn;
	navigate(uri);
}

void WebUIWindow::executeScript(const std::wstring& script)
{
	_WebView->ExecuteScript(script.c_str(),
		Microsoft::WRL::Callback<ICoreWebView2ExecuteScriptCompletedHandler>(
			[this](HRESULT errorCode, LPCWSTR resultObjectAsJson) -> HRESULT
			{
				if (FAILED(errorCode))
				{
					WUI_TRACE(L"failed");
				}

				return S_OK;
			}
		).Get()
	);
}

//===========================================================================
#pragma endregion





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




