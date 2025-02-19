#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainFrame : 
	public cx::wui::MessageMapWindowT<MainFrame, cx::wui::BaseWindow>
{
private:
	std::unique_ptr<View> _View;
	std::unique_ptr<ToolBox> _ToolBox;
	cx::gw::WidgetEventDragDropNotifier _WidgetEventDragDropNotifier;

public:
	MainFrame();

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);	
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onKeyDown(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onAppAbout(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);
	void onNotify(cx::wui::WindowMessage& windowMessage);
	void onUser0(cx::wui::WindowMessage& windowMessage);

public:
	void onIdle(void);

public:
	void createToolBox(void);
	void onToolBoxGroupItemMouseClicked(ToolBox::EventParam* param);
	void onToolBoxGroupItemMouseDbClicked(ToolBox::EventParam* param);
	void onToolBoxGroupItemMouseDragging(ToolBox::EventParam* param);
	void onToolBoxSubItemMouseClicked(ToolBox::EventParam* param);
	void onToolBoxSubItemMouseDbClicked(ToolBox::EventParam* param);
	void onToolBoxSubItemMouseDragging(ToolBox::EventParam* param);
	void doToolBoxDragDrop(std::size_t id);
};
