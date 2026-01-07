/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDropTargetData
{
public:
	std::uint32_t _ClipboardFormat{ 0 };
	std::uint32_t _DataSeq{ 0 };
	std::vector<std::uint8_t> _DataBuffer{};

public:
	WindowDropTargetData() = default;

public:
	explicit WindowDropTargetData(std::uint32_t clipboardFormat, std::uint32_t dataSeq);
	explicit WindowDropTargetData(std::uint32_t clipboardFormat, std::uint32_t dataSeq, std::uint8_t* pointer, std::size_t size);

public:
	void setData(std::uint8_t* pointer, std::size_t size);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDropTargetHandler
{
private:
	HWND _Hwnd{ nullptr };
	std::uint32_t _ClipboardFormat{ 0 };

private:
	IDropTarget* _pDropTarget{ nullptr };

private:
	std::uint32_t _DataSeq{ 0 };

public:
	std::function<void(std::uint32_t seq, std::uint32_t flags, std::uint32_t x, std::uint32_t y, WindowDropTargetData const& data)> dragEnterHandler;
	std::function<void(std::uint32_t seq, std::uint32_t flags, std::uint32_t x, std::uint32_t y)                                  > dragOverHandler;
	std::function<void(std::uint32_t seq)                                                                                         > dragLeaveHandler;
	std::function<void(std::uint32_t seq, std::uint32_t flags, std::uint32_t x, std::uint32_t y)                                  > dropHandler;

public:
	explicit WindowDropTargetHandler(HWND hwnd, std::uint32_t clipboardFormat);

public:
	virtual ~WindowDropTargetHandler();

public:
	WindowDropTargetHandler(const WindowDropTargetHandler&) = delete;
	WindowDropTargetHandler& operator=(const WindowDropTargetHandler&) = delete;

	WindowDropTargetHandler(WindowDropTargetHandler&&) = delete;
	WindowDropTargetHandler& operator=(WindowDropTargetHandler&&) = delete;

private:
	void onDragEnter(DWORD grfKeyState, POINTL pt, HGLOBAL hGlobal);
	void onDragOver(DWORD grfKeyState, POINTL pt);
	void onDragLeave(void);
	void onDrop(DWORD grfKeyState, POINTL pt);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




