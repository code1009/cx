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

bool BitmapSource::loadFromMemory(const BYTE* pImageData, UINT imageSize)
{
	IWICStream*            pStream = nullptr;
	IWICBitmapDecoder*     pBitmapDecoder = nullptr;
	IWICBitmapFrameDecode* pBitmapFrameDecode = nullptr;
	IWICFormatConverter*   pFormatConverter = nullptr;

	IWICImagingFactory* pWICImagingFactory;
	pWICImagingFactory = DirectX2DGraphic::getWICImagingFactory();
	if (pWICImagingFactory)
	{
		return false;
	}


	HRESULT hr = pWICImagingFactory->CreateStream(&pStream);
	if (SUCCEEDED(hr))
	{
		hr = pStream->InitializeFromMemory(const_cast<BYTE*>(pImageData), imageSize);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICImagingFactory->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
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
		hr = pFormatConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	}
	//if (SUCCEEDED(hr))
	//{
	//	hr = pRenderTarget->CreateBitmapFromWicBitmap(pFormatConverter, nullptr, &_pBitmap);
	//}

	if (pStream) pStream->Release();
	if (pBitmapDecoder) pBitmapDecoder->Release();
	if (pBitmapFrameDecode) pBitmapFrameDecode->Release();
	//if (pFormatConverter) pFormatConverter->Release();

	if (SUCCEEDED(hr))
	{
		_pWICBitmapSource = pFormatConverter;
	}
	else
	{
		if (pFormatConverter) pFormatConverter->Release();
	}

	return SUCCEEDED(hr);
}

bool BitmapSource::loadFromFile(const std::wstring& filePath)
{
	IWICBitmapDecoder* pBitmapDecoder = nullptr;
	IWICBitmapFrameDecode* pBitmapFrameDecode = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;

	IWICImagingFactory* pWICImagingFactory;
	pWICImagingFactory = DirectX2DGraphic::getWICImagingFactory();
	if (pWICImagingFactory)
	{
		return false;
	}


	HRESULT hr = pWICImagingFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
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
		hr = pFormatConverter->Initialize(pBitmapFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);
	}

	if (pBitmapDecoder) pBitmapDecoder->Release();
	if (pBitmapFrameDecode) pBitmapFrameDecode->Release();

	if (SUCCEEDED(hr))
	{
		_pWICBitmapSource = pFormatConverter;
	}
	else
	{
		if (pFormatConverter) pFormatConverter->Release();
	}

	return SUCCEEDED(hr);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




