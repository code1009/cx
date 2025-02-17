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
	cx::gw::BitmapList _BitmapList;
	ToolBox::ItemViewDrawingSharedPtr _ItemViewDrawing;
	ToolBox::ItemDrawingSharedPtrs _ItemDrawings;
	ToolBox::ItemDrawingSharedPtr _GroupItemDrawing;
	ToolBox::ItemDrawingSharedPtr _SubItemDrawing;

public:
	explicit ItemView(ToolBox::ControlWindow* window);
	virtual ~ItemView(void);

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
	bool createDeviceResources(cx::gw::Context* ctx);
	void destroyDeviceResources(void);
	void draw(cx::gw::Context* ctx);

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
	cx::gw::BitmapList* getBitmapList(void);

public:
	ToolBox::ItemDrawingSharedPtr getItemDrawing(ToolBox::ItemSharedPtr item);

public:
	void drawItems(cx::gw::Context* ctx);
	void drawItem(cx::gw::Context* ctx, ToolBox::ItemSharedPtr item);

public:
	void onItemStatusChanged(ToolBox::Item* item);
};

