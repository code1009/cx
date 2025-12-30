/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "WebUI.hpp"

//===========================================================================
#include "../Model/UDPTerminal.hpp"
#include "../Model/Model.hpp"
#include "../Model/Utility.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIMessageService::WebUIMessageService(WebUIManager* manager):
	_Manager{ manager }
{
}

WebUIMessageService::~WebUIMessageService()
{
	clearWebMessageQueue();
}

//===========================================================================
WebUIManager* WebUIMessageService::getManager(void)
{
	return _Manager;
}

WebUIWindow* WebUIMessageService::getWindow(void)
{
	if (nullptr == _hWindow)
	{
		return nullptr;
	}


	WebUIWindow* window;
	window = _Manager->findWindow(_hWindow);
	return window;
}

void WebUIMessageService::setWindow(HWND hWindow)
{
	_hWindow = hWindow;
}

//===========================================================================
void WebUIMessageService::onRuntimeExceptionThrown(WebUIWindow* window, const std::wstring& message)
{
	//------------------------------------------------------------------------
	if (message == L"undefined")
	{
		return;
	}


	//------------------------------------------------------------------------
	web::json::value jsonMessage = web::json::value::parse(message);


	//------------------------------------------------------------------------
	if (!jsonMessage.has_field(L"exceptionDetails"))
	{
		return;
	}
	web::json::value jsonExceptionDetails;
	jsonExceptionDetails = jsonMessage.at(L"exceptionDetails");
	if (web::json::value::Object != jsonExceptionDetails.type())
	{
		return;
	}


	//------------------------------------------------------------------------
	if (!jsonExceptionDetails.has_field(L"columnNumber"))
	{
		return;
	}
	if (!jsonExceptionDetails.has_field(L"lineNumber"))
	{
		return;
	}
	if (!jsonExceptionDetails.has_field(L"text"))
	{
		return;
	}
	if (!jsonExceptionDetails.has_field(L"url"))
	{
		return;
	}


	//------------------------------------------------------------------------
	web::json::value jsonUrl;
	web::json::value jsonLineNumber;
	web::json::value jsonColumnNumber;
	web::json::value jsonText;


	jsonUrl = jsonExceptionDetails.at(L"url");
	jsonLineNumber = jsonExceptionDetails.at(L"lineNumber");
	jsonColumnNumber = jsonExceptionDetails.at(L"columnNumber");
	jsonText = jsonExceptionDetails.at(L"text");


	if (web::json::value::String != jsonUrl.type())
	{
		return;
	}
	if (web::json::value::Number != jsonLineNumber.type())
	{
		return;
	}
	if (web::json::value::Number != jsonColumnNumber.type())
	{
		return;
	}
	if (web::json::value::String != jsonText.type())
	{
		return;
	}


	//------------------------------------------------------------------------
	std::wstring url;
	int lineNumber;
	int columnNumber;
	std::wstring text;


	url = jsonUrl.as_string();
	lineNumber = jsonLineNumber.as_integer();
	columnNumber = jsonColumnNumber.as_integer();
	text = jsonText.as_string();


	//------------------------------------------------------------------------
	if (!jsonExceptionDetails.has_field(L"exception"))
	{
		return;
	}
	web::json::value jsonException;
	jsonException = jsonExceptionDetails.at(L"exception");
	if (web::json::value::Object != jsonException.type())
	{
		return;
	}
	

	//------------------------------------------------------------------------
	if (!jsonException.has_field(L"className"))
	{
		return;
	}
	if (!jsonException.has_field(L"description"))
	{
		return;
	}
	web::json::value jsonClassName;
	web::json::value jsonDescription;
	jsonClassName = jsonException.at(L"className");
	jsonDescription = jsonException.at(L"description");
	if (web::json::value::String != jsonClassName.type())
	{
		return;
	}
	if (web::json::value::String != jsonDescription.type())
	{
		return;
	}


	std::wstring className;
	std::wstring description;
	className = jsonClassName.as_string();
	description = jsonDescription.as_string();


	//------------------------------------------------------------------------
	std::wostringstream oss;
	oss << L"Exception"      << std::endl;
	oss << L"Url: "          << url          << std::endl;
	oss << L"LineNumber: "   << lineNumber   << std::endl;
	oss << L"ColumnNumber: " << columnNumber << std::endl;
	oss << L"Text: "         << text         << std::endl;
	oss << L"ClassName: "    << className    << std::endl;
	oss << L"Description: "  << description  << std::endl;
	oss << std::endl;
	OutputDebugStringW(oss.str().c_str());
}

//===========================================================================
void WebUIMessageService::onWebMessage(WebUIWindow* window, const std::wstring& message)
{
	//------------------------------------------------------------------------
	if (message == L"undefined")
	{
		return;
	}


	//------------------------------------------------------------------------
#if 1
	auto m = WebMessage_Alloc();
	m->_Command = L"메시지";
	m->_Message = message;
	_WebMessageRxQueue.push(m);
#endif


	//------------------------------------------------------------------------
#if 0
	web::json::value jsonMessage = web::json::value::parse(message);
	bool rv;


	rv = onCommand(window, jsonMessage);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLDebug)
			<< L"onCommand() failed";
	}
#endif
}

//===========================================================================
void WebUIMessageService::clearWebMessageQueue(void)
{
	clearWebMessageTxQueue();
	clearWebMessageRxQueue();
}

void WebUIMessageService::clearWebMessageTxQueue(void)
{
	std::size_t count;
	std::size_t i;

	count = _WebMessageTxQueue.count();
	for (i=0u; i<count; i++)
	{
		auto m = _WebMessageTxQueue.pop();
		WebMessage_Free(m);
	}
}

void WebUIMessageService::clearWebMessageRxQueue(void)
{
	std::size_t count;
	std::size_t i;

	count = _WebMessageRxQueue.count();
	for (i=0u; i<count; i++)
	{
		auto m = _WebMessageRxQueue.pop();
		WebMessage_Free(m);
	}
}

//===========================================================================
void WebUIMessageService::processWebMessageQueue(void)
{
	pullWebMessageQueue();
	postWebMessageQueue();
}

void WebUIMessageService::pullWebMessageQueue(void)
{
	std::size_t count;
	std::size_t i;

	count = _WebMessageRxQueue.count();
	for (i=0u; i<count; i++)
	{
		auto m = _WebMessageRxQueue.pop();

		pullWebMessage(m);

		WebMessage_Free(m);
	}
}

void WebUIMessageService::postWebMessageQueue(void)
{
	std::size_t count;
	std::size_t i;

	count = _WebMessageTxQueue.count();
	for (i=0u; i<count; i++)
	{
		auto m = _WebMessageTxQueue.pop();

		postWebMessage(m);

		WebMessage_Free(m);
	}
}

void WebUIMessageService::pullWebMessage(WebMessage* webMessage)
{
	//------------------------------------------------------------------------
	WebUIWindow* window = nullptr;


	//------------------------------------------------------------------------
	web::json::value jsonMessage = web::json::value::parse(webMessage->_Message);
	bool rv;


	rv = onCommand(window, jsonMessage);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLDebug)
			<< L"onCommand() failed";
	}
}

void WebUIMessageService::postWebMessage(WebMessage* webMessage)
{
	postStringMessage(webMessage->_Command, webMessage->_Message);
}

void WebUIMessageService::writeWebMessage(std::wstring command, std::wstring message)
{
	auto m = WebMessage_Alloc();
	m->_Command = command;
	m->_Message = message;
	_WebMessageTxQueue.push(m);
}

//===========================================================================
bool WebUIMessageService::onCommand(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	if (!jsonMessage.has_field(L"Command"))
	{
		return false;
	}


	//------------------------------------------------------------------------
	web::json::value jsonCommand;


	jsonCommand = jsonMessage.at(L"Command");
	switch (jsonCommand.type())
	{
	case web::json::value::String:
		break;

	default:
		return false;
		break;
	}


	//------------------------------------------------------------------------
	std::wstring command;
	command = jsonCommand.as_string();
	CX_RUNTIME_LOG(cxLDebug) << L"Command=" << command;
	//------------------------------------------------------------------------
	if (command == L"") { return onCommand_Empty(window, jsonMessage); }
	//------------------------------------------------------------------------
	else if (command == L"페이지이동") { return onCommand_PageNavigation(window, jsonMessage); }
	else if (command == L"메시지")     { return onCommand_StringMessage(window, jsonMessage); }
	else if (command == L"파일갱신")   { return onCommand_FileUpdate(window, jsonMessage); }
	else if (command == L"연결")       { return onCommand_Connect(window, jsonMessage); }
	else if (command == L"연결해제")   { return onCommand_Disconnect(window, jsonMessage); }
	else if (command == L"송신")       { return onCommand_Send(window, jsonMessage); }

	return false;
}

bool WebUIMessageService::onCommand_Empty(WebUIWindow* window, web::json::value& jsonMessage)
{
	return true;
}

bool WebUIMessageService::onCommand_PageNavigation(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	web::json::value jsonTargetPage;
	jsonTargetPage = jsonMessage.at(L"TargetPage");


	//------------------------------------------------------------------------
	std::wstring targetPage;
	targetPage = jsonTargetPage.as_string();


	//------------------------------------------------------------------------
	if (targetPage == L"집")
	{
		getWindow()->getView()->navigateContents(L"/page_home/page.html");
	}

	if (targetPage == L"페이지1") { getWindow()->getView()->navigateContents(L"/page_1/page.html"); }
	if (targetPage == L"페이지2") { getWindow()->getView()->navigateContents(L"/page_2/page.html"); }
	if (targetPage == L"페이지3") { getWindow()->getView()->navigateContents(L"/page_3/page.html"); }

	return true;
}

bool WebUIMessageService::onCommand_StringMessage(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	web::json::value jsonStringMessage;
	jsonStringMessage = jsonMessage.at(L"StringMessage");


	//------------------------------------------------------------------------
	std::wstring stringMessage;
	stringMessage = jsonStringMessage.as_string();
	MessageBoxW(getWindow()->getWindowHandle(), stringMessage.c_str(), L"메시지", MB_OK);
	// https://docs.microsoft.com/en-us/microsoft-edge/webview2/concepts/threading-model#re-entrancy

	//------------------------------------------------------------------------
	postStringMessage();

	return true;
}

bool WebUIMessageService::onCommand_FileUpdate(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	std::wstring json = getFile_Json();


	//------------------------------------------------------------------------
	getManager()->getContentsMap()->registerContents(
		L"/file.json",
		std::make_shared<WebUIContentsUTF8StringStream>(json)
	);


	//-----------------------------------------------------------------------
	std::string data;
	std::wstring uri;
	std::wstring urn;
	urn = L"/page_2/page.html";
	uri = getManager()->getContentsHost() + urn;
	auto e = getManager()->newPopupWindow(getWindow()->getWindowHandle(), uri, { 0,0,1000,1000 });

	return true;
}

//===========================================================================
bool WebUIMessageService::onCommand_Connect(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	web::json::value jsonLocalAddress;
	jsonLocalAddress = jsonMessage.at(L"LocalAddress");

	web::json::value jsonLocalPort;
	jsonLocalPort = jsonMessage.at(L"LocalPort");


	//------------------------------------------------------------------------
	std::wstring LocalAddress;
	LocalAddress = jsonLocalAddress.as_string();

	std::wstring LocalPort;
	LocalPort = jsonLocalPort.as_string();


	//------------------------------------------------------------------------
	web::json::value jsonRemoteAddress;
	jsonRemoteAddress = jsonMessage.at(L"RemoteAddress");

	web::json::value jsonRemotePort;
	jsonRemotePort = jsonMessage.at(L"RemotePort");


	//------------------------------------------------------------------------
	std::wstring RemoteAddress;
	RemoteAddress = jsonRemoteAddress.as_string();

	std::wstring RemotePort;
	RemotePort = jsonRemotePort.as_string();


	//------------------------------------------------------------------------
	getModel()->connect(LocalAddress, LocalPort, RemoteAddress, RemotePort);

	return true;
}

bool WebUIMessageService::onCommand_Disconnect(WebUIWindow* window, web::json::value& jsonMessage)
{
	getModel()->disconnect();

	return true;
}

bool WebUIMessageService::onCommand_Send(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	web::json::value jsonTxData;
	jsonTxData = jsonMessage.at(L"TxData");


	//------------------------------------------------------------------------
	std::wstring TxData;
	TxData = jsonTxData.as_string();


	//------------------------------------------------------------------------
	auto txData = hexStringToByteArray(TxData);
	getModel()->send(txData);

	return true;
}

//===========================================================================
void WebUIMessageService::postStringMessage(std::wstring command, std::wstring message)
{
	//------------------------------------------------------------------------
	web::json::value jsonMessage;
	jsonMessage[L"Command"] = web::json::value::string(command);
	jsonMessage[L"StringMessage"] = web::json::value::string(message);


	//------------------------------------------------------------------------
	utility::stringstream_t stream;
	jsonMessage.serialize(stream);
	getWindow()->getView()->postWebMessageAsJson(stream.str());
}

void WebUIMessageService::postStringMessage(void)
{
	//------------------------------------------------------------------------
	web::json::value jsonMessage;
	jsonMessage[L"Command"] = web::json::value::string(L"메시지");
	jsonMessage[L"StringMessage"] = web::json::value::string(L"안녕하세요? C++입니다.");


	//------------------------------------------------------------------------
	utility::stringstream_t stream;
	jsonMessage.serialize(stream);
	getWindow()->getView()->postWebMessageAsJson(stream.str());
}

//===========================================================================
std::wstring WebUIMessageService::getFile_Json(void)
{
	//------------------------------------------------------------------------
	SYSTEMTIME st;
	GetLocalTime(&st);


	//------------------------------------------------------------------------
	std::wstring currentDateTime;
	currentDateTime = std::format(L"{:04}-{:02}-{:02} {:02}:{:02}:{:02}",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);


	//------------------------------------------------------------------------
	std::vector<std::vector<std::wstring>> dataView;
	dataView.push_back({ L"1", currentDateTime, L"INFO", L"안녕하세요?" });
	dataView.push_back({ L"2", currentDateTime, L"INFO", L"안녕하세요?" });


	//------------------------------------------------------------------------
	std::vector<web::json::value> collection;
	for (auto& e : dataView)
	{
		web::json::value rec;
		rec = web::json::value::array(4);
		rec[0] = web::json::value::string(e[0]);
		rec[1] = web::json::value::string(e[1]);
		rec[2] = web::json::value::string(e[2]);
		rec[3] = web::json::value::string(e[3]);
		collection.push_back(rec);
	}


	//------------------------------------------------------------------------
	web::json::value jsonMessage;
	jsonMessage = web::json::value::array(collection);


	//------------------------------------------------------------------------
	utility::stringstream_t stream;
	jsonMessage.serialize(stream);
	return stream.str();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




