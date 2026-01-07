/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Canvas
{
public:
	std::function<void(DrawingSession*)> drawingHandler;

private:
	std::unique_ptr<Context> _Context;
	std::unique_ptr<Renderer> _Renderer;
	std::unique_ptr<DrawingSession> _DrawingSession;

public:
	explicit Canvas(Factory* factory, HWND hwnd, std::uint32_t cx, std::uint32_t cy);
	explicit Canvas(Factory* factory, HWND hwnd);

public:
	virtual ~Canvas();

public:
	Canvas(const Canvas&) = delete;
	Canvas& operator=(const Canvas&) = delete;
	Canvas(Canvas&&) = delete;
	Canvas& operator=(Canvas&&) = delete;

public:
	void resize(std::uint32_t width, std::uint32_t height);
	void draw(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




