#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemDrawing
{
public:
	ID2D1SolidColorBrush* _pFrameFillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrameLineBrush{ nullptr };

	IDWriteTextFormat* _pCaptionTextFormat{ nullptr };
	ID2D1SolidColorBrush* _pCaptionTextBrush{ nullptr };

public:
	ItemDrawing(void) = default;
	virtual ~ItemDrawing(void) = default;

public:
	virtual bool createDeviceResources(ToolBox::ItemView* itemView);
	virtual void destroyDeviceResources(void);

public:
	virtual void draw(ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawFrame(ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawCaption(ToolBox::ItemView* itemView, ToolBox::Item* item);

public:
	virtual void getFrame_FillColor(cx::gw::Color& color);
	virtual void getFrame_LineColor(cx::gw::Color& color);
	virtual void getFrame_LineSize(cx::gw::coord_t& size);
	virtual void getCaption_TextColor(cx::gw::Color& color);

public:
	virtual void getFrame_Bounds(ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);
	virtual void getCaption_Bounds(ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);

public:
	virtual void getIndentSpace_Size(ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::coord_t& size);
	virtual void getDepth_Size(ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::coord_t& size);
	virtual void getIcon_Size(ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::coord_t& size);
	virtual void getIconSpace_Size(ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::coord_t& size);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::GroupItemDrawing : public ToolBox::ItemDrawing
{
public:
	GroupItemDrawing(void) = default;
	virtual ~GroupItemDrawing(void) = default;

public:
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::SubItemDrawing : public ToolBox::ItemDrawing
{
public:
	SubItemDrawing(void) = default;
	virtual ~SubItemDrawing(void) = default;
};
