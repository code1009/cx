#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Designer
{
	//-----------------------------------------------------------------------
private:
	HWND _Hwnd{ nullptr };

private:
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Widget::Edit> _Edit;
	std::unique_ptr<cx::Widget::PropertiesManipulator> _PropertiesManipulator;

private:
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;
	std::unique_ptr<cx::wui::dragdrop::WindowDropTargetHandler> _DropTargetHandler;

public:
	std::function<void(void)> showItemPropertytHandler{ nullptr };
	std::function<void(void)> modifiedHandler{ nullptr };

private:
	std::unique_ptr<Catalog> _Catalog;
	std::vector<std::wstring> _FontFamilies;

private:
	std::wstring _FilePath;

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
public:
	cx::d2d::Canvas* canvas(void) { return _Canvas.get(); }
	cx::Widget::Edit* edit(void) { return _Edit.get(); }
	cx::Widget::PropertiesManipulator* propertiesManipulator(void) { return _PropertiesManipulator.get(); }
	cx::wui::WindowMouseHandler* mouseHandler(void) { return _MouseHandler.get(); }
	cx::wui::WindowScrollHandler* scrollHandler(void) { return _ScrollHandler.get(); }

public:
	auto catalog() { return _Catalog.get(); }
	auto const& fontFamilies() const { return _FontFamilies; }
	auto filePath() const { return _FilePath; }
	auto isModified() const { return _Edit->editSeed().isModified(); }

	//-----------------------------------------------------------------------
private:
	void setupCanvasView(void);
	void setupMouseHandlerHandler(void);
	void setupScrollHandler(void);
	void setupDropTargetHandler(void);

	void loadCatalog(void);
	void loadFontFamilies(void);

	//-----------------------------------------------------------------------
public:
	void resize(std::uint32_t cx, std::uint32_t cy);
	void invalidate(void);

private:
	void updateScrollBar(void);

	//-----------------------------------------------------------------------
public:
	bool onWindowMessage(cx::wui::WindowMessage& windowMessage);

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
