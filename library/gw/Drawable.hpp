/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Window;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Drawable
{
public:
	using VisibleChangedHandlerParam = Drawable*;
	using VisibleChangedHandler = std::function<void(VisibleChangedHandlerParam)>;

private:
	VisibleChangedHandler _VisibleChangedHandler;
	bool _Visible{ false };

public:
	Drawable() = default;

public:
	virtual ~Drawable() = default;

public:
	Drawable(const Drawable&) = delete;
	Drawable& operator=(const Drawable&) = delete;
	Drawable(Drawable&&) = delete;
	Drawable& operator=(Drawable&&) = delete;

public:
	virtual bool createDeviceResources(Context* ctx) = 0;
	virtual void destroyDeviceResources(void) = 0;
	virtual void draw(Context* ctx) = 0;

public:
	virtual bool getVisible(void) const;
	virtual bool setVisible(bool value);
	virtual void setVisibleChanged(void);
	virtual void setVisibleChangedHandler(VisibleChangedHandler handler);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DocumentGrid : public Drawable
{
private:
	Viewport* _Viewport{ nullptr };

private:
	ID2D1SolidColorBrush* _pDocumentGrid0_LineBrush{ nullptr };
	ID2D1SolidColorBrush* _pDocumentGrid1_LineBrush{ nullptr };
	ID2D1SolidColorBrush* _pDocumentGrid2_LineBrush{ nullptr };

private:
	//IDWriteTextLayout*    _pCoordTextLayout{ nullptr };
	IDWriteTextFormat*    _pCoord_TextFormat{ nullptr };
	ID2D1SolidColorBrush* _pCoord_TextBrush{ nullptr };

public:
	explicit DocumentGrid(Viewport* viewport);

public:
	virtual ~DocumentGrid();

	//-----------------------------------------------------------------------
	// Drawable
public:
	virtual bool createDeviceResources(Context* ctx) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(Context* ctx) override;

	//-----------------------------------------------------------------------
	// DocumentGrid
private:
	void drawDocumentGrid(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class StatusOverayPanel : public Drawable
{
private:
	Viewport* _Viewport{ nullptr };

private:
	std::chrono::time_point<std::chrono::steady_clock> _LastDrawTime;
	std::int64_t                                       _FrameDrawCount{ 0 };
	float                                              _FPS{ 0.0f };

private:
	std::int64_t _Mouse_X{ 0 };
	std::int64_t _Mouse_Y{ 0 };

private:
	//IDWriteTextLayout*    _pStatusTextLayout{ nullptr };
	IDWriteTextFormat*    _pStatus_TextFormat{ nullptr };
	ID2D1SolidColorBrush* _pStatus_TextBrush{ nullptr };
	ID2D1SolidColorBrush* _pStatusBox_FillBrush{ nullptr };
	ID2D1SolidColorBrush* _pStatusBox_LineBrush{ nullptr };

	//-----------------------------------------------------------------------
public:
	explicit StatusOverayPanel(Viewport* viewport);

public:
	virtual ~StatusOverayPanel();

	//-----------------------------------------------------------------------
	// Drawable
public:
	virtual bool createDeviceResources(Context* ctx) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(Context* ctx) override;

	//-----------------------------------------------------------------------
	// StatusOverayPanel
private:
	void drawStatusOverayPanel(Context* ctx);

public:
	void calculateFPS(void);
	void setMousePosition(std::int64_t mouse_x, std::int64_t mouse_y);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




