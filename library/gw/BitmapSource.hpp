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
	bool loadFromMemory(const BYTE* pImageData, UINT imageSize);
	bool loadFromFile(const std::wstring& filePath);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




