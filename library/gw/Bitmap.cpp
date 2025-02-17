/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BitmapSource::~BitmapSource()
{
	Release();
}

//===========================================================================
void BitmapSource::Release(void)
{
	if (_pWICBitmapSource)
	{
		_pWICBitmapSource->Release();
		_pWICBitmapSource = nullptr;
	}
}

IWICBitmapSource* BitmapSource::getWICBitmapSource(void)
{
	return _pWICBitmapSource;
}

bool BitmapSource::isNull(void)
{
	return _pWICBitmapSource == nullptr;
}

void BitmapSource::getBitmapSize(std::size_t& cx, std::size_t& cy)
{
	if (!_pWICBitmapSource)
	{
		return;
	}


	HRESULT hr;
	UINT width = 0;
	UINT height = 0;
	hr = _pWICBitmapSource->GetSize(&width, &height);
	if (FAILED(hr))
	{
		return;
	}


	cx = static_cast<std::size_t>(width);
	cy = static_cast<std::size_t>(height);
}

bool BitmapSource::loadFromMemory(const BYTE* pImageData, UINT imageSize, IWICImagingFactory* pWICImagingFactory)
{
	if (nullptr == pWICImagingFactory)
	{
		pWICImagingFactory = DirectX2DGraphic::getWICImagingFactory();
	}
	if (nullptr == pWICImagingFactory)
	{
		return false;
	}


	HRESULT hr;


	IWICStream*            pStream = nullptr;
	IWICBitmapDecoder*     pBitmapDecoder = nullptr;
	IWICBitmapFrameDecode* pBitmapFrameDecode = nullptr;
	IWICFormatConverter*   pFormatConverter = nullptr;


	hr = pWICImagingFactory->CreateStream(&pStream);
	if (SUCCEEDED(hr))
	{
		hr = pStream->InitializeFromMemory(const_cast<BYTE*>(pImageData), imageSize);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICImagingFactory->CreateDecoderFromStream(
			pStream,
			nullptr, 
			WICDecodeMetadataCacheOnLoad, 
			&pBitmapDecoder
		);
	}
	if (SUCCEEDED(hr))
	{
		hr = pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICImagingFactory->CreateFormatConverter(&pFormatConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = pFormatConverter->Initialize(
			pBitmapFrameDecode, 
			GUID_WICPixelFormat32bppPBGRA, 
			WICBitmapDitherTypeNone, 
			nullptr, 
			0.0f, 
			WICBitmapPaletteTypeCustom
		);
	}

	if (pStream           ) { pStream->Release();            }
	if (pBitmapDecoder    ) { pBitmapDecoder->Release();     }
	if (pBitmapFrameDecode) { pBitmapFrameDecode->Release(); }

	if (SUCCEEDED(hr))
	{
		_pWICBitmapSource = pFormatConverter;
	}
	else
	{
		if (pFormatConverter) { pFormatConverter->Release(); }
	}

	return SUCCEEDED(hr);
}

bool BitmapSource::loadFromFile(const std::wstring& filePath, IWICImagingFactory* pWICImagingFactory)
{
	if (nullptr == pWICImagingFactory)
	{
		pWICImagingFactory = DirectX2DGraphic::getWICImagingFactory();
	}
	if (nullptr == pWICImagingFactory)
	{
		return false;
	}


	HRESULT hr;


	IWICBitmapDecoder* pBitmapDecoder = nullptr;
	IWICBitmapFrameDecode* pBitmapFrameDecode = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;


	hr = pWICImagingFactory->CreateDecoderFromFilename(
		filePath.c_str(), 
		nullptr, 
		GENERIC_READ, 
		WICDecodeMetadataCacheOnLoad,
		&pBitmapDecoder
	);
	if (SUCCEEDED(hr))
	{
		hr = pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICImagingFactory->CreateFormatConverter(&pFormatConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = pFormatConverter->Initialize(
			pBitmapFrameDecode, 
			GUID_WICPixelFormat32bppPBGRA, 
			WICBitmapDitherTypeNone, 
			nullptr, 
			0.0f, 
			WICBitmapPaletteTypeCustom
		);
	}

	if (pBitmapDecoder    ) { pBitmapDecoder->Release();     }
	if (pBitmapFrameDecode) { pBitmapFrameDecode->Release(); }


	if (SUCCEEDED(hr))
	{
		_pWICBitmapSource = pFormatConverter;
	}
	else
	{
		if (pFormatConverter) { pFormatConverter->Release(); }
	}

	return SUCCEEDED(hr);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Bitmap::Bitmap()
{
	_BitmapSource = std::make_unique<BitmapSource>();
}

//===========================================================================
Bitmap::~Bitmap()
{
	Release();

	_BitmapSource.reset();
}

//===========================================================================
void Bitmap::Release(void)
{
	if (_pDBitmap)
	{
		_pDBitmap->Release();
		_pDBitmap = nullptr;
	}
}

ID2D1Bitmap* Bitmap::getDBitmap(void)
{
	return _pDBitmap;
}

BitmapSource* Bitmap::getBitmapSource(void)
{
	return _BitmapSource.get();
}

bool Bitmap::isNull(void)
{
	return _pDBitmap == nullptr;
}

void Bitmap::getBitmapSize(std::size_t& cx, std::size_t& cy)
{
	if (_pDBitmap)
	{
		D2D1_SIZE_F size = _pDBitmap->GetSize();
		cx = static_cast<std::size_t>(size.width);
		cy = static_cast<std::size_t>(size.height);
	}
	else
	{
		cx = 0;
		cy = 0;
	}
}

bool Bitmap::createDeviceResources(Context* ctx)
{
	if (!_BitmapSource || _BitmapSource->isNull())
	{
		return false;
	}

	if (!ctx->getD2dRenderTarget())
	{
		return false;
	}

	if (_pDBitmap)
	{
		return true;
	}

	HRESULT hr = ctx->getD2dRenderTarget()->CreateBitmapFromWicBitmap(
		_BitmapSource->getWICBitmapSource(), 
		nullptr, 
		&_pDBitmap
	);

	return SUCCEEDED(hr);
}

void Bitmap::destroyDeviceResources(void)
{
	Release();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BitmapSharedPtr makeBitmap(
	const void* pImageData, std::size_t imageSize,
	IWICImagingFactory* pWICImagingFactory
)
{
	auto bitmap = std::make_shared<Bitmap>();
	if (!bitmap)
	{
		return nullptr;
	}
	if (!bitmap->getBitmapSource()->loadFromMemory(
		static_cast<const BYTE*>(pImageData), 
		static_cast<UINT>(imageSize), 
		pWICImagingFactory))
	{
		return nullptr;
	}
	return bitmap;
}

BitmapSharedPtr makeBitmap(
	const std::wstring& filePath,
	IWICImagingFactory* pWICImagingFactory
)
{
	auto bitmap = std::make_shared<Bitmap>();
	if (!bitmap)
	{
		return nullptr;
	}
	if (!bitmap->getBitmapSource()->loadFromFile(filePath, pWICImagingFactory))
	{
		return nullptr;
	}
	return bitmap;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
BitmapList::~BitmapList()
{
	clearBitmaps();
}

//===========================================================================
bool BitmapList::createDeviceResources(Context* ctx)
{
	for (auto& pair : _BitmapMap)
	{
		if (!pair.second->createDeviceResources(ctx))
		{
			return false;
		}
	}
	return true;
}

void BitmapList::destroyDeviceResources(void)
{
	for (auto& pair : _BitmapMap)
	{
		pair.second->destroyDeviceResources();
	}
}

BitmapSharedPtr BitmapList::findBitmap(const std::wstring& key)
{
	auto it = _BitmapMap.find(key);
	if (it != _BitmapMap.end())
	{
		return it->second;
	}
	return nullptr;
}

bool BitmapList::addBitmap(const std::wstring& key, BitmapSharedPtr bitmap)
{
	auto result = _BitmapMap.emplace(key, std::move(bitmap));
	return result.second;
}

bool BitmapList::removeBitmap(const std::wstring& key)
{
	return _BitmapMap.erase(key) > 0;
}

void BitmapList::clearBitmaps(void)
{
	_BitmapMap.clear();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




