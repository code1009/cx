/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Widget;
class WidgetDocument;

using WidgetWeakPtr = std::weak_ptr<Widget>;
using WidgetSharedPtr = std::shared_ptr<Widget>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Widget
{
public:
	class PropertyChangedParam
	{
	public:
		Widget*       _Sender;
		std::uint32_t _Code;
		void*         _Data;
	};
	
	using PropertyChangedHandler = std::function<void(PropertyChangedParam*)>;

protected:
	std::wstring       _ClassName {};
	std::wstring       _Name      {};
	bool               _Visible   { true };
	std::vector<Point> _Points    {};

protected:
	PropertyChangedHandler _PropertyChangedHandler{};

public:
	explicit Widget(std::wstring className);

public:
	virtual ~Widget() = default;

public:
	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;

	Widget(Widget&&) = delete;
	Widget& operator=(Widget&&) = delete;

	//-----------------------------------------------------------------------
public:
	virtual void copyTo(WidgetSharedPtr widget) const;
	virtual WidgetSharedPtr clone(void) const = 0;

	//-----------------------------------------------------------------------
public:
	virtual std::wstring getClassName(void) const;
	virtual std::wstring getName(void) const;
	virtual void setName(std::wstring name);
	virtual bool getVisible(void) const;
	virtual void setVisible(bool visible);

public:
	virtual std::vector<Point>& getPoints(void);
	virtual void setPoints(std::vector<Point> points);
	virtual void setPoint(std::size_t n, Point p);
	virtual void getBounds(Point& p0, Point& p1);
	virtual void setBounds(const Point& p0, const Point& p1);
	virtual bool isPointIn(const Point& test) const;
	virtual void moveOffset(const Point& offset);

	//-----------------------------------------------------------------------
public:
	virtual void setPropertyChangedHandler(PropertyChangedHandler handler);
	virtual void setPropertyChanged(std::uint32_t code=0, void* data=nullptr);

public:
	virtual void registerEventHandler(WidgetDocument* doc);

	//-----------------------------------------------------------------------
public:
	virtual bool loadResources(WidgetResourceMap* widgetResourceMap);
	virtual void draw(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




