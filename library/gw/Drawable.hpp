/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
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
	virtual bool getVisible(void) const;
	virtual bool setVisible(bool value);
	virtual void setVisibleChanged(void);
	virtual void setVisibleChangedHandler(VisibleChangedHandler handler);

public:
	virtual bool createDeviceResources(Window* w) = 0;
	virtual void destroyDeviceResources(void) = 0;
	virtual void draw(Window* winwdow) = 0;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DocumentGrid : public Drawable
{
private:
	ID2D1SolidColorBrush* _pDocumentGridLine0Brush{ nullptr };
	ID2D1SolidColorBrush* _pDocumentGridLine1Brush{ nullptr };
	ID2D1SolidColorBrush* _pDocumentGridLine2Brush{ nullptr };

private:
	//IDWriteTextLayout*    _pCoordTextLayout{ nullptr };
	IDWriteTextFormat*    _pCoordTextFormat{ nullptr };
	ID2D1SolidColorBrush* _pCoordTextBrush{ nullptr };

public:
	DocumentGrid() = default;

public:
	virtual ~DocumentGrid();

	//-----------------------------------------------------------------------
	// Drawable
public:
	virtual bool createDeviceResources(Window* w) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(Window* w) override;

	//-----------------------------------------------------------------------
	// DocumentGrid
private:
	void drawDocumentGrid(Window* w);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class StatusOverayPanel : public Drawable
{
	//-----------------------------------------------------------------------
private:
	std::chrono::time_point<std::chrono::steady_clock> _LastDrawTime;
	std::int64_t                                       _FrameDrawCount{ 0 };
	float                                              _FPS{ 0.0f };

private:
	std::int64_t _Mouse_X{ 0 };
	std::int64_t _Mouse_Y{ 0 };

private:
	//IDWriteTextLayout*    _pStatusTextLayout{ nullptr };
	IDWriteTextFormat*    _pStatusTextFormat{ nullptr };
	ID2D1SolidColorBrush* _pStatusTextBrush{ nullptr };
	ID2D1SolidColorBrush* _pStatusBoxFillBrush{ nullptr };
	ID2D1SolidColorBrush* _pStatusBoxLineBrush{ nullptr };

	//-----------------------------------------------------------------------
public:
	StatusOverayPanel();

public:
	virtual ~StatusOverayPanel();

	//-----------------------------------------------------------------------
	// Drawable
public:
	virtual bool createDeviceResources(Window* w) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(Window* w) override;

	//-----------------------------------------------------------------------
	// StatusOverayPanel
private:
	void drawStatusOverayPanel(Window* w);

public:
	void calculateFPS(void);
	void setMousePosition(std::int64_t mouse_x, std::int64_t mouse_y);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




