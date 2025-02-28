#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIContentsStream
{
public:
	WebUIContentsStream();
	virtual ~WebUIContentsStream();

public:
	virtual IStream* getStream(void) const = 0;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIContentsFileStream final : public WebUIContentsStream
{
private:
	std::wstring _FilePath{ };

	HGLOBAL     _hGlobal{ nullptr };
	void* _pGlobal{ nullptr };
	std::size_t _GlobalSize{ 0 };

	IStream* _pStream{ nullptr };

public:
	explicit WebUIContentsFileStream(const std::wstring& filePath);
	virtual ~WebUIContentsFileStream();

public:
	WebUIContentsFileStream(const WebUIContentsFileStream&) = delete;
	WebUIContentsFileStream& operator=(const WebUIContentsFileStream&) = delete;

	WebUIContentsFileStream(WebUIContentsFileStream&&) = delete;
	WebUIContentsFileStream& operator=(WebUIContentsFileStream&&) = delete;

public:
	virtual IStream* getStream(void) const override;

private:
	bool load(void);
	void unload(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIContentsResourceStream final : public WebUIContentsStream
{
private:
	std::wstring _ResourceName{ };
	HRSRC        _hResource{ nullptr };

	HGLOBAL     _hGlobal{ nullptr };
	void*       _pGlobal{ nullptr };
	std::size_t _GlobalSize{ 0 };

	IStream* _pStream{ nullptr };

public:
	explicit WebUIContentsResourceStream(const std::wstring& resourceName);
	virtual ~WebUIContentsResourceStream();

public:
	WebUIContentsResourceStream(const WebUIContentsResourceStream&) = delete;
	WebUIContentsResourceStream& operator=(const WebUIContentsResourceStream&) = delete;

	WebUIContentsResourceStream(WebUIContentsResourceStream&&) = delete;
	WebUIContentsResourceStream& operator=(WebUIContentsResourceStream&&) = delete;

public:
	virtual IStream* getStream(void) const override;

private:
	bool load(void);
	void unload(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIContentsUTF8StringStream final : public WebUIContentsStream
{
private:
	std::string _UTF8String{ };

	HGLOBAL     _hGlobal{ nullptr };
	void*       _pGlobal{ nullptr };
	std::size_t _GlobalSize{ 0 };

	IStream* _pStream{ nullptr };

public:
	explicit WebUIContentsUTF8StringStream(const std::wstring& s);
	explicit WebUIContentsUTF8StringStream(const std::string& s);
	virtual ~WebUIContentsUTF8StringStream();

public:
	WebUIContentsUTF8StringStream(const WebUIContentsUTF8StringStream&) = delete;
	WebUIContentsResourceStream& operator=(const WebUIContentsUTF8StringStream&) = delete;

	WebUIContentsUTF8StringStream(WebUIContentsUTF8StringStream&&) = delete;
	WebUIContentsUTF8StringStream& operator=(WebUIContentsUTF8StringStream&&) = delete;

public:
	virtual IStream* getStream(void) const override;

public:
	std::wstring mbcs_to_wcs(std::string input, UINT codepage);
	std::string  wcs_to_mbcs(std::wstring input, UINT codepage);
	std::string  utf8_to_mbcs(std::string /*input*/utf8, UINT codepage);
	std::string  mbcs_to_utf8(std::string /*input*/mbcs, UINT codepage);

private:
	bool load(void);
	void unload(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIContents final
{
private:
	std::wstring _URN{};
	std::wstring _Headers{};
	std::shared_ptr<WebUIContentsStream> _Stream{};

public:
	explicit WebUIContents(const std::wstring& URN, const std::wstring& headers, std::shared_ptr<WebUIContentsStream> stream);
	~WebUIContents();

public:
	WebUIContents(const WebUIContents& other);
	WebUIContents& operator=(const WebUIContents& other);

	WebUIContents(WebUIContents&& other) noexcept;
	WebUIContents& operator=(WebUIContents&& other) noexcept;

public:
	std::wstring getURN    (void) const;
	std::wstring getHeaders(void) const;
	IStream*     getStream (void) const;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIExtensionMIMETypeMap final
{
private:
	std::map<std::wstring, std::pair<std::wstring, std::wstring>> _Container{};


	//-------------------------------------------------------------------------
public:
	WebUIExtensionMIMETypeMap();
	~WebUIExtensionMIMETypeMap();


	//-------------------------------------------------------------------------
public:
	WebUIExtensionMIMETypeMap(const WebUIExtensionMIMETypeMap& other);
	WebUIExtensionMIMETypeMap& operator=(const WebUIExtensionMIMETypeMap& other);

	WebUIExtensionMIMETypeMap(WebUIExtensionMIMETypeMap&& other) noexcept;
	WebUIExtensionMIMETypeMap& operator=(WebUIExtensionMIMETypeMap&& other) noexcept;


	//-------------------------------------------------------------------------
private:
	void registerDefault(void);

public:
	void registertExtensionMIMEType(
		const std::wstring& extension, 
		const std::wstring& MIMEType = L"application/x-binary", 
		const std::wstring description = L""
	);
	std::wstring getExtensionMIMEType(const std::wstring& extension);
	std::wstring getExtensionDescription(const std::wstring& extension);


	//-------------------------------------------------------------------------
public:
	std::wstring getExtension(const std::wstring& urn) const;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIContentsMap final
{
private:
	WebUIExtensionMIMETypeMap _ExtensionMIMETypeMap{};
	std::map< std::wstring, std::shared_ptr<WebUIContents>> _Container{};


	//-------------------------------------------------------------------------
public:
	WebUIContentsMap();
	~WebUIContentsMap();


	//-------------------------------------------------------------------------
public:
	WebUIContentsMap(const WebUIContentsMap& other);
	WebUIContentsMap& operator=(const WebUIContentsMap& other);

	WebUIContentsMap(WebUIContentsMap&& other) noexcept;
	WebUIContentsMap& operator=(WebUIContentsMap&& other) noexcept;


	//-------------------------------------------------------------------------
public:
	void registerContents(const std::wstring& urn, const std::wstring& headers, std::shared_ptr<WebUIContentsStream> stream);
	void registerContents(const std::wstring& urn, std::shared_ptr<WebUIContentsStream> stream);

public:
	WebUIContents* findContents(const std::wstring& urn);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




