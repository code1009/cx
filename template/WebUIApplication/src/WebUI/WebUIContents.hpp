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
	WebUIContentsStream() = default;

public:
	virtual ~WebUIContentsStream() = default;

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

public:
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

public:
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

public:
	virtual ~WebUIContentsUTF8StringStream();

public:
	WebUIContentsUTF8StringStream(const WebUIContentsUTF8StringStream&) = delete;
	WebUIContentsResourceStream& operator=(const WebUIContentsUTF8StringStream&) = delete;

	WebUIContentsUTF8StringStream(WebUIContentsUTF8StringStream&&) = delete;
	WebUIContentsUTF8StringStream& operator=(WebUIContentsUTF8StringStream&&) = delete;

public:
	virtual IStream* getStream(void) const override;

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

public:
	~WebUIContents() = default;

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

public:
	WebUIExtensionMIMETypeMap();

public:
	~WebUIExtensionMIMETypeMap() = default;

public:
	WebUIExtensionMIMETypeMap(const WebUIExtensionMIMETypeMap& other);
	WebUIExtensionMIMETypeMap& operator=(const WebUIExtensionMIMETypeMap& other);

	WebUIExtensionMIMETypeMap(WebUIExtensionMIMETypeMap&& other) noexcept;
	WebUIExtensionMIMETypeMap& operator=(WebUIExtensionMIMETypeMap&& other) noexcept;

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

public:
	WebUIContentsMap() = default;

public:
	~WebUIContentsMap() = default;

public:
	WebUIContentsMap(const WebUIContentsMap& other);
	WebUIContentsMap& operator=(const WebUIContentsMap& other);

	WebUIContentsMap(WebUIContentsMap&& other) noexcept;
	WebUIContentsMap& operator=(WebUIContentsMap&& other) noexcept;

public:
	void registerContents(const std::wstring& urn, const std::wstring& headers, std::shared_ptr<WebUIContentsStream> stream);
	void registerContents(const std::wstring& urn, std::shared_ptr<WebUIContentsStream> stream);

public:
	WebUIContents* findContents(const std::wstring& urn);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




