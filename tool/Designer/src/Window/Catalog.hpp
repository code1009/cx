#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Designer;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CatalogInfo
{
public:
	std::wstring type;
	std::wstring label;
	std::wstring name;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Catalog
{
	//-----------------------------------------------------------------------
private:
	Designer* _Designer{ nullptr };

public:
	std::vector<CatalogInfo> _Items;

	//-----------------------------------------------------------------------
public:
    explicit Catalog(Designer* designer) :
		_Designer(designer)
	{
	}

public:
    virtual ~Catalog() = default;

	//-----------------------------------------------------------------------
public:
    Catalog(const Catalog& other) = delete;
    Catalog& operator=(const Catalog& other) = delete;
    Catalog(Catalog&& other) noexcept = delete;
    Catalog& operator=(Catalog&& other) noexcept = delete;

	//-----------------------------------------------------------------------
public:
	void addLabel(std::wstring Label);
	void addSpare();
	void addItem(
		std::shared_ptr<cx::Widget::Item> const& item, 
		cx::Widget::ClassInfo::MakePropertiesFunction const& makeProperties, 
		cx::Widget::StringView const& friendlyName
	);
};
