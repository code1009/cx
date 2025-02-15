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
WICBitmap::~WICBitmap()
{
	Release();
}

bool WICBitmap::loadBitmapFromMemory(const BYTE* pImageData, UINT imageSize)
{
	IWICStream*            pStream = nullptr;
	IWICBitmapDecoder*     pBitmapDecoder = nullptr;
	IWICBitmapFrameDecode* pBitmapFrameDecode = nullptr;
	IWICFormatConverter*   pFormatConverter = nullptr;


	IWICImagingFactory* pWICFactory;
	pWICFactory = DirectX2DGraphic::getWICImagingFactory();
	if (pWICFactory)
	{
		return false;
	}


	HRESULT hr = pWICFactory->CreateStream(&pStream);
	if (SUCCEEDED(hr))
	{
		hr = pStream->InitializeFromMemory(const_cast<BYTE*>(pImageData), imageSize);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICFactory->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pBitmapDecoder);
	}
	if (SUCCEEDED(hr))
	{
		hr = pBitmapDecoder->GetFrame(0, &pBitmapFrameDecode);
	}
	if (SUCCEEDED(hr))
	{
		hr = pWICFactory->CreateFormatConverter(&pFormatConverter);
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

void WICBitmap::Release(void)
{
	if (_pWICBitmapSource)
	{
		_pWICBitmapSource->Release();
		_pWICBitmapSource = nullptr;
	}
}

IWICBitmapSource* WICBitmap::getWICBitmapSource(void)
{
	return _pWICBitmapSource;
}

bool WICBitmap::isNull(void)
{
	return _pWICBitmapSource == nullptr;
}

void WICBitmap::getBitmapSize(std::size_t& cx, std::size_t& cy)
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




