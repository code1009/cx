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
	WidgetDesignerCommand() = default;
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
	std::stack<std::shared_ptr<WidgetDesignerCommand>> _UndoStack;
	std::stack<std::shared_ptr<WidgetDesignerCommand>> _RedoStack;

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
	void executeCommand(std::shared_ptr<WidgetDesignerCommand> command);

public:
	void undo(void);
	void redo(void);

public:
	std::shared_ptr<WidgetDesignerCommand> getLastExecutedCommand(void);
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_setGridSize : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	std::uint64_t _OldX, _OldY;
	std::uint64_t _NewX, _NewY;

public:
	WidgetDesignerCommand_setGridSize(WidgetDesignerModel* model, std::uint64_t x, std::uint64_t y)
		: _WidgetDesignerModel(model), _NewX(x), _NewY(y)
	{
		_WidgetDesignerModel->getWidgetDesigner()->getGridSize(_OldX, _OldY);
	}

	void execute() override
	{
		_WidgetDesignerModel->getWidgetDesigner()->setGridSize(_NewX, _NewY);
	}

	void undo() override
	{
		_WidgetDesignerModel->getWidgetDesigner()->setGridSize(_OldX, _OldY);
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_setSnapToGrid : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	bool _OldSnapToGrid;
	bool _NewSnapToGrid;

public:
	WidgetDesignerCommand_setSnapToGrid(WidgetDesignerModel* model, bool snapToGrid)
		: _WidgetDesignerModel(model), _NewSnapToGrid(snapToGrid)
	{
		_OldSnapToGrid = _WidgetDesignerModel->getWidgetDesigner()->getSnapToGrid();
	}

	void execute() override
	{
		_WidgetDesignerModel->getWidgetDesigner()->setSnapToGrid(_NewSnapToGrid);
	}

	void undo() override
	{
		_WidgetDesignerModel->getWidgetDesigner()->setSnapToGrid(_OldSnapToGrid);
	}
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




