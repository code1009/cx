﻿/////////////////////////////////////////////////////////////////////////////
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
void Context::setD2dFactory(ID2D1Factory* v)
{
	_pD2dFactory = v;
}

void Context::setDWriteFactory(IDWriteFactory* v)
{
	_pDWriteFactory = v;
}

void Context::setWICImagingFactory(IWICImagingFactory* v)
{
	_pWICImagingFactory = v;
}

//===========================================================================
void Context::setD2dRenderTarget(ID2D1RenderTarget* v)
{
	_pD2dRenderTarget = v;
}

void Context::setD2dHwndRenderTarget(ID2D1HwndRenderTarget* v)
{
	_pD2dHwndRenderTarget = v;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




