#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{




#if 0
	//ID2D1Geometry* geometry,

	STDMETHOD_(void, DrawGeometry)(
		_In_ ID2D1Geometry* geometry,
		_In_ ID2D1Brush* brush,
		FLOAT strokeWidth = 1.0f,
		_In_opt_ ID2D1StrokeStyle* strokeStyle = NULL
		) PURE;

	/// <param name="opacityBrush">An optionally specified opacity brush. Only the alpha
	/// channel of the corresponding brush will be sampled and will be applied to the
	/// entire fill of the geometry. If this brush is specified, the fill brush must be
	/// a bitmap brush with an extend mode of D2D1_EXTEND_MODE_CLAMP.</param>
	STDMETHOD_(void, FillGeometry)(
		_In_ ID2D1Geometry* geometry,
		_In_ ID2D1Brush* brush,
		_In_opt_ ID2D1Brush* opacityBrush = NULL
		) PURE;
#endif

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
	void DrawText(std::wstring const& text, float x, float y, float w, float h, Color const& color, TextFormat& format);

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
