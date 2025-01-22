#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainFrame : public wui::MessageMapWindowT<MainFrame, wui::BaseWindow>
{
public:
	std::unique_ptr<View> _View;

public:
	MainFrame();

public:
	void registerWindowMessageMap(void);
	void onCreate(wui::WindowMessage& windowMessage);
	void onDestroy(wui::WindowMessage& windowMessage);
	void onClose(wui::WindowMessage& windowMessage);
	void onSize(wui::WindowMessage& windowMessage);
	void onEraseBkgnd(wui::WindowMessage& windowMessage);
	void onPaint(wui::WindowMessage& windowMessage);
	void onCommand(wui::WindowMessage& windowMessage);
	void onMenuCommand(wui::WindowMessage& windowMessage);
	void onAppAbout(wui::WindowMessage& windowMessage);
	void onCtlCommand(wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
