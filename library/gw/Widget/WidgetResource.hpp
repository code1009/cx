/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetResource
{
public:
	enum class Type
	{
		UNKNOWN,
		StrokeStyle,
		SolidColorBrush,
		TextFormat
	};

private:
	std::wstring _Id     { };
	Type         _Type   { Type::UNKNOWN };
	IUnknown*    _Pointer{ nullptr };

public:
	explicit WidgetResource(std::wstring id, Type type, IUnknown* pointer=nullptr);

public:
	virtual ~WidgetResource();

public:
	WidgetResource(const WidgetResource&) = delete;
	WidgetResource& operator=(const WidgetResource&) = delete;

	WidgetResource(WidgetResource&&) = delete;
	WidgetResource& operator=(WidgetResource&&) = delete;

public:
	void release(void);

public:
	std::wstring getId(void);
	Type         getType(void);
	IUnknown*    getPointer(void);
	void         setPointer(IUnknown* pointer);

public:
	IDWriteTextFormat*    getTextFormat(void);
	ID2D1SolidColorBrush* getSolidColorBrush(void);
	ID2D1StrokeStyle*     getStrokeStyle(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using WidgetResourceSharedPtr = std::shared_ptr<WidgetResource>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetResourceMap
{
private:
	Context* _Context{ nullptr };
	std::unordered_map < std::wstring, WidgetResourceSharedPtr > _WidgetResourceSharedPtrMap;

public:
	explicit WidgetResourceMap(Context* ctx);

public:
	virtual ~WidgetResourceMap();

public:
	WidgetResourceMap(const WidgetResourceMap&) = delete;
	WidgetResourceMap& operator=(const WidgetResourceMap&) = delete;

	WidgetResourceMap(WidgetResourceMap&&) = delete;
	WidgetResourceMap& operator=(WidgetResourceMap&&) = delete;

public:
	virtual void discard(void);
	virtual void clear(void);

protected:
	virtual WidgetResourceSharedPtr registerWidgetResource(std::wstring id, WidgetResource::Type type);

public:
	WidgetResource* getWidgetFillStyle_SolidColorBrush(WidgetFillStyle* style);
	WidgetResource* getWidgetLineStyle_SolidColorBrush(WidgetLineStyle* style);
	WidgetResource* getWidgetLineStyle_StrokeStyle    (WidgetLineStyle* style);
	WidgetResource* getWidgetTextStyle_SolidColorBrush(WidgetTextStyle* style);
	WidgetResource* getWidgetTextStyle_TextFormat     (WidgetTextStyle* style);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




