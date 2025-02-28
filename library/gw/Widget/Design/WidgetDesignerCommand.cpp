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
void WidgetDesignerCommandManager::executeCommand(std::unique_ptr<WidgetDesignerCommand> command)
{
	command->execute();


	_UndoStack.push(std::move(command));
	while (!_RedoStack.empty())
	{
		_RedoStack.pop();
	}
}

void WidgetDesignerCommandManager::undo(void)
{
	if (!_UndoStack.empty())
	{
		auto command = std::move(_UndoStack.top());

		_UndoStack.pop();
		command->undo();
		_RedoStack.push(std::move(command));
	}
}

void WidgetDesignerCommandManager::redo(void)
{
	if (!_RedoStack.empty())
	{
		auto command = std::move(_RedoStack.top());

		_RedoStack.pop();
		command->execute();
		_UndoStack.push(std::move(command));
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




