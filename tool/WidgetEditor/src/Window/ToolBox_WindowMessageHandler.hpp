/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class ToolBox::EventType
{
	Unknown,

	MouseMove,
	MouseLButtonDown,
	MouseLButtonUp,

	MousePressed,
	MouseReleased,
	MouseClicked,
	MouseDbClicked,
	MouseOver,
	MouseLeave,
	MouseDragging
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::MouseEventParam
{
public:
	cx::gw::Point _MousePosition      {};
	bool          _MouseLButtonPressed{ false };
	bool          _MouseRButtonPressed{ false };
	bool          _KeyShiftPressed    { false };
	bool          _KeyCtrlPressed     { false };
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::WindowMessageHandler
{
private:
	ToolBox::ItemView* _ItemView{ nullptr };

private:
	ToolBox::Item* _MousePressedItem  { nullptr };
	ToolBox::Item* _MouseReleasedItem { nullptr };
	ToolBox::Item* _MouseClickedItem  { nullptr };
	ToolBox::Item* _MouseDbclickedItem{ nullptr };
	ToolBox::Item* _MouseOverItem     { nullptr };
	ToolBox::Item* _MouseDraggingItem { nullptr };

private:
	bool _WindowMouseCaptureEnabled{ true };
	bool _WindowMouseCaptured      { false };

private:
	std::uint64_t _MouseClickedTime{ 0 };

public:
	WindowMessageHandler() = default;

public:
	virtual ~WindowMessageHandler() = default;

public:
	WindowMessageHandler(const WindowMessageHandler&) = delete;
	WindowMessageHandler& operator=(const WindowMessageHandler&) = delete;

	WindowMessageHandler(WindowMessageHandler&&) = delete;
	WindowMessageHandler& operator=(WindowMessageHandler&&) = delete;

public:
	void setItemView(ToolBox::ItemView* itemView);

public:
	void reset(void);

public:
	bool isIn(ToolBox::Item* test);

public:
	ToolBox::Item* hitTest(const cx::gw::Point& point);
	ToolBox::Item* hitTest(const cx::gw::Point& point, ToolBox::ItemSharedPtrs& items);
	ToolBox::Item* hitTest(const cx::gw::Point& point, ToolBox::ItemSharedPtr& item);

public:
	bool getWindowMouseCaptureEnabled (void);
	void setWindowMouseCaptureEnabled (bool enabled);

public:
	void setWindowMouseCapture(HWND hwnd);
	void releaseWindowMouseCapture(void);

public:
	void onWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void onWindowMouseMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled);

public:
	void onMouse(ToolBox::EventType eventType, ToolBox::MouseEventParam& param);

public:
	void onMouseMove        (ToolBox::MouseEventParam& param);
	void onMouseLButtonDown (ToolBox::MouseEventParam& param);
	void onMouseLButtonUp   (ToolBox::MouseEventParam& param);

public:
	void notifyMouseMove        (ToolBox::MouseEventParam& param);
	void notifyMouseLButtonDown (ToolBox::MouseEventParam& param);
	void notifyMouseLButtonUp   (ToolBox::MouseEventParam& param);

	void notifyMousePressed  (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseReleased (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseClicked  (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseDbClicked(ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseOver     (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseLeave    (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseDragging (ToolBox::Item* item, ToolBox::MouseEventParam& param);
};





