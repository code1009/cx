/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetEventDragDropData
{
public:
	std::wstring _String;

public:
	WidgetEventDragDropData() = default;

public:
	virtual ~WidgetEventDragDropData() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetEventDragDropNotifier
{
private:
	WidgetEventDragDropData _DragDropData;

private:
	class Impl;
	std::unique_ptr<Impl> _Impl;

public:
	WidgetEventDragDropNotifier();

public:
	virtual ~WidgetEventDragDropNotifier();

public:
	WidgetEventDragDropNotifier(const WidgetEventDragDropNotifier&) = delete;
	WidgetEventDragDropNotifier& operator=(const WidgetEventDragDropNotifier&) = delete;

	WidgetEventDragDropNotifier(WidgetEventDragDropNotifier&&) = delete;
	WidgetEventDragDropNotifier& operator=(WidgetEventDragDropNotifier&&) = delete;

public:
	void setDragDropData(const WidgetEventDragDropData& data);

public:
	bool doDragDrop(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetEventDragDropHandler
{
private:
	WidgetDocument* _WidgetDocument{ nullptr };
	Viewport* _Viewport{ nullptr };

private:
	class Impl;
	std::unique_ptr<Impl> _Impl;

public:
	explicit WidgetEventDragDropHandler(WidgetDocument* doc, Viewport* viewport);

public:
	virtual ~WidgetEventDragDropHandler();

public:
	WidgetEventDragDropHandler(const WidgetEventDragDropHandler&) = delete;
	WidgetEventDragDropHandler& operator=(const WidgetEventDragDropHandler&) = delete;

	WidgetEventDragDropHandler(WidgetEventDragDropHandler&&) = delete;
	WidgetEventDragDropHandler& operator=(WidgetEventDragDropHandler&&) = delete;

public:
	void registerDragDrop(void);
	void unregisterDragDrop(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool initializeWidgetEventDragDrop(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




