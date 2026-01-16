#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Designer
{
	//-----------------------------------------------------------------------
public:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Widget::PropertiesManipulator> _PropertiesManipulator;
	std::unique_ptr<cx::Widget::Edit> _Edit;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;
	std::unique_ptr<cx::wui::dragdrop::WindowDropTargetHandler> _DropTargetHandler;

	std::function<void(void)> _ShowItemPropertytHandler{ nullptr };

private:
	std::unique_ptr<Catalog> _Catalog;

private:
	std::wstring _FilePath;

private:
	std::vector<std::wstring> _FontFamilies;

	//-----------------------------------------------------------------------
public:
	explicit Designer(HWND hwnd);
	virtual ~Designer();

	//-----------------------------------------------------------------------
public:
	Designer(const Designer& other) = delete;
	Designer& operator=(const Designer& other) = delete;
	Designer(Designer&& other) noexcept = delete;
	Designer& operator=(Designer&& other) noexcept = delete;

	//-----------------------------------------------------------------------
private:
	void setupCanvasView(void);
	void setupMouseHandlerHandler(void);
	void setupScrollHandler(void);
	void setupDropTargetHandler(void);

	//-----------------------------------------------------------------------
private:
	void loadCatalog(void);
public:
	auto catalog() { return _Catalog.get(); }

	//-----------------------------------------------------------------------
private:
	void loadFontFamilies(void);
public:
	auto const& fontFamilies() const { return _FontFamilies; }

	//-----------------------------------------------------------------------
public:
	void resize(std::uint32_t cx, std::uint32_t cy);
	void invalidate(void);

private:
	void updateScrollBar(void);

	//-----------------------------------------------------------------------
	// commands
	//-----------------------------------------------------------------------
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
	void zoomIn(void);
	void zoomOut(void);

public:
	void bringToFront(void);
	void sendToBack(void);
	void bringToTop(void);
	void sendToBottom(void);

public:
	void setViewProperties(void);

public:
	void snapToGrid(bool enable);

public:
	void showGrid(bool show);
	void showGridCoord(bool show);
	void showStatus(bool show);

public:
	void showItemProperty(std::shared_ptr<cx::Widget::Item> const& item);
};
