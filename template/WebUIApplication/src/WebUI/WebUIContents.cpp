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
WebUIContentsStream::WebUIContentsStream()
{

}

WebUIContentsStream::~WebUIContentsStream()
{

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIContentsFileStream::WebUIContentsFileStream(const std::wstring& filePath) :
	_FilePath(filePath)
{
	bool rv;


	rv = load();
	if (false == rv)
	{
		throw std::wstring(L"WebUIContentsFileStream ctor:" + _FilePath);
	}
}

WebUIContentsFileStream::~WebUIContentsFileStream()
{
	unload();
}

IStream* WebUIContentsFileStream::getStream(void) const
{
	return _pStream;
};

bool WebUIContentsFileStream::load(void)
{
	//-------------------------------------------------------------------------
	std::vector<std::uint8_t> buffer;
	std::ifstream ifile(_FilePath, std::ios::binary);


	if (ifile.is_open())
	{
		ifile.seekg(0, std::ios::end);
		buffer.resize(ifile.tellg());

		ifile.seekg(0, std::ios::beg);
		ifile.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
		ifile.close();
	}
	else
	{
		return false;
	}


	//-------------------------------------------------------------------------
	std::size_t bufferSize = buffer.size();
	std::uint8_t* bufferPointer = reinterpret_cast<std::uint8_t*>(buffer.data());


	//-------------------------------------------------------------------------
	_hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, bufferSize);
	if (_hGlobal)
	{
		_pGlobal = ::GlobalLock(_hGlobal);
		if (_pGlobal)
		{
			CopyMemory(_pGlobal, bufferPointer, bufferSize);


			if (::CreateStreamOnHGlobal(_hGlobal, FALSE, &_pStream) == S_OK)
			{
				_GlobalSize = bufferSize;

				return true;
			}


			::GlobalUnlock(_hGlobal);
		}
		::GlobalFree(_hGlobal);
	}

	_hGlobal = nullptr;
	_pGlobal = nullptr;
	_GlobalSize = 0;

	_pStream = nullptr;

	return false;
}

void WebUIContentsFileStream::unload(void)
{
	if (_pStream)
	{
		_pStream->Release();
		_pStream = nullptr;
	}

	if (_hGlobal)
	{
		::GlobalUnlock(_hGlobal);
		::GlobalFree(_hGlobal);

		_hGlobal = nullptr;
		_pGlobal = nullptr;
		_GlobalSize = 0;
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIContentsResourceStream::WebUIContentsResourceStream(const std::wstring& resourceName):
	_ResourceName(resourceName)
{
	bool rv;


	rv = load();
	if (false == rv)
	{
		throw std::wstring(L"WebUIContentsResourceStream ctor:" + resourceName);
	}
}

WebUIContentsResourceStream::~WebUIContentsResourceStream()
{
	unload();
}

IStream* WebUIContentsResourceStream::getStream(void) const
{ 
	return _pStream; 
};

bool WebUIContentsResourceStream::load(void)
{
	//-------------------------------------------------------------------------
//	LPCWSTR pType = RT_HTML;
	LPCWSTR pType = L"WEB";

	HINSTANCE hInstance = cx::wui::getAppModule()->getInstanceHandle();


	HRSRC hResource = ::FindResourceW(hInstance, _ResourceName.c_str(), pType);
	if (!hResource)
	{
		return false;
	}

	DWORD bufferSize = ::SizeofResource(hInstance, hResource);
	if (!bufferSize)
	{
		return false;
	}

	const void* bufferPointer = ::LockResource(::LoadResource(hInstance, hResource));
	if (!bufferPointer)
	{
		return false;
	}


	//-------------------------------------------------------------------------
	_hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, bufferSize);
	if (_hGlobal)
	{
		_pGlobal = ::GlobalLock(_hGlobal);
		if (_pGlobal)
		{
			CopyMemory(_pGlobal, bufferPointer, bufferSize);


			if (::CreateStreamOnHGlobal(_hGlobal, FALSE, &_pStream) == S_OK)
			{
				_GlobalSize = bufferSize;

				_hResource = hResource;

				return true;
			}


			::GlobalUnlock(_hGlobal);
		}
		::GlobalFree(_hGlobal);
	}


	_hResource = nullptr;

	_hGlobal = nullptr;
	_pGlobal = nullptr;
	_GlobalSize = 0;

	_pStream = nullptr;

	return false;
}

void WebUIContentsResourceStream::unload(void)
{
	if (_pStream)
	{
		_pStream->Release();
		_pStream = nullptr;
	}

	if (_hGlobal)
	{
		::GlobalUnlock(_hGlobal);
		::GlobalFree(_hGlobal);

		_hGlobal = nullptr;
		_pGlobal = nullptr;
		_GlobalSize = 0;
	}

	_hResource = nullptr;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIContentsUTF8StringStream::WebUIContentsUTF8StringStream(const std::wstring& s)
{
	_UTF8String = wcs_to_mbcs(s, CP_UTF8);


	bool rv;


	rv = load();
	if (false == rv)
	{
		throw std::wstring(L"WebUIContentsUTF8StringStream ctor");
	}
}

WebUIContentsUTF8StringStream::WebUIContentsUTF8StringStream(const std::string& s)
{
	_UTF8String = mbcs_to_utf8(s, CP_ACP);


	bool rv;


	rv = load();
	if (false == rv)
	{
		throw std::wstring(L"WebUIContentsUTF8StringStream ctor");
	}
}

WebUIContentsUTF8StringStream::~WebUIContentsUTF8StringStream()
{
	unload();
}

IStream* WebUIContentsUTF8StringStream::getStream(void) const
{
	return _pStream;
};

std::wstring WebUIContentsUTF8StringStream::mbcs_to_wcs(std::string input, UINT codepage)
{
	int len = MultiByteToWideChar(codepage, 0, input.c_str(), -1, NULL, 0);


	if (len > 0)
	{
		std::vector<wchar_t> buf(len);


		MultiByteToWideChar(codepage, 0, input.c_str(), -1, &buf[0], len);

		return std::wstring(&buf[0]);
	}

	return std::wstring();
}

std::string WebUIContentsUTF8StringStream::wcs_to_mbcs(std::wstring input, UINT codepage)
{
	int len = WideCharToMultiByte(codepage, 0, input.c_str(), -1, NULL, 0, NULL, NULL);


	if (len > 0)
	{
		std::vector<char> buf(len);


		WideCharToMultiByte(codepage, 0, input.c_str(), -1, &buf[0], len, NULL, NULL);

		return std::string(&buf[0]);
	}

	return std::string();
}

std::string WebUIContentsUTF8StringStream::utf8_to_mbcs(std::string /*input*/utf8, UINT codepage)
{
	//	std::string  utf8 ;
	std::wstring utf16;
	std::string  mbcs;


	//	utf8  = input;
	utf16 = mbcs_to_wcs(utf8, CP_UTF8);
	mbcs = wcs_to_mbcs(utf16, codepage);

	return mbcs;
}

std::string WebUIContentsUTF8StringStream::mbcs_to_utf8(std::string /*input*/mbcs, UINT codepage)
{
	std::string  utf8;
	std::wstring utf16;
	//	std::string  mbcs ;


	//	mbcs  = input;
	utf16 = mbcs_to_wcs(mbcs, codepage);
	utf8 = wcs_to_mbcs(utf16, CP_UTF8);

	return utf8;
}

bool WebUIContentsUTF8StringStream::load(void)
{
	const void* ptr = _UTF8String.c_str();
	std::size_t size = _UTF8String.size();


	_hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, size);
	if (_hGlobal)
	{
		_pGlobal = ::GlobalLock(_hGlobal);
		if (_pGlobal)
		{
			CopyMemory(_pGlobal, ptr, size);


			if (::CreateStreamOnHGlobal(_hGlobal, FALSE, &_pStream) == S_OK)
			{
				_GlobalSize = size;

				return true;
			}


			::GlobalUnlock(_hGlobal);
		}
		else
		{
			if (0 == size)
			{
				if (::CreateStreamOnHGlobal(_hGlobal, FALSE, &_pStream) == S_OK)
				{
					_GlobalSize = size;

					return true;
				}
			}
		}
		::GlobalFree(_hGlobal);
	}


	_hGlobal = nullptr;
	_pGlobal = nullptr;
	_GlobalSize = 0;

	_pStream = nullptr;

	return false;
}

void WebUIContentsUTF8StringStream::unload(void)
{
	if (_pStream)
	{
		_pStream->Release();
		_pStream = nullptr;
	}

	if (_hGlobal)
	{
		::GlobalUnlock(_hGlobal);
		::GlobalFree(_hGlobal);

		_hGlobal = nullptr;
		_pGlobal = nullptr;
		_GlobalSize = 0;
	}


	_UTF8String.clear();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIContents::WebUIContents(
	const std::wstring& URN, 
	const std::wstring& headers, 
	std::shared_ptr<WebUIContentsStream> stream):
	_URN(URN), 
	_Headers(headers), 
	_Stream(stream) 
{
}

WebUIContents::~WebUIContents() 
{
}

WebUIContents::WebUIContents(const WebUIContents& other):
	_URN(other._URN), 
	_Headers(other._Headers), 
	_Stream(other._Stream) 
{
}

WebUIContents& WebUIContents::operator=(const WebUIContents& other) 
{
	if (this != &other) 
	{
		_URN = other._URN;
		_Headers = other._Headers;
		_Stream = other._Stream;
	}

	return *this;
}

WebUIContents::WebUIContents(WebUIContents&& other) noexcept :
	_URN(std::move(other._URN)), 
	_Headers(std::move(other._Headers)), 
	_Stream(std::move(other._Stream)) 
{
}

WebUIContents& WebUIContents::operator=(WebUIContents&& other) noexcept 
{
	if (this != &other) 
	{
		_URN = std::move(other._URN);
		_Headers = std::move(other._Headers);
		_Stream = std::move(other._Stream);
	}
	
	return *this;
}

std::wstring WebUIContents::getURN() const 
{
	return _URN;
}

std::wstring WebUIContents::getHeaders() const 
{
	return _Headers;
}

IStream* WebUIContents::getStream() const 
{
	return _Stream->getStream();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIExtensionMIMETypeMap::WebUIExtensionMIMETypeMap()
{
	registerDefault();
}

WebUIExtensionMIMETypeMap::~WebUIExtensionMIMETypeMap()
{

}

WebUIExtensionMIMETypeMap::WebUIExtensionMIMETypeMap(const WebUIExtensionMIMETypeMap& other) :
	_Container(other._Container)
{
}

WebUIExtensionMIMETypeMap& WebUIExtensionMIMETypeMap::operator=(const WebUIExtensionMIMETypeMap& other)
{
	if (this != &other)
	{
		_Container = other._Container;
	}

	return *this;
}

WebUIExtensionMIMETypeMap::WebUIExtensionMIMETypeMap(WebUIExtensionMIMETypeMap&& other) noexcept :
	_Container(std::move(other._Container))
{
}

WebUIExtensionMIMETypeMap& WebUIExtensionMIMETypeMap::operator=(WebUIExtensionMIMETypeMap&& other) noexcept
{
	if (this != &other)
	{
		_Container = std::move(other._Container);
	}

	return *this;
}

void WebUIExtensionMIMETypeMap::registerDefault(void)
{
	// https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
	registertExtensionMIMEType(L".aac"    , L"audio/aac"                                                                , L"AAC audio                                                                    ");
	registertExtensionMIMEType(L".abw"    , L"application/x-abiword"                                                    , L"AbiWord document                                                             ");
	registertExtensionMIMEType(L".apng"   , L"image/apng"                                                               , L"Animated Portable Network Graphics (APNG) image                              ");
	registertExtensionMIMEType(L".arc"    , L"application/x-freearc"                                                    , L"Archive document (multiple files embedded)                                   ");
	registertExtensionMIMEType(L".avif"   , L"image/avif"                                                               , L"AVIF image                                                                   ");
	registertExtensionMIMEType(L".avi"    , L"video/x-msvideo"                                                          , L"AVI: Audio Video Interleave                                                  ");
	registertExtensionMIMEType(L".azw"    , L"application/vnd.amazon.ebook"                                             , L"Amazon Kindle eBook format                                                   ");
	registertExtensionMIMEType(L".bin"    , L"application/octet-stream"                                                 , L"Any kind of binary data                                                      ");
	registertExtensionMIMEType(L".bmp"    , L"image/bmp"                                                                , L"Windows OS/2 Bitmap Graphics                                                 ");
	registertExtensionMIMEType(L".bz"     , L"application/x-bzip"                                                       , L"BZip archive                                                                 ");
	registertExtensionMIMEType(L".bz2"    , L"application/x-bzip2"                                                      , L"BZip2 archive                                                                ");
	registertExtensionMIMEType(L".cda"    , L"application/x-cdf"                                                        , L"CD audio                                                                     ");
	registertExtensionMIMEType(L".csh"    , L"application/x-csh"                                                        , L"C-Shell script                                                               ");
	registertExtensionMIMEType(L".css"    , L"text/css"                                                                 , L"Cascading Style Sheets (CSS)                                                 ");
	registertExtensionMIMEType(L".csv"    , L"text/csv"                                                                 , L"Comma-separated values (CSV)                                                 ");
	registertExtensionMIMEType(L".doc"    , L"application/msword"                                                       , L"Microsoft Word                                                               ");
	registertExtensionMIMEType(L".docx"   , L"application/vnd.openxmlformats-officedocument.wordprocessingml.document"  , L"Microsoft Word (OpenXML)                                                     ");
	registertExtensionMIMEType(L".eot"    , L"application/vnd.ms-fontobject"                                            , L"MS Embedded OpenType fonts                                                   ");
	registertExtensionMIMEType(L".epub"   , L"application/epub+zip"                                                     , L"Electronic publication (EPUB)                                                ");
	registertExtensionMIMEType(L".gz"     , L"application/gzip"                                                         , L"GZip Compressed Archive                                                      ");
	registertExtensionMIMEType(L".gif"    , L"image/gif"                                                                , L"Graphics Interchange Format (GIF)                                            ");
	registertExtensionMIMEType(L".htm"    , L"text/html"                                                                , L"HyperText Markup Language (HTML)                                             ");
	registertExtensionMIMEType(L".html"   , L"text/html"                                                                , L"HyperText Markup Language (HTML)                                             ");
	registertExtensionMIMEType(L".ico"    , L"image/vnd.microsoft.icon"                                                 , L"Icon format                                                                  ");
	registertExtensionMIMEType(L".ics"    , L"text/calendar"                                                            , L"iCalendar format                                                             ");
	registertExtensionMIMEType(L".jar"    , L"application/java-archive"                                                 , L"Java Archive (JAR)                                                           ");
	registertExtensionMIMEType(L".jpeg"   , L"image/jpeg"                                                               , L"JPEG images                                                                  ");
	registertExtensionMIMEType(L".jpg"    , L"image/jpeg"                                                               , L"JPEG images                                                                  ");
	registertExtensionMIMEType(L".js"     , L"text/javascript"                                                          , L"JavaScript(Specifications: HTML and RFC 9239)                                ");
	registertExtensionMIMEType(L".json"   , L"application/json"                                                         , L"JSON format                                                                  ");
	registertExtensionMIMEType(L".jsonld" , L"application/ld+json"                                                      , L"JSON-LD format                                                               ");
	registertExtensionMIMEType(L".mid"    , L"audio/midi"                                                               , L"Musical Instrument Digital Interface (MIDI)                                  ");
	registertExtensionMIMEType(L".midi"   , L"audio/x-midi"                                                             , L"Musical Instrument Digital Interface (MIDI)                                  ");
	registertExtensionMIMEType(L".mjs"    , L"text/javascript"                                                          , L"JavaScript module                                                            ");
	registertExtensionMIMEType(L".mp3"    , L"audio/mpeg"                                                               , L"MP3 audio                                                                    ");
	registertExtensionMIMEType(L".mp4"    , L"video/mp4"                                                                , L"MP4 video                                                                    ");
	registertExtensionMIMEType(L".mpeg"   , L"video/mpeg"                                                               , L"MPEG Video                                                                   ");
	registertExtensionMIMEType(L".mpkg"   , L"application/vnd.apple.installer+xml"                                      , L"Apple Installer Package                                                      ");
	registertExtensionMIMEType(L".odp"    , L"application/vnd.oasis.opendocument.presentation"                          , L"OpenDocument presentation document                                           ");
	registertExtensionMIMEType(L".ods"    , L"application/vnd.oasis.opendocument.spreadsheet"                           , L"OpenDocument spreadsheet document                                            ");
	registertExtensionMIMEType(L".odt"    , L"application/vnd.oasis.opendocument.text"                                  , L"OpenDocument text document                                                   ");
	registertExtensionMIMEType(L".oga"    , L"audio/ogg"                                                                , L"Ogg audio                                                                    ");
	registertExtensionMIMEType(L".ogv"    , L"video/ogg"                                                                , L"Ogg video                                                                    ");
	registertExtensionMIMEType(L".ogx"    , L"application/ogg"                                                          , L"Ogg                                                                          ");
	registertExtensionMIMEType(L".opus"   , L"audio/ogg"                                                                , L"Opus audio in Ogg container                                                  ");
	registertExtensionMIMEType(L".otf"    , L"font/otf"                                                                 , L"OpenType font                                                                ");
	registertExtensionMIMEType(L".png"    , L"image/png"                                                                , L"Portable Network Graphics                                                    ");
	registertExtensionMIMEType(L".pdf"    , L"application/pdf"                                                          , L"Adobe Portable Document Format (PDF)                                         ");
	registertExtensionMIMEType(L".php"    , L"application/x-httpd-php"                                                  , L"Hypertext Preprocessor (Personal Home Page)                                  ");
	registertExtensionMIMEType(L".ppt"    , L"application/vnd.ms-powerpoint"                                            , L"Microsoft PowerPoint                                                         ");
	registertExtensionMIMEType(L".pptx"   , L"application/vnd.openxmlformats-officedocument.presentationml.presentation", L"Microsoft PowerPoint (OpenXML)                                               ");
	registertExtensionMIMEType(L".rar"    , L"application/vnd.rar"                                                      , L"RAR archive                                                                  ");
	registertExtensionMIMEType(L".rtf"    , L"application/rtf"                                                          , L"Rich Text Format (RTF)                                                       ");
	registertExtensionMIMEType(L".sh"     , L"application/x-sh"                                                         , L"Bourne shell script                                                          ");
	registertExtensionMIMEType(L".svg"    , L"image/svg+xml"                                                            , L"Scalable Vector Graphics (SVG)                                               ");
	registertExtensionMIMEType(L".tar"    , L"application/x-tar"                                                        , L"Tape Archive (TAR)                                                           ");
	registertExtensionMIMEType(L".tif"    , L"image/tiff"                                                               , L"Tagged Image File Format (TIFF)                                              ");
	registertExtensionMIMEType(L".tiff"   , L"image/tiff"                                                               , L"Tagged Image File Format (TIFF)                                              ");
	registertExtensionMIMEType(L".ts"     , L"video/mp2t"                                                               , L"MPEG transport stream                                                        ");
	registertExtensionMIMEType(L".ttf"    , L"font/ttf"                                                                 , L"TrueType Font                                                                ");
	registertExtensionMIMEType(L".txt"    , L"text/plain"                                                               , L"Text, (generally ASCII or ISO 8859-n)                                        ");
	registertExtensionMIMEType(L".vsd"    , L"application/vnd.visio"                                                    , L"Microsoft Visio                                                              ");
	registertExtensionMIMEType(L".wav"    , L"audio/wav"                                                                , L"Waveform Audio Format                                                        ");
	registertExtensionMIMEType(L".weba"   , L"audio/webm"                                                               , L"WEBM audio                                                                   ");
	registertExtensionMIMEType(L".webm"   , L"video/webm"                                                               , L"WEBM video                                                                   ");
	registertExtensionMIMEType(L".webp"   , L"image/webp"                                                               , L"WEBP image                                                                   ");
	registertExtensionMIMEType(L".woff"   , L"font/woff"                                                                , L"Web Open Font Format (WOFF)                                                  ");
	registertExtensionMIMEType(L".woff2"  , L"font/woff2"                                                               , L"Web Open Font Format (WOFF)                                                  ");
	registertExtensionMIMEType(L".xhtml"  , L"application/xhtml+xml"                                                    , L"XHTML                                                                        ");
	registertExtensionMIMEType(L".xls"    , L"application/vnd.ms-excel"                                                 , L"Microsoft Excel                                                              ");
	registertExtensionMIMEType(L".xlsx"   , L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"        , L"Microsoft Excel (OpenXML)                                                    ");
	registertExtensionMIMEType(L".xml"    , L"application/xml"                                                          , L"XML(application/xml|text/xml|application/atom+xml)                           ");
	registertExtensionMIMEType(L".xul"    , L"application/vnd.mozilla.xul+xml"                                          , L"XUL                                                                          ");
	registertExtensionMIMEType(L".zip"    , L"application/zip"                                                          , L"ZIP archive(Windows uploads .zip with MIME type application/x-zip-compressed)");
	registertExtensionMIMEType(L".3gp"    , L"audio/3gpp"                                                               , L"3GPP audio/video container                                                   ");
	registertExtensionMIMEType(L".3g2"    , L"video/3gpp2"                                                              , L"3GPP2 audio/video container                                                  ");
	registertExtensionMIMEType(L".7z"     , L"application/x-7z-compressed"                                              , L"7-zip archive                                                                ");
}

void WebUIExtensionMIMETypeMap::registertExtensionMIMEType(const std::wstring& extension, const std::wstring& MIMEType, const std::wstring description)
{
	_Container[extension] = std::make_pair(MIMEType, description);
}

std::wstring WebUIExtensionMIMETypeMap::getExtensionMIMEType(const std::wstring& extension)
{
	auto it = _Container.find(extension);


	if (it != _Container.end()) {
		return it->second.first;
	}


	return L"";
}

std::wstring WebUIExtensionMIMETypeMap::getExtensionDescription(const std::wstring& extension)
{
	auto it = _Container.find(extension);


	if (it != _Container.end())
	{
		return it->second.second;
	}


	return L"";
}

std::wstring WebUIExtensionMIMETypeMap::getExtension(const std::wstring& urn) const
{
	size_t pos = urn.find_last_of(L'.');


	if (pos != std::wstring::npos)
	{
		return urn.substr(pos);
	}


	return L"";
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebUIContentsMap::WebUIContentsMap() 
{
}

WebUIContentsMap::~WebUIContentsMap() 
{
}

WebUIContentsMap::WebUIContentsMap(const WebUIContentsMap& other):
	_ExtensionMIMETypeMap(other._ExtensionMIMETypeMap), 
	_Container(other._Container) 
{
}

WebUIContentsMap& WebUIContentsMap::operator=(const WebUIContentsMap& other) 
{
	if (this != &other) 
	{
		_ExtensionMIMETypeMap = other._ExtensionMIMETypeMap;
		_Container = other._Container;
	}

	return *this;
}

WebUIContentsMap::WebUIContentsMap(WebUIContentsMap&& other) noexcept:
	_ExtensionMIMETypeMap(std::move(other._ExtensionMIMETypeMap)), 
	_Container(std::move(other._Container)) 
{
}

WebUIContentsMap& WebUIContentsMap::operator=(WebUIContentsMap&& other) noexcept 
{
	if (this != &other) 
	{
		_ExtensionMIMETypeMap = std::move(other._ExtensionMIMETypeMap);
		_Container = std::move(other._Container);
	}

	return *this;
}

void WebUIContentsMap::registerContents(const std::wstring& urn, const std::wstring& headers, std::shared_ptr<WebUIContentsStream> stream) 
{
	_Container[urn] = std::make_shared<WebUIContents>(urn, headers, stream);
}

void WebUIContentsMap::registerContents(const std::wstring& urn, std::shared_ptr<WebUIContentsStream> stream) 
{
	std::wstring extension;
	std::wstring headers;
	std::wstring MIMEType;


	extension = _ExtensionMIMETypeMap.getExtension(urn);
	MIMEType = _ExtensionMIMETypeMap.getExtensionMIMEType(extension);

	if (!MIMEType.empty())
	{
		headers = L"Content-Type: " + MIMEType;
	}
	else
	{
		headers = L"Content-Type: application/x-binary";
	}

	_Container[urn] = std::make_shared<WebUIContents>(urn, headers, stream);
}

WebUIContents* WebUIContentsMap::findContents(const std::wstring& urn)
{
	auto it = _Container.find(urn);


	if (it != _Container.end()) 
	{
		return it->second.get();
	}


	return nullptr;
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




