/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class BitmapSource
{
private:
	IWICBitmapSource* _pWICBitmapSource { nullptr };

public:
	BitmapSource() = default;

public:
	virtual ~BitmapSource();

public:
	BitmapSource(const BitmapSource&) = delete;
	BitmapSource& operator=(const BitmapSource&) = delete;

	BitmapSource(BitmapSource&&) = delete;
	BitmapSource& operator=(BitmapSource&&) = delete;

public:
	void Release(void);

public:
	IWICBitmapSource* getWICBitmapSource(void);

public:
	bool isNull(void);
	void getBitmapSize(std::size_t& cx, std::size_t& cy);

public:
	bool loadFromMemory(const BYTE* pImageData, UINT imageSize, IWICImagingFactory* pWICImagingFactory=nullptr);
	bool loadFromFile(const std::wstring& filePath, IWICImagingFactory* pWICImagingFactory = nullptr);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Bitmap
{
private:
	ID2D1Bitmap* _pDBitmap{ nullptr };
	std::unique_ptr<BitmapSource> _BitmapSource{};

public:
	Bitmap();

public:
	virtual ~Bitmap();

public:
	Bitmap(const Bitmap&) = delete;
	Bitmap& operator=(const Bitmap&) = delete;

	Bitmap(Bitmap&&) = delete;
	Bitmap& operator=(Bitmap&&) = delete;

public:
	void Release(void);

public:
	ID2D1Bitmap* getDBitmap(void);
	BitmapSource* getBitmapSource(void);

public:
	bool isNull(void);
	void getBitmapSize(std::size_t& cx, std::size_t& cy);

public:
	virtual bool createDeviceResources(Context* ctx);
	virtual void destroyDeviceResources(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using BitmapWeakPtr = std::weak_ptr<Bitmap>;
using BitmapSharedPtr = std::shared_ptr<Bitmap>;

//===========================================================================
BitmapSharedPtr makeBitmap(
	const void* pImageData, std::size_t imageSize, 
	IWICImagingFactory* pWICImagingFactory = nullptr
);

BitmapSharedPtr makeBitmap(
	const std::wstring& filePath, 
	IWICImagingFactory* pWICImagingFactory = nullptr
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class BitmapList
{
public:
	using BitmapMap = std::map<std::wstring, BitmapSharedPtr>;

private:
	BitmapMap _BitmapMap;

public:
	BitmapList() = default;

public:
	virtual ~BitmapList();

public:
	BitmapList(const BitmapList&) = delete;
	BitmapList& operator=(const BitmapList&) = delete;

	BitmapList(BitmapList&&) = delete;
	BitmapList& operator=(BitmapList&&) = delete;

public:
	virtual bool createDeviceResources(Context* ctx);
	virtual void destroyDeviceResources(void);

public:
	BitmapSharedPtr findBitmap(const std::wstring& key);
	bool addBitmap(const std::wstring& key, BitmapSharedPtr bitmap);
	bool removeBitmap(const std::wstring& key);
	void clearBitmaps(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




