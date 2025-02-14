#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox::ItemDrawing
{
public:
	IDWriteTextFormat* _pCaptionTextFormat{ nullptr };
	ID2D1SolidColorBrush* _pCaptionTextBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrameFillBrush{ nullptr };
	ID2D1SolidColorBrush* _pFrameLineBrush{ nullptr };

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
