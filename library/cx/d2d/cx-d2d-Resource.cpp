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
Resource::~Resource()
{
	unregisterResource();
}

//===========================================================================
void Resource::registerResource(ResourceManager* resourceManager)
{
	CX_RUNTIME_ASSERT(nullptr == _ResourceManager);
	
	_ResourceManager = resourceManager;
	_ResourceManager->registerResource(this);
}

void Resource::unregisterResource(void)
{
	if (_ResourceManager)
	{
		_ResourceManager->unregisterResource(this);
		_ResourceManager = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ResourceManager::~ResourceManager()
{
	releaseResources();
	_Resources.clear();
}

//===========================================================================
void ResourceManager::registerResource(Resource* resource)
{
	auto it = std::find(
		_Resources.begin(),
		_Resources.end(),
		resource
	);
	if (it == _Resources.end())
	{
		_Resources.push_back(resource);
	}
	else
	{
		CX_RUNTIME_ASSERT(0);
	}
}

void ResourceManager::unregisterResource(Resource* resource)
{
	auto it = std::find(
		_Resources.begin(),
		_Resources.end(),
		resource
	);
	if (it != _Resources.end())
	{
		_Resources.erase(it);
	}
	else
	{
		CX_RUNTIME_ASSERT(0);
	}
}

void ResourceManager::releaseResources(void)
{
	for (auto& resource : _Resources)
	{
		resource->releaseResources();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
