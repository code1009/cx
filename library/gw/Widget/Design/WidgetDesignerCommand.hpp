/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand
{
public:
	virtual ~WidgetDesignerCommand() = default;

public:
	WidgetDesignerCommand(const WidgetDesignerCommand&) = delete;
	WidgetDesignerCommand& operator=(const WidgetDesignerCommand&) = delete;

	WidgetDesignerCommand(WidgetDesignerCommand&&) = delete;
	WidgetDesignerCommand& operator=(WidgetDesignerCommand&&) = delete;

public:
	virtual void execute(void) = 0;
	virtual void undo(void) = 0;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommandManager
{
private:
	std::stack<std::unique_ptr<WidgetDesignerCommand>> _UndoStack;
	std::stack<std::unique_ptr<WidgetDesignerCommand>> _RedoStack;

public:
	WidgetDesignerCommandManager() = default;

public:
	virtual ~WidgetDesignerCommandManager() = default;

public:
	WidgetDesignerCommandManager(const WidgetDesignerCommandManager&) = delete;
	WidgetDesignerCommandManager& operator=(const WidgetDesignerCommandManager&) = delete;

	WidgetDesignerCommandManager(WidgetDesignerCommandManager&&) = delete;
	WidgetDesignerCommandManager& operator=(WidgetDesignerCommandManager&&) = delete;

public:
	void executeCommand(std::unique_ptr<WidgetDesignerCommand> command);

public:
	void undo(void);
	void redo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




