/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerWindow : public BaseEditWindow
{
private:
	std::unique_ptr<WidgetDesignerModel> _WidgetDesignerModel;

public:
	explicit WidgetDesignerWindow(HWND hwnd, bool center = true);

	//-----------------------------------------------------------------------
	// Window
protected:
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;

	//-----------------------------------------------------------------------
	// BaseEditWindow
public:
	virtual void drawDocument(void) override;

	//-----------------------------------------------------------------------
	WidgetDesignerModel* getWidgetDesignerModel(void) { return _WidgetDesignerModel.get(); }
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




