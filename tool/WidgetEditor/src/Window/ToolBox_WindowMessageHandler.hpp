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
	cx::gw::Point _MousePosition      { };
	bool          _MouseLButtonPressed{ false };
	bool          _MouseRButtonPressed{ false };
	bool          _ShiftKeyPressed    { false };
	bool          _CtrlKeyPressed     { false };

public:
	bool _DragDrop{ false };
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::WindowMessageHandler
{
private:
	ToolBox::ItemView* _ItemView{ nullptr };

private:
	ToolBox::Item* _Item_MousePressed   { nullptr };
	ToolBox::Item* _Item_MouseReleased  { nullptr };
	ToolBox::Item* _Item_MouseClicked   { nullptr };
	ToolBox::Item* _Item_MouseDbClicked { nullptr };
	ToolBox::Item* _Item_MouseOver      { nullptr };
	ToolBox::Item* _Item_MouseDragging  { nullptr };

private:
	bool _MouseCaptureEnabled{ true };
	bool _MouseCaptured      { false };


private:
	bool _MouseTrackEnabled{ true };
	bool _MouseTracked     { false };

private:
	std::uint64_t _MouseClickedTime{ 0   };
	std::uint64_t _MouseDbClickTime{ 300 };

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
	bool isItemViewIn(ToolBox::Item* test);
	ToolBox::Item* hitTest(const cx::gw::Point& point, ToolBox::ItemSharedPtrs& items);
	ToolBox::Item* hitTest(const cx::gw::Point& point, ToolBox::ItemSharedPtr& item);

public:
	void reset(void);
	ToolBox::Item* hitTest (const cx::gw::Point& point);

public:
	void getMouseDbClickTime (std::uint64_t& time);
	void setMouseDbClickTime (std::uint64_t time);

public:
	bool getMouseCaptureEnabled (void);
	void setMouseCaptureEnabled (bool enabled);
	void setMouseCapture        (HWND hwnd);
	void releaseMouseCapture    (void);

public:
	bool getMouseTrackEnabled (void);
	void setMouseTrackEnabled (bool enabled);
	void setMouseTrack        (HWND hwnd);
	void releaseMouseTrack    (void);

public:
	bool onWindowMessage      (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void onWindowMouseMessage (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled);

public:
	void onMouse (ToolBox::EventType eventType, ToolBox::MouseEventParam& param);

public:
	void onMouseMove        (ToolBox::MouseEventParam& param);
	void onMouseLButtonDown (ToolBox::MouseEventParam& param);
	void onMouseLButtonUp   (ToolBox::MouseEventParam& param);
	void onMouseHover       (ToolBox::MouseEventParam& param);
	void onMouseLeave       (ToolBox::MouseEventParam& param);

public:
	void notifyMouseMove        (ToolBox::MouseEventParam& param);
	void notifyMouseLButtonDown (ToolBox::MouseEventParam& param);
	void notifyMouseLButtonUp   (ToolBox::MouseEventParam& param);

	void notifyMousePressed   (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseReleased  (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseClicked   (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseDbClicked (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseOver      (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseLeave     (ToolBox::Item* item, ToolBox::MouseEventParam& param);
	void notifyMouseDragging  (ToolBox::Item* item, ToolBox::MouseEventParam& param);
};





