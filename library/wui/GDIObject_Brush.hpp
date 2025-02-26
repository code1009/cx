#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class Brush final
{
private:
	HBRUSH _BrushHandle{ nullptr };

public:
	explicit Brush(
		COLORREF color
	);

public:
	~Brush();

public:
	Brush(const Brush&) = delete;
	Brush& operator=(const Brush&) = delete;

	Brush(Brush&&) = delete;
	Brush& operator=(Brush&&) = delete;

public:
	[[nodiscard]] constexpr operator HBRUSH(void) const
	{
		return _BrushHandle;
	}

public:
	HBRUSH createBrush(
		COLORREF color
	);

public:
	HBRUSH select(HDC hdc);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


