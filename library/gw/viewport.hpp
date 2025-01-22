/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class viewport
{
public:
	using repainter = std::function<void(void)>;

protected:
	HWND _hwnd{ nullptr };
	repainter _repainter{};

protected:
	std::int64_t _window_cx{ 0 };
	std::int64_t _window_cy{ 0 };

	double _document_cx{ 0 };
	double _document_cy{ 0 };

	double _scale{ 1.0 };

	std::int64_t _image_cx{ 0 };
	std::int64_t _image_cy{ 0 };

protected:
	double _document_viewport_x{ 0 };
	double _document_viewport_y{ 0 };

	std::int64_t _image_viewport_x{ 0 };
	std::int64_t _image_viewport_y{ 0 };

protected:
	std::int64_t _view_x_scroll_min{ 0 };
	std::int64_t _view_x_scroll_max{ 0 };
	std::int64_t _view_x_scroll_page{ 0 };
	std::int64_t _view_x_scroll_line{ 0 };

	std::int64_t _view_y_scroll_min{ 0 };
	std::int64_t _view_y_scroll_max{ 0 };
	std::int64_t _view_y_scroll_page{ 0 };
	std::int64_t _view_y_scroll_line{ 0 };

	bool         _scrollbar_enabled{ false };


	//-----------------------------------------------------------------------
public:
	explicit viewport(HWND hwnd, repainter r);

public:
	virtual ~viewport() = default;


	//-----------------------------------------------------------------------
public:
	viewport(const viewport&) = delete;
	viewport& operator=(const viewport&) = delete;

	viewport(viewport&&) = delete;
	viewport& operator=(viewport&&) = delete;


	//-----------------------------------------------------------------------
public:
	virtual HWND get_hwnd(void);
	virtual void repaint(void);


	//-----------------------------------------------------------------------
public:
	virtual void get_window_size(std::int64_t& cx, std::int64_t& cy);
	virtual void get_scale(double& scale);

	virtual void get_document_size(double& cx, double& cy);
	virtual void get_document_viewport_point(double& x, double& y);
	virtual void get_document_viewport_size(double& cx, double& cy);

	virtual void get_image_size(std::int64_t& cx, std::int64_t& cy);
	virtual void get_image_viewport_point(std::int64_t& x, std::int64_t& y);
	virtual void get_image_viewport_size(std::int64_t& cx, std::int64_t& cy);

public:
	virtual void window_to_document(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y);
	virtual void document_to_window(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y);

public:
	virtual void set_window_size(std::int64_t cx, std::int64_t cy);
	virtual void set_scale(double scale);
	virtual void set_document_size(double cx, double cy);

protected:
	virtual void update_image_size(void);
	virtual void update_viewport(void);
	virtual void update_view_scroll(void);
	virtual void update_scrollbar_position(void);


	//-----------------------------------------------------------------------
public:
	virtual void vscroll(std::uint32_t scroll_code);
	virtual void hscroll(std::uint32_t scroll_code);

protected:
	virtual std::int64_t scroll(
		std::uint32_t scroll_code, std::int64_t scroll_pos,
		std::uint64_t a_view_scroll_page,
		std::uint64_t a_view_scroll_line,
		std::uint64_t a_view_scroll_min,
		std::uint64_t a_view_scroll_max,
		std::uint64_t a_view_scroll_pos);

public:
	virtual void enable_scrollbar(bool enable);


	//-----------------------------------------------------------------------
public:
	virtual void zoom(bool zoom_in);
	virtual void fit_document_to_window(bool vert = false);


	//-----------------------------------------------------------------------
public:
	virtual void get_document_viewport_point_translation(double& x, double& y);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class center_viewport : public viewport
{
public:
	explicit center_viewport(HWND hwnd, repainter r);

public:
	virtual ~center_viewport() = default;

	//-----------------------------------------------------------------------
	// viewport
public:
	virtual void window_to_document(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y) override;
	virtual void document_to_window(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y) override;

protected:
	virtual void update_viewport(void) override;

public:
	virtual void get_document_viewport_point_translation(double& x, double& y) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




