#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ClipboardFormat
{
private:
	std::uint32_t _CustomClipboardFormat{ 0 };

public:
	ClipboardFormat() = default;
	virtual ~ClipboardFormat() = default;

public:
	ClipboardFormat(const ClipboardFormat&) = delete;
	ClipboardFormat& operator=(const ClipboardFormat&) = delete;

	ClipboardFormat(ClipboardFormat&&) = delete;
	ClipboardFormat& operator=(ClipboardFormat&&) = delete;

public:
	bool registerClipboardFormat(std::wstring name);

	std::uint32_t getClipboardFormat(void);
	void setClipboardFormat(std::uint32_t cf);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




