/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ShapeTemplate : public Shape
{
public:
	using drawShape = std::function<void(Context*, ShapeTemplate*)>;

private:
	drawShape _drawShape;

public:
	explicit ShapeTemplate(std::wstring className, drawShape draw);

public:
	virtual ~ShapeTemplate() = default;

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual WidgetSharedPtr clone(void) const override;
	virtual void draw(Context* ctx) override;

	//-----------------------------------------------------------------------
public:
	virtual void onWidgetDrawingStyleChanged(WidgetDrawingStyle::StyleChangedParam* param);

public:
	virtual WidgetFillStyle* getFillStyle(void) { return &_FillStyle; }
	virtual WidgetLineStyle* getLineStyle(void) { return &_LineStyle; }
	virtual WidgetTextStyle* getTextStyle(void) { return &_TextStyle; }

public:
	virtual WidgetResource* getFill_Brush     (void){ return _Fill_Brush     ; };
	virtual WidgetResource* getLine_Brush     (void){ return _Line_Brush     ; };
	virtual WidgetResource* getText_TextFormat(void){ return _Text_TextFormat; };
	virtual WidgetResource* getText_Brush     (void){ return _Text_Brush     ; };
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




