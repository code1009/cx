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
	bool         _IsDeviceResource { false };
	std::wstring _Id               { };
	Type         _Type             { Type::UNKNOWN };
	IUnknown*    _Pointer          { nullptr };

public:
	explicit WidgetResource(
		std::wstring id, 
		Type type, 
		IUnknown* pointer=nullptr
	);

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
	bool         IsDeviceResource (void);
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
bool isDeviceResource(WidgetResource::Type type);





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
	virtual void discardDeviceResources(void);

protected:
	virtual WidgetResourceSharedPtr registerWidgetResource(std::wstring id, WidgetResource::Type type);

public:
	WidgetResource* getFill_SolidColorBrush(WidgetFillStyle* style);
	WidgetResource* getLine_SolidColorBrush(WidgetLineStyle* style);
	WidgetResource* getLine_StrokeStyle    (WidgetLineStyle* style);
	WidgetResource* getText_SolidColorBrush(WidgetTextStyle* style);
	WidgetResource* getText_TextFormat     (WidgetTextStyle* style);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




