#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemView
{
private:
	ToolBox::ControlWindow* _Window{ nullptr };
	ToolBox::ItemSharedPtrs _Items;
	std::size_t _LastMadeID{ 0 };
	cx::gw::Point _ItemViewSize;

private:
	ToolBox::ItemDrawingSharedPtrs _ItemDrawings;
	ToolBox::ItemDrawingSharedPtr _GroupItemDrawing;
	ToolBox::ItemDrawingSharedPtr _SubItemDrawing;

public:
	explicit ItemView(ToolBox::ControlWindow* window);
	virtual ~ItemView(void) = default;

public:
	ItemView(const ItemView&) = delete;
	ItemView& operator=(const ItemView&) = delete;

	ItemView(ItemView&&) = delete;
	ItemView& operator=(ItemView&&) = delete;

public:
	ToolBox::ControlWindow* getWindow(void);

public:
	cx::gw::Point getItemViewSize(void);

public:
	bool createDeviceResources(void);
	void destroyDeviceResources(void);
	void draw(void);

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
	cx::gw::coord_t recalcItemLayout(cx::gw::coord_t offset, cx::gw::coord_t size, ToolBox::ItemSharedPtr item);

public:
	ToolBox::ItemDrawingSharedPtr getItemDrawing(ToolBox::ItemSharedPtr item);

public:
	void drawItems(void);
	void drawItem(ToolBox::ItemSharedPtr item);	
};

