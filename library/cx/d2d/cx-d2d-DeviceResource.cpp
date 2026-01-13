/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include <cx/runtime/runtime.hpp>
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DeviceResource::~DeviceResource()
{
	unregisterDeviceResource();
}

//===========================================================================
void DeviceResource::registerDeviceResource(DeviceResourceManager* deviceResourceManager)
{
	CX_RUNTIME_ASSERT(nullptr == _DeviceResourceManager);
	
	_DeviceResourceManager = deviceResourceManager;
	_DeviceResourceManager->registerDeviceResource(this);
}

void DeviceResource::unregisterDeviceResource(void)
{
	if (_DeviceResourceManager)
	{
		_DeviceResourceManager->unregisterDeviceResource(this);
		_DeviceResourceManager = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DeviceResourceManager::~DeviceResourceManager()
{
	releaseDeviceResources();
	_DeviceResources.clear();
}

//===========================================================================
void DeviceResourceManager::registerDeviceResource(DeviceResource* deviceResource)
{
	auto it = std::find(
		_DeviceResources.begin(),
		_DeviceResources.end(),
		deviceResource
	);
	if (it == _DeviceResources.end())
	{
		_DeviceResources.push_back(deviceResource);
	}
	else
	{
		CX_RUNTIME_ASSERT(0);
	}
}

void DeviceResourceManager::unregisterDeviceResource(DeviceResource* deviceResource)
{
	auto it = std::find(
		_DeviceResources.begin(),
		_DeviceResources.end(),
		deviceResource
	);
	if (it != _DeviceResources.end())
	{
		_DeviceResources.erase(it);
	}
	else
	{
		CX_RUNTIME_ASSERT(0);
	}
}

void DeviceResourceManager::releaseDeviceResources(void)
{
	for (auto& deviceResource : _DeviceResources)
	{
		deviceResource->releaseDeviceResources();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
