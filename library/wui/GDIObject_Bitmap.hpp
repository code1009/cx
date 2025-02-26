#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class Bitmap final
{
private:
	HBITMAP _BitmapHandle{ nullptr };

public:
	explicit Bitmap(
		const std::wstring& filePath
	);

public:
	~Bitmap();

public:
	Bitmap(const Bitmap&) = delete;
	Bitmap& operator=(const Bitmap&) = delete;

	Bitmap(Bitmap&&) = delete;
	Bitmap& operator=(Bitmap&&) = delete;

public:
	[[nodiscard]] constexpr operator HBITMAP(void) const
	{
		return _BitmapHandle;
	}

private:
	HBITMAP loadBitmap(
		const std::wstring& filePath
	);

public:
	HBITMAP select(HDC hdc);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


