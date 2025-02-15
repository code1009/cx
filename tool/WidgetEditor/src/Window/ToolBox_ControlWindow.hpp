#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ControlWindow : public cx::gw::Window
{
private:
	HWND _WindowHandle{ nullptr };
	std::unique_ptr<ToolBox::ItemView> _ItemView;

public:
	explicit ControlWindow(HWND hwnd);
	virtual ~ControlWindow(void) = default;

public:
	ControlWindow(const ControlWindow&) = delete;
	ControlWindow& operator=(const ControlWindow&) = delete;

	ControlWindow(ControlWindow&&) = delete;
	ControlWindow& operator=(ControlWindow&&) = delete;

	//-----------------------------------------------------------------------
	// cx::gw::Window
public:
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;

	//-----------------------------------------------------------------------
	// ToolBox::ControlWindow
	ToolBox::ItemView* getItemView(void);
};

