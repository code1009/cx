#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ControlWindow : public cx::gw::Window
{
public:
	
private:
	HWND _WindowHandle{ nullptr };
	ToolBox::ItemSharedPtrs _Items;
	std::size_t _LastMadeID{ 0 };

private:
	ID2D1SolidColorBrush* _Brush{ nullptr };

	ToolBox::ItemDrawingSharedPtrs _ItemDrawings;
	ToolBox::ItemDrawingSharedPtr _GroupItemDrawing;
	ToolBox::ItemDrawingSharedPtr _SubItemDrawing;

public:
	explicit ControlWindow(HWND hwnd);
	virtual ~ControlWindow(void) = default;

public:
	ControlWindow(const ControlWindow&) = delete;
	ControlWindow& operator=(const ControlWindow&) = delete;

	ControlWindow(ControlWindow&&) = delete;
	ControlWindow& operator=(ControlWindow&&) = delete;

	//-----------------------------------------------------------------------
	// cx::gw::Window
public:
	virtual void resize(std::int64_t width, std::int64_t height) override;
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;

	//-----------------------------------------------------------------------
	// ToolBox::ControlWindow
public:
	std::size_t makeID(void);

public:
	ToolBox::ItemSharedPtrs& getItems(void);

public:
	void addItem(ToolBox::GroupItemSharedPtr parentItem, ToolBox::ItemSharedPtr item);
	void removeItem(ToolBox::ItemSharedPtr item);
	void removeItem(ToolBox::ItemSharedPtrs& items, ToolBox::ItemSharedPtr item);
	void clearItems(void);
	void clearItems(ToolBox::ItemSharedPtrs& items);
	void clearSubItems(ToolBox::GroupItemSharedPtr& groupItem);

	ToolBox::ItemSharedPtr findItem(std::size_t id);
	ToolBox::ItemSharedPtr findItem(ToolBox::ItemSharedPtrs& items, std::size_t id);

public:
	void recalcLayout(void);
	cx::gw::coord_t recalcItemLayout(cx::gw::coord_t offset, ToolBox::ItemSharedPtr item);

public:
	ToolBox::ItemDrawingSharedPtr getItemDrawing(ToolBox::ItemSharedPtr item);

public:
	void drawItems(void);
	void drawItem(ToolBox::ItemSharedPtr item);	
};

