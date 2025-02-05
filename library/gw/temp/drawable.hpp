/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class window;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class drawable
{
public:
	using visible_changed_handler = std::function<void(drawable*)>;

private:
	visible_changed_handler _visible_changed_handler;
	bool _visible{ false };

public:
	drawable() = default;

public:
	virtual ~drawable() = default;

public:
	drawable(const drawable&) = delete;
	drawable& operator=(const drawable&) = delete;
	drawable(drawable&&) = delete;
	drawable& operator=(drawable&&) = delete;

public:
	virtual bool get_visible(void) const;	
	virtual bool set_visible(bool value);
	virtual void set_visible_changed(void);
	virtual void set_visible_changed_handler(visible_changed_handler handler);

public:
	virtual bool create_device_resources(window* w) = 0;
	virtual void destroy_device_resources(void) = 0;
	virtual void draw(window* w) = 0;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class document_grid : public drawable
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
	document_grid() = default;

public:
	virtual ~document_grid();

	//-----------------------------------------------------------------------
	// drawable
public:
	virtual bool create_device_resources(window* w) override;
	virtual void destroy_device_resources(void) override;
	virtual void draw(window* w) override;

	//-----------------------------------------------------------------------
	// document_grid
private:
	void draw_document_grid(window* w);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class status : public drawable
{
	//-----------------------------------------------------------------------
private:
	std::chrono::time_point<std::chrono::steady_clock> _last_draw_time;
	std::int64_t                                       _frame_draw_count{ 0 };
	float                                              _fps{ 0.0f };

private:
	std::int64_t _mouse_x{ 0 };
	std::int64_t _mouse_y{ 0 };

private:
	//IDWriteTextLayout*    _pStatusTextLayout{ nullptr };
	IDWriteTextFormat*    _pStatusTextFormat{ nullptr };
	ID2D1SolidColorBrush* _pStatusTextBrush{ nullptr };
	ID2D1SolidColorBrush* _pStatusBoxFillBrush{ nullptr };
	ID2D1SolidColorBrush* _pStatusBoxLineBrush{ nullptr };

	//-----------------------------------------------------------------------
public:
	status();

public:
	virtual ~status();

	//-----------------------------------------------------------------------
	// drawable
public:
	virtual bool create_device_resources(window* w) override;
	virtual void destroy_device_resources(void) override;
	virtual void draw(window* w) override;

	//-----------------------------------------------------------------------
	// status
private:
	void draw_status(window* w);

public:
	void calculate_fps(void);
	void set_mouse_position(std::int64_t mouse_x, std::int64_t mouse_y);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




