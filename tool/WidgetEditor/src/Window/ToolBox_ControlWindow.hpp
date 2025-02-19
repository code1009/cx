#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ControlWindow : public cx::gw::Window
{
private:
	ToolBox* _ToolBox{ nullptr };
	std::unique_ptr<ToolBox::ItemView> _ItemView;
	ToolBox::WindowMessageHandler _WindowMessageHandler;

public:
	explicit ControlWindow(ToolBox* toolBox);
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
	ToolBox* getToolBox(void) { return _ToolBox; }
	ToolBox::ItemView* getItemView(void) { return _ItemView.get(); }
	ToolBox::WindowMessageHandler* getWindowMessageHandler(void) { return &_WindowMessageHandler; }
};

