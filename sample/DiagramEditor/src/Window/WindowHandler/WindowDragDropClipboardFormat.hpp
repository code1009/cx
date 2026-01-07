/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDragDropClipboardFormat
{
private:
	ClipboardFormat _ClipboardFormat;

public:
	WindowDragDropClipboardFormat() = default;

public:
	WindowDragDropClipboardFormat(const WindowDragDropClipboardFormat& other) = delete;
	WindowDragDropClipboardFormat& operator=(const WindowDragDropClipboardFormat& other) = delete;
	WindowDragDropClipboardFormat(WindowDragDropClipboardFormat&& other) noexcept = delete;
	WindowDragDropClipboardFormat& operator=(WindowDragDropClipboardFormat&& other) noexcept = delete;

public:
	bool initialize(void);
	std::uint32_t getClipboardFormat(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowDragDropClipboardFormat* getWindowDragDropClipboardFormat(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




