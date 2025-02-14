#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox : 
	public cx::wui::MessageMapWindowT<ToolBox, cx::wui::BaseWindow>
{
public:
	using coord_t = float;

public:
	class Item;
	class GroupItem;
	class SubItem;

public:
	class ControlWindow;

public:
	class ItemDrawing;
	class GroupItemDrawing;
	class SubItemDrawing;
	//class RecalcLayout;
	//class Notify;

public:
	using ItemHandle = std::shared_ptr<Item>;
	using ItemHandles = std::vector<ItemHandle>;

private:
	HWND _ParentWindowHandle{ nullptr };
	std::unique_ptr<ControlWindow> _ControlWindow;

public:
	explicit ToolBox(HWND parentWindowHandle);

public:
	virtual HWND createToolBox(HWND parentWindowHandle);

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onHScroll(cx::wui::WindowMessage& windowMessage);
	void onVScroll(cx::wui::WindowMessage& windowMessage);
	void onMouseWheel(cx::wui::WindowMessage& windowMessage);
	void onKeyDown(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onPaint(cx::wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
