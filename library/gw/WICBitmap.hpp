/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WICBitmap
{
private:
	IWICBitmapSource* _pWICBitmapSource { nullptr };

public:
	WICBitmap() = default;

public:
	virtual ~WICBitmap();

public:
	WICBitmap(const WICBitmap&) = delete;
	WICBitmap& operator=(const WICBitmap&) = delete;

	WICBitmap(WICBitmap&&) = delete;
	WICBitmap& operator=(WICBitmap&&) = delete;

public:
	void Release(void);

public:
	IWICBitmapSource* getWICBitmapSource(void);

public:
	bool isNull(void);
	void getBitmapSize(std::size_t& cx, std::size_t& cy);

public:
	bool loadBitmapFromMemory(const BYTE* pImageData, UINT imageSize);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




