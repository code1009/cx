#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : 
	public cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>
{
private:
	std::unique_ptr<cx::gw::WidgetDesignerWindow> _Window;
	cx::gw::WidgetEventDragDropNotifier _WidgetEventDragDropNotifier;

public:
	explicit View(HWND parentWindowHandle);
	virtual ~View();

public:
	virtual LRESULT onWindowMessage(cx::wui::WindowMessage& windowMessage) override;

//public:
	//cx::gw::WidgetDesignerWindow* getWidgetDesignerWindow(void) { return _Window.get(); }

public:
	virtual HWND createView(HWND parentWindowHandle);

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onHScroll(cx::wui::WindowMessage& windowMessage);
	void onVScroll(cx::wui::WindowMessage& windowMessage);	
	void onMouseWheel(cx::wui::WindowMessage& windowMessage);
	void onMouseMove(cx::wui::WindowMessage& windowMessage);
	void onKeyDown(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onPaint(cx::wui::WindowMessage& windowMessage);	
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);

public:
	void onIdle(void);

public:
	void doDragDrop(std::wstring dragDropData);

public:
	void fileNew(void);
	void fileOpen(void);
	void fileSave(void);
	void fileSaveAs(void);
	void filePrint(void);
	void editUndo(void);
	void editRedo(void);
	void editCopy(void);
	void editCut(void);
	void editPaste(void);
	void editDelete(void);
	void editSelectAll(void);
	void editDeselectAll(void);
	void editSnapToGrid(void);
	void editBringToTop(void);
	void editSendToBottom(void);
	void viewDocumentGrid(void);
	void viewStatusOverlay(void);
};
