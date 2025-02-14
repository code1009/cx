#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ControlWindow;

class ToolBox::GroupItem;
class ToolBox::SubItem;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::Item
{
private:
	std::size_t _ID{ 0 };
	std::wstring _Caption{ };
	std::wstring _Description{ };
	std::wstring _Icon{ };
	cx::gw::coord_t _Size{ 0 };

private:
	ToolBox::GroupItemWeakPtr _ParentItem{ };
	std::size_t _Depth{ 0 };

private:
	cx::gw::Point _p0{ };
	cx::gw::Point _p1{ };

public:
	Item(void) = default;
	virtual ~Item(void) = default;

public:
	Item(const Item&) = delete;
	Item& operator=(const Item&) = delete;

	Item(Item&&) = delete;
	Item& operator=(Item&&) = delete;

public:
	std::size_t getID(void) const;
	std::wstring getCaption(void) const;
	std::wstring getDescription(void) const;
	std::wstring getIcon(void) const;
	cx::gw::coord_t getSize(void) const;

	void setID(std::size_t id);
	void setCaption(const std::wstring& caption);
	void setDescription(const std::wstring& description);
	void setIcon(const std::wstring& icon);
	void setSize(const cx::gw::coord_t size);

public:
	ToolBox::GroupItemWeakPtr getParentItem(void) const;
	void setParentItem(ToolBox::GroupItemWeakPtr parentItem);

	std::size_t getDepth(void) const;
	void setDepth(const std::size_t depth);

	ToolBox::GroupItemWeakPtr getRootItem(void) const;

public:
	cx::gw::coord_t getCX(void) const;
	cx::gw::coord_t getCY(void) const;
	cx::gw::coord_t getX(void) const;
	cx::gw::coord_t getY(void) const;
	void setCX(cx::gw::coord_t cx);
	void setCY(cx::gw::coord_t cy);
	void setX(cx::gw::coord_t x);
	void setY(cx::gw::coord_t y);

	void setP0(const cx::gw::Point& p0);
	void setP1(const cx::gw::Point& p1);
	cx::gw::Point getP0(void) const;
	cx::gw::Point getP1(void) const;

	virtual cx::gw::coord_t calcHeight(void);
	virtual cx::gw::coord_t calcWidth(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::GroupItem : public ToolBox::Item
{
private:
	bool _CollapseSubItems { false };
	ToolBox::ItemSharedPtrs _SubItems;

public:
	GroupItem(void) = default;
	virtual ~GroupItem(void) = default;

public:
	virtual cx::gw::coord_t calcHeight(void) override;
	virtual cx::gw::coord_t calcWidth(void) override;

public:
	ToolBox::ItemSharedPtrs& getSubItems(void);
	void CollapseSubItems(bool collapse);
	bool isCollapseSubItems(void) const;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::SubItem : public ToolBox::Item
{
public:
	SubItem(void) = default;
	virtual ~SubItem(void) = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::GroupItemSharedPtr makeGroupItem(
	std::size_t id,
	std::wstring caption,
	std::wstring description = std::wstring(),
	std::wstring icon = std::wstring(),
	cx::gw::coord_t size = 24.0f
);

ToolBox::SubItemSharedPtr makeSubItem(
	std::size_t id,
	std::wstring caption,
	std::wstring description = std::wstring(),
	std::wstring icon = std::wstring(),
	cx::gw::coord_t size = 24.0f
);




