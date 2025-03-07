#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemStatus
{
public:
	using StatusChangedHandler = std::function<void(void)>;

private:
	StatusChangedHandler _StatusChangedHandler;

private:
	bool _Hover{ false };
	bool _Pressed{ false };
	bool _Disabled{ false };
	bool _Selected{ false };

public:
	ItemStatus(void) = default;
	virtual ~ItemStatus(void) = default;

public:
	ItemStatus(const ItemStatus&) = delete;
	ItemStatus& operator=(const ItemStatus&) = delete;
	ItemStatus(ItemStatus&&) = delete;
	ItemStatus& operator=(ItemStatus&&) = delete;

public:
	void setStatusChanged(void);
	void setStatusChangedHandler(StatusChangedHandler handler);

public:
	bool getHover(void) const;
	void setHover(bool hover);
	bool getPressed(void) const;
	void setPressed(bool pressed);
	bool getDisabled(void) const;
	void setDisabled(bool disabled);
	bool getSelected(void) const;
	void setSelected(bool selected);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::Item
{
private:
	std::size_t        _Id         { 0 };
	std::wstring       _Caption    { };
	std::wstring       _Icon       { };
	ToolBox::ItemStyle _Style      { ToolBox::ItemStyle::Flat };
	cx::gw::coord_t    _Size       { 0 };
	std::wstring       _Description{ };

private:
	ToolBox::ItemStatus _Status{ };

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
	virtual std::size_t getId(void) const;
	virtual std::wstring getCaption(void) const;
	virtual std::wstring getIcon(void) const;
	virtual ToolBox::ItemStyle getStyle(void) const;
	virtual cx::gw::coord_t getSize(void) const;
	virtual std::wstring getDescription(void) const;

	virtual void setId(std::size_t id);
	virtual void setCaption(const std::wstring& caption);
	virtual void setIcon(const std::wstring& icon);
	virtual void setSize(const cx::gw::coord_t size);
	virtual void setStyle(ToolBox::ItemStyle style);
	virtual void setDescription(const std::wstring& description);

public:
	virtual ToolBox::ItemStatus* getStatus(void);

public:
	virtual ToolBox::GroupItemWeakPtr getParentItem(void) const;
	virtual void setParentItem(ToolBox::GroupItemWeakPtr parentItem);

	virtual std::size_t getDepth(void) const;
	virtual void setDepth(const std::size_t depth);

	virtual ToolBox::GroupItemWeakPtr getRootItem(void) const;

public:
	virtual cx::gw::coord_t getCX(void) const;
	virtual cx::gw::coord_t getCY(void) const;
	virtual cx::gw::coord_t getX(void) const;
	virtual cx::gw::coord_t getY(void) const;
	virtual void setCX(cx::gw::coord_t cx);
	virtual void setCY(cx::gw::coord_t cy);
	virtual void setX(cx::gw::coord_t x);
	virtual void setY(cx::gw::coord_t y);

	virtual void setP0(const cx::gw::Point& p0);
	virtual void setP1(const cx::gw::Point& p1);
	virtual cx::gw::Point getP0(void) const;
	virtual cx::gw::Point getP1(void) const;

public:
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

public:
	virtual std::size_t getDepth(void) const override
	{
		if (ToolBox::Item::getDepth()>0)
		{
			return ToolBox::Item::getDepth() - 1;
		}

		return 0;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::GroupItemSharedPtr makeGroupItem(
	std::size_t id,
	std::wstring caption,
	std::wstring icon = std::wstring(),
	ToolBox::ItemStyle style = ToolBox::ItemStyle::Flat,
	cx::gw::coord_t size = 28.0f,
	std::wstring description = std::wstring()
);

ToolBox::SubItemSharedPtr makeSubItem(
	std::size_t id,
	std::wstring caption,
	std::wstring icon = std::wstring(),
	ToolBox::ItemStyle style = ToolBox::ItemStyle::Flat,
	cx::gw::coord_t size = 28.0f,
	std::wstring description = std::wstring()
);




