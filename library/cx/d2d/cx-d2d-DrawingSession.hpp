#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DrawingSession
{
private:
	Context* _Context{ nullptr };

public:
	explicit DrawingSession(Context* context);
	virtual ~DrawingSession() = default;

public:
	DrawingSession(const DrawingSession&) = delete;
	DrawingSession& operator=(const DrawingSession&) = delete;
	DrawingSession(DrawingSession&&) = delete;
	DrawingSession& operator=(DrawingSession&&) = delete;

public:
	Context* getContext(void) const noexcept
	{
		return _Context;
	}

public:
	void Clear(Color const& color);

public:
	void DrawText(std::wstring const& text, float x, float y, float w, float h, Color const& color, TextFormat const& format);

public:
	void DrawLine(float x0, float y0, float x1, float y1, Color const& color, float strokeWidth);

public:
	void FillRectangle(float x, float y, float w, float h, Color const& color);
	void DrawRectangle(float x, float y, float w, float h, Color const& color, float strokeWidth);

	void FillRoundedRectangle(float x, float y, float w, float h, float radiusX, float radiusY, Color const& color);
	void DrawRoundedRectangle(float x, float y, float w, float h, float radiusX, float radiusY, Color const& color, float strokeWidth);

public:
	void FillCircle(Point const& center, float radius, Color const& color);
	void DrawCircle(Point const& center, float radius, Color const& color, float strokeWidth);

	void FillEllipse(float x, float y, float radiusX, float radiusY, Color const& color);
	void DrawEllipse(float x, float y, float radiusX, float radiusY, Color const& color, float strokeWidth);

public:
	void Transform(D2D1_MATRIX_3X2_F const& value);
	D2D1_MATRIX_3X2_F Transform(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
