/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetEventWindowMessageHandler
{
private:
	WidgetDocument* _WidgetDocument{ nullptr };
	Viewport*       _Viewport      { nullptr };

private:
	Widget* _Widget_MousePressed   { nullptr };
	Widget* _Widget_MouseReleased  { nullptr };
	Widget* _Widget_MouseClicked   { nullptr };
	Widget* _Widget_MouseDbClicked { nullptr };
	Widget* _Widget_MouseOver      { nullptr };
	Widget* _Widget_MouseDragging  { nullptr };

private:
	std::uint64_t _MouseClickedTime { 0   };
	std::uint64_t _MouseDbClickTime { 300 };

private:
	bool _MouseCaptureEnabled { true  };
	bool _MouseCaptured       { false };

private:
	bool _MouseTrackEnabled { true  };
	bool _MouseTracked      { false };

public:
	WidgetEventWindowMessageHandler() = default;

public:
	virtual ~WidgetEventWindowMessageHandler() = default;

public:
	WidgetEventWindowMessageHandler(const WidgetEventWindowMessageHandler&) = delete;
	WidgetEventWindowMessageHandler& operator=(const WidgetEventWindowMessageHandler&) = delete;

	WidgetEventWindowMessageHandler(WidgetEventWindowMessageHandler&&) = delete;
	WidgetEventWindowMessageHandler& operator=(WidgetEventWindowMessageHandler&&) = delete;

public:
	void setWidgetDocument(WidgetDocument* doc);
	void setViewport(Viewport* viewport);
	bool isWidgetDocumentIn(Widget* test);

public:
	Widget* hitTest (const cx::gw::Point& point);
	void update(void);

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
	void onMouse (WidgetEventType eventType, WidgetMouseEventParam& param);

public:
	void onMouseMove        (WidgetMouseEventParam& param);
	void onMouseLButtonDown (WidgetMouseEventParam& param);
	void onMouseLButtonUp   (WidgetMouseEventParam& param);

	void onMouseHover       (WidgetMouseEventParam& param);
	void onMouseLeave       (WidgetMouseEventParam& param);

public:
	void notifyMouseMove        (WidgetMouseEventParam& param);
	void notifyMouseLButtonDown (WidgetMouseEventParam& param);
	void notifyMouseLButtonUp   (WidgetMouseEventParam& param);

	void notifyMousePressed   (Widget* widget, WidgetMouseEventParam& param);
	void notifyMouseReleased  (Widget* widget, WidgetMouseEventParam& param);
	void notifyMouseClicked   (Widget* widget, WidgetMouseEventParam& param);
	void notifyMouseDbClicked (Widget* widget, WidgetMouseEventParam& param);
	void notifyMouseOver      (Widget* widget, WidgetMouseEventParam& param);
	void notifyMouseLeave     (Widget* widget, WidgetMouseEventParam& param);
	void notifyMouseDragging  (Widget* widget, WidgetMouseEventParam& param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





