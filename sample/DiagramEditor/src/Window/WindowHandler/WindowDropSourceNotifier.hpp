/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui::dragdrop
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDropSourceData
{
public:
	std::uint32_t _ClipboardFormat{ 0 };
	std::vector<std::uint8_t> _DataBuffer;

public:
	WindowDropSourceData() = default;
	~WindowDropSourceData() = default;

public:
	explicit WindowDropSourceData(std::uint32_t clipboardFormat);

public:
	void setData(std::uint8_t* pointer, std::size_t size);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowDropSourceNotifier
{
private:
	IDropSource* _pDropSource{ nullptr };
	IDataObject* _pDataObject{ nullptr };
	WindowDropSourceData _Data;

public:
	WindowDropSourceNotifier();

public:
	virtual ~WindowDropSourceNotifier();

public:
	WindowDropSourceNotifier(const WindowDropSourceNotifier&) = delete;
	WindowDropSourceNotifier& operator=(const WindowDropSourceNotifier&) = delete;

	WindowDropSourceNotifier(WindowDropSourceNotifier&&) = delete;
	WindowDropSourceNotifier& operator=(WindowDropSourceNotifier&&) = delete;

public:
	bool doDragDrop(WindowDropSourceData const& data);

private:
	bool createCOMObject(void);
	void releaseCOMObject(void);

	bool setData(
		IDataObject* pDataObject,
		const std::uint8_t* pointer,
		std::size_t size,
		std::uint32_t cf
	);

	bool doDragDrop(DWORD& dwEffect, bool& drop);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




