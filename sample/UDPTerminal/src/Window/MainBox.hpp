#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{
class WebUIManager;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainBox : 
	public cx::wui::MessageMapWindowT<MainBox, cx::wui::ModalTemplateBaseDialog>
{
private:
	std::shared_ptr<app::WebUIManager> _WebUIManager;

public:
	MainBox();

public:
	virtual ~MainBox();

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onUser0(cx::wui::WindowMessage& windowMessage);
	void onUser1(cx::wui::WindowMessage& windowMessage);
};
