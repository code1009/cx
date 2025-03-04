﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>
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
WebUIManager::WebUIManager(HWND hMainWindow):
	_hMainWindow{ hMainWindow }
{
	_MessageService = std::make_shared<WebUIMessageService>(this);

	registerContentsMap();
}

WebUIManager::~WebUIManager()
{
	CX_RUNTIME_LOG(LDbug)
		<< L"_WindowMap.size() = " << _WindowMap.size()
		<< std::endl;

	_WindowMap.clear();
}

//===========================================================================
std::wstring WebUIManager::getContentsDataFolder(void) const
{
	WCHAR path[MAX_PATH];


	std::wstring dataDirectory;
	HRESULT hr;


	// L"C:\\Users\\USER\\AppData\\Roaming";
	hr = SHGetFolderPathW(nullptr, CSIDL_APPDATA, NULL, 0, path);
	if (SUCCEEDED(hr))
	{
		dataDirectory = std::wstring(path);

		dataDirectory.append(L"\\wui\\");
	}
	else
	{
		dataDirectory = std::wstring(L".\\");
	}

	dataDirectory.append(L"app");
	dataDirectory.append(L"\\data");


	return dataDirectory;
}

std::wstring WebUIManager::getContentsHost(void) const
{
	std::wstring host = L"https://code1009.github.io";


	return host;
}

std::wstring WebUIManager::getContentsURI(const std::wstring& urn) const
{
	std::wstring uri;


	uri = getContentsHost() + urn;

	return uri;
}

std::wstring WebUIManager::parseContentsURN(const std::wstring& uri) const
{
	std::wstring host = getContentsHost();
	std::wstring urn;


	if (0 == uri.compare(0, host.size(), host))
	{
		urn = uri.substr(host.size());
	}


	auto pos = urn.find_first_of(L'?');


	if (pos != std::string::npos)
	{
		return urn.substr(0, pos);
	}


	return urn;
}

//===========================================================================
std::wstring replaceBackslashesWithSlashes(const std::wstring& input) 
{
	std::wstring result = input;
	std::wstring::size_type pos = 0;


	while ((pos = result.find(L'\\', pos)) != std::wstring::npos) 
	{
		result.replace(pos, 1, L"/");
		++pos;
	}

	return result;
}

std::vector<std::wstring> getFileList(const std::wstring& directoryPath) 
{
	std::vector<std::wstring> fileList;

	
	try 
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (entry.is_regular_file()) 
			{
				fileList.push_back(replaceBackslashesWithSlashes(entry.path().wstring()));
			}
		}
	}
	catch (const std::filesystem::filesystem_error& ) 
	{
	}

	return fileList;
}

void loadWebUIContentsMapFromFile(WebUIContentsMap& _ContentsMap)
{
	std::vector<std::wstring> fileList;
	std::wstring baseDirectory;
	std::wstring urn;


	baseDirectory = L"D:\\res\\web";

	fileList = getFileList(baseDirectory);


	for (auto& filePath : fileList)
	{
		urn = filePath.substr(baseDirectory.size());
		_ContentsMap.registerContents(urn, std::make_shared<WebUIContentsFileStream>(filePath));

		CX_RUNTIME_LOG(LDbug)
			<< urn << L" : " << filePath;
	}
}

void loadWebUIContentsMapFromResource(WebUIContentsMap& _ContentsMap)
{
	//------------------------------------------------------------------------
	std::vector<std::wstring> resourceCollection
	{
		L"/basic.css",
		L"/core.js",             
		L"/images/logo.png"    ,
		L"/page/home.css"          ,
		L"/page/home.html"         ,
		L"/page/home.js"           
	};


	for (std::wstring res : resourceCollection)
	{
		_ContentsMap.registerContents(res, std::make_shared<WebUIContentsResourceStream>(res));
	}
}

void WebUIManager::registerContentsMap(void)
{
	//------------------------------------------------------------------------
	//loadWebUIContentsMapFromFile(_ContentsMap);
	loadWebUIContentsMapFromResource(_ContentsMap);


	//------------------------------------------------------------------------
	//_ContentsMap.registerWebContents(L"/favicon.ico", std::make_shared<WebContentsResourceStream>(L"favicon.ico"));


	//------------------------------------------------------------------------
	std::wstring json;


	json = _MessageService->getFile_Json();

	_ContentsMap.registerContents(L"/file.json", std::make_shared<WebUIContentsUTF8StringStream>(json));
}

WebUIContentsMap* WebUIManager::getContentsMap(void)
{
	return &_ContentsMap;
}

//===========================================================================
WebUIMessageService* WebUIManager::getMessageService(void)
{
	return _MessageService.get();
}

//===========================================================================
std::shared_ptr<WebUIWindow> WebUIManager::newPopupWindow(HWND hParentWindow, std::wstring uri)
{
	hParentWindow=_hMainWindow;
	auto e = std::make_shared<WebUIWindow>(this, uri, hParentWindow, true);

	
	_WindowMap[e->getWindowHandle()] = e;


	return e;
}

std::shared_ptr<WebUIWindow> WebUIManager::newPopupWindow(HWND hParentWindow, std::wstring uri, const RECT& rect)
{
	auto e = std::make_shared<WebUIWindow>(this, uri, hParentWindow, true);

	
	cx::wui::moveWindow(e->getWindowHandle(), rect);


	_WindowMap[e->getWindowHandle()] = e;


	return e;
}

std::shared_ptr<WebUIWindow> WebUIManager::newChildWindow(HWND hParentWindow, std::wstring uri)
{
	auto e = std::make_shared<WebUIWindow>(this, uri, hParentWindow, false);


	_WindowMap[e->getWindowHandle()] = e;


	return e;
}

void WebUIManager::deleteWindow(HWND hWindow)
{
	auto it = _WindowMap.find(hWindow);


	if(it!=_WindowMap.end())
	{
		_WindowMap.erase(it);
	}
	else
	{
		CX_RUNTIME_LOG(LDbug) << L"window handle not found";
	}
}

void WebUIManager::onDestroyWindow(HWND hWindow)
{
	::PostMessage(_hMainWindow, WM_USER + 0, (WPARAM)hWindow, 0);
}

void WebUIManager::deleteAndDestroyAllWindows(void)
{
	for (auto e : _WindowMap)
	{
		e.second->destroyWindow();
	}

	_WindowMap.clear();
}

void WebUIManager::moveWindow(HWND hParentWindow, const RECT& rect)
{
	HWND hParent;


	for(auto e : _WindowMap)
	{
		hParent = GetParent(e.first);

		if (hParent == hParentWindow)
		{
			cx::wui::moveWindow(e.first, rect);
		}
	}
}

WebUIWindow* WebUIManager::findWindow(HWND hWindow)
{
	auto it = _WindowMap.find(hWindow);


	if (it != _WindowMap.end())
	{
		return it->second.get();
	}

	return nullptr;
}

void WebUIManager::postBroadcastWebMessageAsJson(const std::wstring& msg)
{
	for (auto& e : _WindowMap)
	{
		e.second->getView()->postWebMessageAsJson(msg);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




