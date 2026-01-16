#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Context;
class ResourceManager;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Resource
{
private:
	ResourceManager* _ResourceManager{ nullptr };

public:
	Resource() = default;
	virtual ~Resource();

public:
	Resource(const Resource&) = delete;
	Resource& operator=(const Resource&) = delete;
	Resource(Resource&&) = delete;
	Resource& operator=(Resource&&) = delete;

public:
	virtual void registerResource(ResourceManager* deviceResourceManager);
	virtual void unregisterResource(void);

public:
	virtual bool createResources(Context* context) = 0;
	virtual void releaseResources(void) = 0;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ResourceManager
{
private:
	std::vector<Resource*> _Resources;

public:
	ResourceManager() = default;
	virtual ~ResourceManager();

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

public:
	void registerResource(Resource* deviceResource);
	void unregisterResource(Resource* deviceResource);
	void releaseResources(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
