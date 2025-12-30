#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class AppModule
{
private:
	HINSTANCE _InstanceHandle{ nullptr };
	HINSTANCE _ResourceInstanceHandle{ nullptr };

public:
	AppModule() = default;
	virtual ~AppModule() = default;

public:
	AppModule(const AppModule&) = delete;
	AppModule& operator=(const AppModule&) = delete;

	AppModule(AppModule&&) = delete;
	AppModule& operator=(AppModule&&) = delete;

public:
	virtual HINSTANCE getInstanceHandle(void) const;
	virtual HINSTANCE setInstanceHandle(HINSTANCE handle);

public:
	virtual HINSTANCE getResourceInstanceHandle(void) const;
	virtual HINSTANCE setResourceInstanceHandle(HINSTANCE handle);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
AppModule* getAppModule(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




