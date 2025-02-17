#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox : 
	public cx::wui::MessageMapWindowT<ToolBox, cx::wui::BaseWindow>
{
public:
	enum class ItemStyle;
	class ItemStatus;
	class Item;
	class GroupItem;
	class SubItem;
	class ItemView;
	class ControlWindow;
	class ItemViewDrawing;
	class ItemDrawing;
	class GroupItemDrawing;
	class SubItemDrawing;

public:
	//class RecalcLayout;
	//class Notify;

public:
	using ItemWeakPtr = std::weak_ptr<Item>;
	using ItemSharedPtr = std::shared_ptr<Item>;
	using ItemSharedPtrs = std::vector<ItemSharedPtr>;

	using GroupItemWeakPtr = std::weak_ptr<GroupItem>;
	using GroupItemSharedPtr = std::shared_ptr<GroupItem>;
	using GroupItemSharedPtrs = std::vector<GroupItemSharedPtr>;

	using SubItemWeakPtr = std::weak_ptr<SubItem>;
	using SubItemSharedPtr = std::shared_ptr<SubItem>;
	using SubItemSharedPtrs = std::vector<SubItemSharedPtr>;

	using ItemViewDrawingWeakPtr = std::weak_ptr<ItemViewDrawing>;
	using ItemViewDrawingSharedPtr = std::shared_ptr<ItemViewDrawing>;
	using ItemViewDrawingSharedPtrs = std::vector<ItemViewDrawingSharedPtr>;

	using ItemDrawingWeakPtr = std::weak_ptr<ItemDrawing>;
	using ItemDrawingSharedPtr = std::shared_ptr<ItemDrawing>;
	using ItemDrawingSharedPtrs = std::vector<ItemDrawingSharedPtr>;

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
