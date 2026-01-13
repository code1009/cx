#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DeviceResourceManager;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DeviceResource
{
private:
	DeviceResourceManager* _DeviceResourceManager{ nullptr };

public:
	DeviceResource() = default;
	virtual ~DeviceResource();

public:
	DeviceResource(const DeviceResource&) = delete;
	DeviceResource& operator=(const DeviceResource&) = delete;
	DeviceResource(DeviceResource&&) = delete;
	DeviceResource& operator=(DeviceResource&&) = delete;

public:
	virtual void registerDeviceResource(DeviceResourceManager* deviceResourceManager);
	virtual void unregisterDeviceResource(void);
	virtual void releaseDeviceResources(void) = 0;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DeviceResourceManager
{
private:
	std::vector<DeviceResource*> _DeviceResources;

public:
	DeviceResourceManager() = default;
	virtual ~DeviceResourceManager();

public:
	DeviceResourceManager(const DeviceResourceManager&) = delete;
	DeviceResourceManager& operator=(const DeviceResourceManager&) = delete;
	DeviceResourceManager(DeviceResourceManager&&) = delete;
	DeviceResourceManager& operator=(DeviceResourceManager&&) = delete;

public:
	void registerDeviceResource(DeviceResource* deviceResource);
	void unregisterDeviceResource(DeviceResource* deviceResource);
	void releaseDeviceResources(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
