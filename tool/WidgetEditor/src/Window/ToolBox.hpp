#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox : 
	public cx::wui::MessageMapWindowT<ToolBox, cx::wui::BaseWindow>
{
public:
	enum class ItemStyle
	{
		Flat,
		Button
	};

public:
	class ItemStatus;

	class Item;
	class GroupItem;
	class SubItem;

	class ItemView;
	
	class ControlWindow;

	class ItemViewDrawing;
	class ItemDrawing;
	
	class GroupItemDrawing;
	class GroupItemButtonDrawing;
	class GroupItemHoverDrawing;

	class SubItemDrawing;
	class SubItemButtonDrawing;
	class SubItemHoverDrawing;

	enum class EventType;
	class MouseEventParam;
	class WindowMessageHandler;

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
	virtual LRESULT onWindowMessage(cx::wui::WindowMessage& windowMessage) override;

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

public:
	void addIconBitmap(std::wstring name, std::wstring filePath);
	void addIconBitmap(std::wstring name, const void* pImageData, std::size_t imageSize);

public:
	GroupItemSharedPtr addGroupItem(
		std::size_t id,
		GroupItemSharedPtr parentItem,
		std::wstring caption, 
		std::wstring icon = std::wstring(), 
		ItemStyle style = ItemStyle::Flat, 
		cx::gw::coord_t size = 28.0f, 
		std::wstring description = std::wstring()
	);
	SubItemSharedPtr addSubItem(
		std::size_t id,
		GroupItemSharedPtr parentItem,
		std::wstring caption,
		std::wstring icon = std::wstring(),
		ItemStyle style = ItemStyle::Flat,
		cx::gw::coord_t size = 28.0f,
		std::wstring description = std::wstring()
	);

public:
	void releaseWindowMouseCapture(void);

public:
	class EventParam
	{
	public:
		ToolBox*    _Sender   { nullptr };
		std::size_t _Id       { 0       };
		bool        _DragDrop { false   };
	};
	using EventHandler = std::function<void(EventParam*)>;

private:
	EventHandler _GroupItemMouseClicked  {};
	EventHandler _GroupItemMouseDbClicked{};
	EventHandler _GroupItemMouseDragging {};
	EventHandler _SubItemMouseClicked    {};
	EventHandler _SubItemMouseDbClicked  {};
	EventHandler _SubItemMouseDragging   {};

public:
	void setGroupItemMouseClickedHandler  (EventHandler handler) { _GroupItemMouseClicked  =handler; };
	void setGroupItemMouseDbClickedHandler(EventHandler handler) { _GroupItemMouseDbClicked=handler; };
	void setGroupItemMouseDraggingHandler (EventHandler handler) { _GroupItemMouseDragging =handler; };
	void setSubItemMouseClickedHandler    (EventHandler handler) { _SubItemMouseClicked    =handler; };
	void setSubItemMouseDbClickedHandler  (EventHandler handler) { _SubItemMouseDbClicked  =handler; };
	void setSubItemMouseDraggingHandler   (EventHandler handler) { _SubItemMouseDragging   =handler; };
	
public:
	void setGroupItemMouseClicked  (std::size_t id, MouseEventParam& param){ notifyMouseEvent(_GroupItemMouseClicked  , id, param); }
	void setGroupItemMouseDbClicked(std::size_t id, MouseEventParam& param){ notifyMouseEvent(_GroupItemMouseDbClicked, id, param); }
	void setGroupItemMouseDragging (std::size_t id, MouseEventParam& param){ notifyMouseEvent(_GroupItemMouseDragging , id, param); }
	void setSubItemMouseClicked    (std::size_t id, MouseEventParam& param){ notifyMouseEvent(_SubItemMouseClicked    , id, param); }
	void setSubItemMouseDbClicked  (std::size_t id, MouseEventParam& param){ notifyMouseEvent(_SubItemMouseDbClicked  , id, param); }
	void setSubItemMouseDragging   (std::size_t id, MouseEventParam& param){ notifyMouseEvent(_SubItemMouseDragging   , id, param); }

public:
	void notifyMouseEvent(EventHandler handler, std::size_t id, MouseEventParam& param);
};
