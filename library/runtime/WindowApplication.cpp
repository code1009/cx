﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "runtime.hpp"

//===========================================================================
#include "../gw/DirectX2DGraphic.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool WindowApplication::initialize(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	hr = OleInitialize(nullptr);
	if (FAILED(hr))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	if (gw::DirectX2DGraphic::createFactory() == false)
	{
		::OleUninitialize();

		return false;
	}


	//-----------------------------------------------------------------------
	_DebugMemory.enableMemoryCheck();
	_DebugMemory.startMemoryLeakCheck();


	return true;
}

void WindowApplication::terminate(void)
{
	//-----------------------------------------------------------------------
	_DebugMemory.endMemoryLeakCheck();


	//-----------------------------------------------------------------------
	gw::DirectX2DGraphic::destroyFactory();


	//-----------------------------------------------------------------------
	::OleUninitialize();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}

