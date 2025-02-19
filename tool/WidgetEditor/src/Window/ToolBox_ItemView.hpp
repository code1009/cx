#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemView
{
private:
	ToolBox::ControlWindow* _Window{ nullptr };

	ToolBox::ItemSharedPtrs _Items;
	
	cx::gw::Point _ItemViewSize;

	cx::gw::BitmapList _BitmapList;
	cx::gw::BitmapSharedPtr _GroupItemExpandBitmap;
	cx::gw::BitmapSharedPtr _GroupItemCollapseBitmap;

	ToolBox::ItemViewDrawingSharedPtr _ItemViewDrawing;
	ToolBox::ItemDrawingSharedPtrs _ItemDrawings;

	ToolBox::ItemDrawingSharedPtr _GroupItemDrawing;
	ToolBox::ItemDrawingSharedPtr _GroupItemButtonDrawing;
	ToolBox::ItemDrawingSharedPtr _GroupItemHoverDrawing;
	
	ToolBox::ItemDrawingSharedPtr _SubItemDrawing;
	ToolBox::ItemDrawingSharedPtr _SubItemButtonDrawing;
	ToolBox::ItemDrawingSharedPtr _SubItemHoverDrawing;

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
	cx::gw::BitmapSharedPtr getGroupItemExpandBitmap(void);
	cx::gw::BitmapSharedPtr getGroupItemCollapseBitmap(void);

public:
	ToolBox::ItemDrawingSharedPtr getItemDrawing(ToolBox::ItemSharedPtr item);

public:
	void drawItems(cx::gw::Context* ctx);
	void drawItem(cx::gw::Context* ctx, ToolBox::ItemSharedPtr item);

public:
	void onItemStatusChanged(ToolBox::Item* item);

	void onMouseEvent(ToolBox::EventType eventType, ToolBox::MouseEventParam& param);
	void onMouseEvent(ToolBox::EventType eventType, ToolBox::Item* item, ToolBox::MouseEventParam& param);
	
	void onGroupItemMouseEvent(ToolBox::EventType eventType, ToolBox::GroupItem* item, ToolBox::MouseEventParam& param);
	void onGroupItemMouseClicked(ToolBox::GroupItem* item, ToolBox::MouseEventParam& param);
	void onGroupItemMouseDbClicked(ToolBox::GroupItem* item, ToolBox::MouseEventParam& param);
	void onGroupItemMouseDragging(ToolBox::GroupItem* item, ToolBox::MouseEventParam& param);

	void onSubItemMouseEvent(ToolBox::EventType eventType, ToolBox::SubItem* item, ToolBox::MouseEventParam& param);
	void onSubItemMouseClicked(ToolBox::SubItem* item, ToolBox::MouseEventParam& param);
	void onSubItemMouseDbClicked(ToolBox::SubItem* item, ToolBox::MouseEventParam& param);
	void onSubItemMouseDragging(ToolBox::SubItem* item, ToolBox::MouseEventParam& param);
};

