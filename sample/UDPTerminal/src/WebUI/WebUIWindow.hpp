#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIManager;
class WebUIView;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void regsiterWebUIWindowClass(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebUIWindow :
	public cx::wui::MessageMapWindowT<WebUIWindow, cx::wui::BaseWindow>
{
private:
	WebUIManager*              _Manager{ nullptr };
	bool                       _PopupWindowStyle{ false };
	std::unique_ptr<WebUIView> _View{ nullptr };

public:
	explicit WebUIWindow(WebUIManager* manager, std::wstring uri, HWND parentWindowHandle, bool popupWindowStyle);

public:
	virtual ~WebUIWindow();

public:
	WebUIManager* getManager(void) const { return _Manager; }
	WebUIView*    getView   (void) const { return _View.get(); }

protected:
	virtual HWND createWebUIWindow(HWND parentWindowHandle, bool popupWindowStyle);

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onDPIChanged(cx::wui::WindowMessage& windowMessage);
	void onUser1(cx::wui::WindowMessage& windowMessage);

public:
	int getDPIAwareBound(int bound) const;
	bool isPopupWindow(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




