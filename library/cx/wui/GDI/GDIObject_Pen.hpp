#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class Pen final
{
private:
	HPEN _PenHandle{ nullptr };

public:
	explicit Pen(
		int style, int width, COLORREF color
	);

public:
	~Pen();

public:
	Pen(const Pen&) = delete;
	Pen& operator=(const Pen&) = delete;

	Pen(Pen&&) = delete;
	Pen& operator=(Pen&&) = delete;

public:
	[[nodiscard]] constexpr operator HPEN(void) const
	{
		return _PenHandle;
	}

	HPEN getPenHandle(void) const
	{
		return _PenHandle;
	}

private:
	HPEN createPen(
		int style, int width, COLORREF color
	);

public:
	HPEN select(HDC hdc);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


