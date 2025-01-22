#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class Font final
{
private:
	HFONT _FontHandle{ nullptr };

public:
	explicit Font(
		const std::wstring& faceName, 
		int pointSize
	);

public:
	~Font();

public:
	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	Font(Font&&) = delete;
	Font& operator=(Font&&) = delete;

public:
	[[nodiscard]] constexpr operator HFONT(void) const
	{
		return _FontHandle;
	}

public:
	HFONT createFont(
		const std::wstring& faceName,
		int pointSize
	);

public:
	HFONT select(HDC hdc);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


