#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Designer
{
public:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Widget::PropertiesManipulator> _PropertiesManipulator;
	std::unique_ptr<cx::Widget::Edit> _Edit;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;
	std::unique_ptr<cx::wui::dragdrop::WindowDropTargetHandler> _DropTargetHandler;

private:
	std::vector<std::wstring> _FontFamilies;
	std::wstring _FilePath;

public:
	explicit Designer(HWND hwnd);
	virtual ~Designer();

public:
	Designer(const Designer& other) = delete;
	Designer& operator=(const Designer& other) = delete;
	Designer(Designer&& other) noexcept = delete;
	Designer& operator=(Designer&& other) noexcept = delete;

public:
	void resize(std::uint32_t cx, std::uint32_t cy);
	void invalidate(void);

public:
	void updateScrollBar(void);

public:
	void newFile(void);
	void openFile(void);
	void saveFile(void);
	void saveFileAs(void);

public:
	void undo(void);
	void redo(void);
	void cut(void);
	void copy(void);
	void paste(void);
	void selectAll(void);
	void deselectAll(void);
	void erase(void);

public:
	void zoomIn(float px, float py);
	void zoomOut(float px, float py);

public:
	void bringToFront(void);
	void sendToBack(void);
	void bringToTop(void);
	void sendToBottom(void);

public:
	void setFileProperties(void);

public:
	void snapToGrid(bool enable);

public:
	void showGrid(bool show);
	void showGridCoord(bool show);
	void showStatus(bool show);

public:
	void onFile_New             (void);
	void onFile_Open            (void);
	void onFile_Save            (void);
	void onFile_SaveAs          (void);
	void onEdit_Undo            (void);
	void onEdit_Redo            (void);
	void onEdit_Cut             (void);
	void onEdit_Copy            (void);
	void onEdit_Paste           (void);
	void onEdit_SelectAll       (void);
	void onEdit_DeselectAll     (void);
	void onEdit_Delete          (void);
	void onDesign_ViewCommand   (void);
	void onDesign_ViewProperty  (void);
	void onDesign_ZoomIn        (void);
	void onDesign_ZoomOut       (void);
	void onDesign_BringToFront  (void);
	void onDesign_SendToBack    (void);
	void onDesign_BringToTop    (void);
	void onDesign_SendToBottom  (void);
	void onDesign_FileProperties(void);
	void onDesign_SnapToGrid    (void);
	void onDesign_ShowGrid      (void);
	void onDesign_ShowGridCoord (void);
	void onDesign_ShowStatus    (void);
};
