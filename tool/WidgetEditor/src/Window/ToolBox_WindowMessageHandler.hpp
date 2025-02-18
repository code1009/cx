﻿/////////////////////////////////////////////////////////////////////////////
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
	ToolBox::Item* _Item_MousePressed   { nullptr };
	ToolBox::Item* _Item_MouseReleased  { nullptr };
	ToolBox::Item* _Item_MouseClicked   { nullptr };
	ToolBox::Item* _Item_MouseDbClicked { nullptr };
	ToolBox::Item* _Item_MouseOver      { nullptr };
	ToolBox::Item* _Item_MouseDragging  { nullptr };

private:
	bool _WindowMouseCaptureEnabled{ true };
	bool _WindowMouseCaptured      { false };

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

public:
	void reset(void);
	bool isIn(ToolBox::Item* test);

public:
	ToolBox::Item* hitTest (const cx::gw::Point& point);
	ToolBox::Item* hitTest (const cx::gw::Point& point, ToolBox::ItemSharedPtrs& items);
	ToolBox::Item* hitTest (const cx::gw::Point& point, ToolBox::ItemSharedPtr& item);

public:
	void getMouseDbClickTime (std::uint64_t& time);
	void setMouseDbClickTime (std::uint64_t time);

public:
	bool getWindowMouseCaptureEnabled (void);
	void setWindowMouseCaptureEnabled (bool enabled);

public:
	void setWindowMouseCapture     (HWND hwnd);
	void releaseWindowMouseCapture (void);

public:
	bool onWindowMessage      (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void onWindowMouseMessage (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool& handled);

public:
	void onMouse (ToolBox::EventType eventType, ToolBox::MouseEventParam& param);

public:
	void onMouseMove        (ToolBox::MouseEventParam& param);
	void onMouseLButtonDown (ToolBox::MouseEventParam& param);
	void onMouseLButtonUp   (ToolBox::MouseEventParam& param);

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





