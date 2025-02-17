#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemViewDrawing
{
public:
	ID2D1SolidColorBrush* _pFrame_FillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrame_LineBrush{ nullptr };

public:
	ItemViewDrawing(void) = default;
	virtual ~ItemViewDrawing(void) = default;

public:
	virtual bool createDeviceResources(cx::gw::Context* ctx);
	virtual void destroyDeviceResources(void);

public:
	virtual void drawItemView(cx::gw::Context* ctx, ToolBox::ItemView* itemView);

public:
	virtual void drawFrame(cx::gw::Context* ctx, ToolBox::ItemView* itemView);

public:
	virtual void getFrame_FillColor(cx::gw::Color& color);
	virtual void getFrame_LineColor(cx::gw::Color& color);
	virtual void getFrame_LineSize(cx::gw::coord_t& size);

public:
	virtual void getFrame_Bounds(ToolBox::ItemView* itemView, cx::gw::Point& p0, cx::gw::Point& p1);
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemDrawing
{
public:
	ID2D1SolidColorBrush* _pFrame_FillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrame_LineBrush{ nullptr };

	ID2D1SolidColorBrush* _pFace_FillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFace_LineBrush{ nullptr };

	ID2D1SolidColorBrush* _pFace_Button_FillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFace_Button_H_LineBrush{ nullptr };
	ID2D1SolidColorBrush* _pFace_Button_S_LineBrush{ nullptr };

	IDWriteTextFormat* _pCaption_TextFormat{ nullptr };
	ID2D1SolidColorBrush* _pCaption_TextBrush{ nullptr };

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
	virtual void drawFace(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawFaceButton(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawIcon(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);
	virtual void drawCaption(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);

public:
	virtual void getFrame_FillColor(cx::gw::Color& color);
	virtual void getFrame_LineColor(cx::gw::Color& color);
	virtual void getFrame_LineSize(cx::gw::coord_t& size);

	virtual void getFace_FillColor(cx::gw::Color& color);
	virtual void getFace_LineColor(cx::gw::Color& color);
	virtual void getFace_LineSize(cx::gw::coord_t& size);

	virtual void getFace_Button_FillColor(cx::gw::Color& color);
	virtual void getFace_Button_H_LineColor(cx::gw::Color& color);
	virtual void getFace_Button_S_LineColor(cx::gw::Color& color);
	virtual void getFace_Button_LineSize(cx::gw::coord_t& size);

	virtual void getCaption_TextColor(cx::gw::Color& color);
	virtual void getCaption_TextFontBold(bool& bold);

public:
	virtual void getFrame_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);
	virtual void getFace_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);
	virtual void getFace_Button_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1);
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
	virtual void drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item) override;
	virtual void getFace_FillColor(cx::gw::Color& color) override;
	virtual void getCaption_TextFontBold(bool& bold) override;

public:
	virtual void drawSubItems(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::SubItemDrawing : public ToolBox::ItemDrawing
{
public:
	SubItemDrawing(void) = default;
	virtual ~SubItemDrawing(void) = default;

public:
	virtual void drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item) override;
	virtual void getFace_FillColor(cx::gw::Color& color) override;
	virtual void getFace_LineSize(cx::gw::coord_t& size) override;
};
