/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void WidgetDesignerCommandManager::executeCommand(std::shared_ptr<WidgetDesignerCommand> command)
{
	command->execute();


	_UndoStack.push(command);
	while (!_RedoStack.empty())
	{
		_RedoStack.pop();
	}
}

void WidgetDesignerCommandManager::undo(void)
{
	if (!_UndoStack.empty())
	{
		auto command = _UndoStack.top();

		_UndoStack.pop();
		command->undo();
		_RedoStack.push(command);
	}
}

void WidgetDesignerCommandManager::redo(void)
{
	if (!_RedoStack.empty())
	{
		auto command = _RedoStack.top();

		_RedoStack.pop();
		command->execute();
		_UndoStack.push(command);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




