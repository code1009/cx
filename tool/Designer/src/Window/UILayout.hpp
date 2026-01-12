#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using UICoord = cx::Widget::Coord;
constexpr UICoord UICoordZero = static_cast<cx::Widget::Coord>(0.0f);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class UIColStyle
{
	Fixed,
	Fill
};

enum class UIRowStyle
{
	Fixed,
	Fill,
	Near,
	Center,
	Far,
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UILayoutStyle
{
public:
	UICoord _CX{ UICoordZero };
	UICoord _CY{ UICoordZero };

public:
	UIColStyle _ColStyle{ UIColStyle::Fill };
	UIRowStyle _RowStyle{ UIRowStyle::Fixed };

public:
	UILayoutStyle() = default;
	virtual ~UILayoutStyle() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UILayoutItem
{
public:
	void* item{ nullptr };
	UICoord _L{ UICoordZero };
	UICoord _T{ UICoordZero };
	UICoord _R{ UICoordZero };
	UICoord _B{ UICoordZero };
	

public:
	UILayoutItem() = default;
	virtual ~UILayoutItem() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UILayout
{
public:
	UILayoutStyle _Style{ };
	UILayoutItem _Item{ };

public:
	UILayout() = default;
	virtual ~UILayout() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UIRowLayout
{
public:
	std::vector<UILayout> _Cols;
	UICoord _MinCX{ UICoordZero };
	UICoord _MinCY{ UICoordZero };
	UIRowStyle _RowStyle{ UIRowStyle::Fixed };
	UIColStyle _ColStyle{ UIColStyle::Fixed };
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UIGridLayout
{
public:
	std::vector<UIRowLayout> _Rows;
	UICoord _MinCX{ UICoordZero };
	UICoord _MinCY{ UICoordZero };
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UILayoutManager
{
	//-----------------------------------------------------------------------
public:
	UIGridLayout _Grid;
	UICoord _X;
	UICoord _Y;
	UICoord _ViewCX;
	UICoord _ViewCY;
	UICoord _RemainingViewCX{ UICoordZero };
	UICoord _RemainingViewCY{ UICoordZero };

	UICoord _RowCY;

	UICoord _CX;
	UICoord _CY;

	//-----------------------------------------------------------------------
public:
	UILayoutManager() = default;
	virtual ~UILayoutManager() = default;

	//-----------------------------------------------------------------------
public:
	UILayoutManager(const UILayoutManager& other) = delete;
	UILayoutManager& operator=(const UILayoutManager& other) = delete;
	UILayoutManager(UILayoutManager&& other) noexcept = delete;
	UILayoutManager& operator=(UILayoutManager&& other) noexcept = delete;

	//-----------------------------------------------------------------------
public:
	void add(void* item, UILayoutStyle const& style, bool first = false);

	//-----------------------------------------------------------------------
public:
	void recalcLayout(UICoord cx, UICoord cy);

public:
	void recalcRowLayout(UIRowLayout& row);

private:
	void updateGrid(void);
	void updateRow(UIRowLayout& row);
};
