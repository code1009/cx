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
	virtual bool createDeviceResources(cx::gw::Context* ctx);
	virtual void destroyDeviceResources(void);

public:
	virtual void drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);

public:
	virtual void drawFrame(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawIcon(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawCaption(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);

public:
	virtual void getFrame_FillColor(cx::gw::Color& color);
	virtual void getFrame_LineColor(cx::gw::Color& color);
	virtual void getFrame_LineSize(cx::gw::coord_t& size);
	virtual void getCaption_TextColor(cx::gw::Color& color);
	virtual void getCaption_TextFontBold(bool& bold);

public:
	virtual void getFrame_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);
	virtual void getIcon_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);
	virtual void getCaption_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);

public:
	virtual void getIndentSpace_Size(ToolBox::Item* item, cx::gw::coord_t& size);
	virtual void getDepth_Size(ToolBox::Item* item, cx::gw::coord_t& size);
	virtual void getIcon_Size(ToolBox::Item* item, cx::gw::coord_t& size);
	virtual void getIconSpace_Size(ToolBox::Item* item, cx::gw::coord_t& size);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::GroupItemDrawing : public ToolBox::ItemDrawing
{
public:
	GroupItemDrawing(void) = default;
	virtual ~GroupItemDrawing(void) = default;

public:
	virtual void getCaption_TextFontBold(bool& bold) override
	{
		bold = true;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::SubItemDrawing : public ToolBox::ItemDrawing
{
public:
	SubItemDrawing(void) = default;
	virtual ~SubItemDrawing(void) = default;

public:
	virtual void getFrame_FillColor(cx::gw::Color& color) override
	{
		color = cx::gw::Color(1.00f, 1.00f, 1.00f, 1.0f);
	}
};
