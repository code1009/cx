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
	if (_pDBitmap)
	{
		return true;
	}

	if (!_BitmapSource || _BitmapSource->isNull())
	{
		return false;
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
}




