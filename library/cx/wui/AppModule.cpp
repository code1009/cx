/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
HINSTANCE AppModule::getInstanceHandle(void) const
{
	return _InstanceHandle;
}

HINSTANCE AppModule::setInstanceHandle(HINSTANCE handle)
{
	HINSTANCE old;


	old = _InstanceHandle;
	_InstanceHandle = handle;

	return old;
}

//===========================================================================
HINSTANCE AppModule::getResourceInstanceHandle(void) const
{
	if (_ResourceInstanceHandle)
	{
		return _ResourceInstanceHandle;
	}

	return _InstanceHandle;
}

HINSTANCE AppModule::setResourceInstanceHandle(HINSTANCE handle)
{
	HINSTANCE old;


	old = _ResourceInstanceHandle;
	_ResourceInstanceHandle = handle;

	return old;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
AppModule* getAppModule(void)
{
	static AppModule instance;

	return &instance;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




