/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>

//===========================================================================
#include "WebUI.hpp"





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
//	::MessageBoxW(window->getHandle(), oss.str().c_str(), L"RuntimeExceptionThrown", MB_OK);


	//------------------------------------------------------------------------
	oss.str(L"");
	oss.clear();
}

//===========================================================================
void WebUIMessageService::onReceiveWebMessage(WebUIWindow* window, const std::wstring& message)
{
	//------------------------------------------------------------------------
	if (message == L"undefined")
	{
		return;
	}


	//------------------------------------------------------------------------
	web::json::value jsonMessage = web::json::value::parse(message);
	bool rv;


	rv = onReceiveCommand(window, jsonMessage);
}

//===========================================================================
bool WebUIMessageService::onReceiveCommand(WebUIWindow* window, web::json::value& jsonMessage)
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
	if (command == L"") { return onReceiveCommand_Empty(window, jsonMessage); }

	else if (command == L"페이지이동") { return onReceiveCommand_Navigate(window, jsonMessage); }
	else if (command == L"메시지") { return onReceiveCommand_Message(window, jsonMessage); }
	else if (command == L"파일갱신") { return onReceiveCommand_FileUpdate(window, jsonMessage); }

	return false;
}

bool WebUIMessageService::onReceiveCommand_Empty(WebUIWindow* window, web::json::value& jsonMessage)
{
	return true;
}

bool WebUIMessageService::onReceiveCommand_Navigate(WebUIWindow* window, web::json::value& jsonMessage)
{
	//------------------------------------------------------------------------
	web::json::value jsonTargetPage;


	jsonTargetPage = jsonMessage.at(L"TargetPage");


	//------------------------------------------------------------------------
	std::wstring targetPage;


	targetPage = jsonTargetPage.as_string();


	//------------------------------------------------------------------------
	if (targetPage == L"페이지1") { getWindow()->getView()->navigateContents(L"/page/page1.html"); }
	if (targetPage == L"페이지2") { getWindow()->getView()->navigateContents(L"/page/page2.html"); }
	if (targetPage == L"페이지3") { getWindow()->getView()->navigateContents(L"/page/page3.html"); }

	return true;
}

bool WebUIMessageService::onReceiveCommand_Message(WebUIWindow* window, web::json::value& jsonMessage)
{
	return true;
}

bool WebUIMessageService::onReceiveCommand_FileUpdate(WebUIWindow* window, web::json::value& jsonMessage)
{
	return true;
}

//===========================================================================
void WebUIMessageService::postWebMessage_Message(void)
{
	//------------------------------------------------------------------------
	web::json::value jsonMessage;


	jsonMessage[L"Command"] = web::json::value::string(L"메시지");
	jsonMessage[L"Message"] = web::json::value::string(L"안녕하세요? C++입니다.");


	//------------------------------------------------------------------------
	utility::stringstream_t stream;


	jsonMessage.serialize(stream);


	getWindow()->getView()->postWebMessageAsJson(stream.str());
}

std::wstring WebUIMessageService::getFile_Json(void)
{
	std::wstring json;

	return json;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




