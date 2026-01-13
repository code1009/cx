#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using UICoord = cx::Widget::Coord;
constexpr UICoord UICoordZero = static_cast<cx::Widget::Coord>(0.0f);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class UILayoutAlignment
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
	UILayoutAlignment _ColAlignment{ UILayoutAlignment::Fill };
	UILayoutAlignment _RowAlignment{ UILayoutAlignment::Fixed };

public:
	UILayoutStyle() = default;
	virtual ~UILayoutStyle() = default;

public:
	UILayoutStyle(
		UICoord cx, UICoord cy, 
		UILayoutAlignment colAlignment = UILayoutAlignment::Fill,
		UILayoutAlignment rowAlignment = UILayoutAlignment::Fixed
	) :
		_CX(cx), _CY(cy), _ColAlignment(colAlignment), _RowAlignment(rowAlignment)
	{
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UILayoutItem
{
public:
	std::uint32_t _Flag{ 0 };
	void* _Data{ nullptr };
	UICoord _L{ UICoordZero };
	UICoord _T{ UICoordZero };
	UICoord _R{ UICoordZero };
	UICoord _B{ UICoordZero };

public:
	UILayoutItem() = default;
	virtual ~UILayoutItem() = default;

public:
	UILayoutItem(std::uint32_t flag, void* data) :
		_Flag(flag), _Data(data)
	{
	}
	UILayoutItem(void* data, std::uint32_t flag = 0) :
		_Flag(flag), _Data(data)
	{
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UILayout
{
public:
	std::function<void(UILayout* uiLayout)> layoutChangedHandler;

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
	UILayoutAlignment _ColAlignment{ UILayoutAlignment::Fixed };
	UILayoutAlignment _RowAlignment{ UILayoutAlignment::Fixed };
	std::size_t _ColFixedCount{ 0 };
	UICoord  _ColFixedCX{ UICoordZero };
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
private:
	UIGridLayout _Grid{};

private:
	UICoord _ViewCX{ UICoordZero };
	UICoord _ViewCY{ UICoordZero };
	UICoord _RemainCX{ UICoordZero };
	UICoord _RemainCY{ UICoordZero };

private:
	UICoord _X{ UICoordZero };
	UICoord _Y{ UICoordZero };
	UICoord _RowCY{ UICoordZero };
	UICoord _RowCX{ UICoordZero };
	UICoord _CX{ UICoordZero };
	UICoord _CY{ UICoordZero };

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
	void clear(void);
	void add(
		bool firstCol, 
		UILayoutStyle const& style, 
		std::function<void(UILayout*)> layoutChangedHandler = nullptr, 
		void* data = nullptr, std::uint32_t itemFlag = 0
	);

	//-----------------------------------------------------------------------
private:
	void updateRow(UIRowLayout& row);
	void updateGrid(void);

private:
	void recalcItemLayout(UILayout& item, UICoord x, UICoord y, UICoord cx, UICoord cy);
	void recalcRowLayout(UIRowLayout& row);

public:
	void recalcLayout(UICoord cx, UICoord cy);
};
