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
protected:
	std::unique_ptr<DocumentGrid>      _DocumentGrid{};
	std::unique_ptr<StatusOverayPanel> _StatusOverayPanel{};

public:
	explicit BaseEditWindow(HWND hwnd, bool center = true);

	//-----------------------------------------------------------------------
	// Window
protected:
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;

	//-----------------------------------------------------------------------
	// BaseEditWindow
public:
	[[nodiscard]] constexpr DocumentGrid*      getDocumentGrid     (void) { return _DocumentGrid.get();      }
	[[nodiscard]] constexpr StatusOverayPanel* getStatusOverayPanel(void) { return _StatusOverayPanel.get(); }

public:
	virtual void drawDocument(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




