#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Context
{
private:
	HWND _hwnd{ nullptr };

private:
	wil::com_ptr_nothrow<ID2D1HwndRenderTarget>	_D2dHwndRenderTarget;

private:
	wil::com_ptr_nothrow<ID2D1Factory> _D2dFactory;
	wil::com_ptr_nothrow<IDWriteFactory> _DWriteFactory;
	wil::com_ptr_nothrow<IWICImagingFactory> _WICImagingFactory;

private:
	wil::com_ptr_nothrow<ID2D1SolidColorBrush> _CurrentSolidColorBrush;
	wil::com_ptr_nothrow<ID2D1StrokeStyle> _CurrentStrokeStyle;
	wil::com_ptr_nothrow<IDWriteTextFormat> _CurrentTextFormat;

private:
	std::unique_ptr<ResourceManager> _DeviceResourceManager;
	std::unique_ptr<ResourceManager> _DeviceIndependentResourceManager;

public:
	explicit Context(HWND hwnd, Factory* factory);
	virtual ~Context();

public:
	Context(const Context&) = delete;
	Context& operator=(const Context&) = delete;

	Context(Context&&) = delete;
	Context& operator=(Context&&) = delete;

public:
	HWND getHwnd(void) const noexcept
	{
		return _hwnd;
	}

public:
	bool isValid(void) const noexcept
	{
		return (_D2dHwndRenderTarget != nullptr);
	}

public:
	wil::com_ptr_nothrow<ID2D1Factory>& getD2dFactory(void);
	wil::com_ptr_nothrow<IDWriteFactory>& getDWriteFactory(void);
	wil::com_ptr_nothrow<IWICImagingFactory>& getWICImagingFactory(void);

public:
	bool createRenderTarget(std::uint32_t cx, std::uint32_t cy);
	bool createRenderTarget(void);
	void destroyRenderTarget(void);

public:
	wil::com_ptr_nothrow<ID2D1HwndRenderTarget>& getD2dHwndRenderTarget(void);

public:
	bool createResources(void);
	void destroyResources(void);

public:
	wil::com_ptr_nothrow<ID2D1SolidColorBrush>& getCurrentSolidColorBrush(void);
	wil::com_ptr_nothrow<ID2D1StrokeStyle>& getCurrentStrokeStyle(void);
	wil::com_ptr_nothrow<IDWriteTextFormat>& getCurrentTextFormat(void);

public:
	ResourceManager* getDeviceResourceManager(void);
	ResourceManager* getDeviceIndependentResourceManager(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
