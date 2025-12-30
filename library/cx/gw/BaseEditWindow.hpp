/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class BaseEditWindow : public Window
{
private:
	std::unique_ptr<DocumentGrid>      _DocumentGrid{};
	std::unique_ptr<StatusOverayPanel> _StatusOverayPanel{};

public:
	explicit BaseEditWindow(HWND hwnd, bool center = true);

	//-----------------------------------------------------------------------
	// Window
protected:
	virtual bool createDeviceIndependentResources(void) override;
	virtual void destroyDeviceIndependentResources(void) override;
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;

	//-----------------------------------------------------------------------
	// BaseEditWindow
public:
	DocumentGrid*      getDocumentGrid     (void) { return _DocumentGrid.get();      }
	StatusOverayPanel* getStatusOverayPanel(void) { return _StatusOverayPanel.get(); }

public:
	virtual void drawDocument(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




