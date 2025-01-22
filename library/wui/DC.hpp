#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class DC
{
private:
	HDC _DCHandle{ nullptr };

public:
	DC() = default;
	virtual ~DC() = default;

public:
	DC(const DC&) = delete;
	DC& operator=(const DC&) = delete;

	DC(DC&&) = delete;
	DC& operator=(DC&&) = delete;

public:
	[[nodiscard]] constexpr operator HDC (void) const
	{
		return _DCHandle;
	}

public:
	virtual HDC getDCHandle(void) const;
	virtual HDC setDCHandle(HDC handle);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


