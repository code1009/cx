/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_setViewProperties::EditCommand_setViewProperties(
		Edit& edit,
		ViewProperties const& current,
		ViewProperties const& previous
	) :
		_Edit(edit),
		_Current(current),
		_Previous(previous)
	{
	}

	//=======================================================================
	void EditCommand_setViewProperties::execute(void)
	{
		ViewProperties const& viewProperties = _Current;
		_Edit.viewContext().setSize(viewProperties.width(), viewProperties.height());
		_Edit.viewBackground().fillStyle().fillColor(viewProperties.backgroundFillColor());
		_Edit.notifyViewPropertiesChanged();
	}

	void EditCommand_setViewProperties::undo(void)
	{
		ViewProperties const& viewProperties = _Previous;
		_Edit.viewContext().setSize(viewProperties.width(), viewProperties.height());
		_Edit.viewBackground().fillStyle().fillColor(viewProperties.backgroundFillColor());
		_Edit.notifyViewPropertiesChanged();
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_commitItemPropertiesChanged::EditCommand_commitItemPropertiesChanged(
		Edit& edit,
		std::shared_ptr<Item> const& item,
		std::shared_ptr<Item> const& current,
		std::shared_ptr<Item> const& previous
	) :
		_Edit(edit),
		_Item(item),
		_Current(current),
		_Previous(previous)
	{
	}

	//=======================================================================
	void EditCommand_commitItemPropertiesChanged::execute(void)
	{
		bool invalidated = false;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			_Current->copyTo(design);
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	void EditCommand_commitItemPropertiesChanged::undo(void)
	{
		bool invalidated = false;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			_Previous->copyTo(design);
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_pasteItems::EditCommand_pasteItems(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items
	) :
		_Edit(edit),
		_Items(items)
	{
		std::size_t pasteOffsetStep = _Edit.pasteOffsetStep();
		std::uint64_t gridXSize = 0;
		std::uint64_t gridYSize = 0;
		_Edit.getGridSize(gridXSize, gridYSize);

		Point offset{ static_cast<Coord>(gridXSize * pasteOffsetStep), static_cast<Coord>(gridYSize * pasteOffsetStep) };
		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				std::shared_ptr<Item> newItem = design->clone();
				std::shared_ptr<Design> newDesign = std::dynamic_pointer_cast<Design>(newItem);
				if (newDesign)
				{
					newDesign->anchors().visible(true);
					newDesign->moveOffset(offset);

					_PasteItems.push_back(newDesign);
				}
			}
		}
	}

	//=======================================================================
	void EditCommand_pasteItems::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _PasteItems)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				design->registerEventHandler(_Edit.eventHandlerRegistry());
				design->setEdit(&_Edit);
				_Edit.model().add(item);
				invalidated = true;
			}
		}
		_Edit.pasteOffsetStep(_Edit.pasteOffsetStep() + 1);


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}

	void EditCommand_pasteItems::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _PasteItems)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
				design->setEdit(nullptr);
				_Edit.eventHandlerRegistry().unregisterEventHandler(design);
				invalidated = true;
			}
		}
		std::size_t pasteOffsetStep = _Edit.pasteOffsetStep();
		if (pasteOffsetStep > 0)
		{
			_Edit.pasteOffsetStep(pasteOffsetStep - 1);
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_addItem::EditCommand_addItem(
		Edit& edit,
		std::shared_ptr<Item> const& item
	) :
		_Edit(edit),
		_Item(item)
	{
	}

	//=======================================================================
	void EditCommand_addItem::execute(void)
	{
		bool invalidated = false;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			design->registerEventHandler(_Edit.eventHandlerRegistry());
			design->setEdit(&_Edit);
			_Edit.model().add(design);
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}

	void EditCommand_addItem::undo(void)
	{
		bool invalidated = false;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			_Edit.eventHandlerRegistry().unregisterEventHandler(design);
			design->setEdit(nullptr);
			_Edit.model().remove(_Item);
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_deleteItems::EditCommand_deleteItems(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items
	) :
		_Edit(edit),
		_Items(items)
	{
		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				auto index = _Edit.model().indexOf(design);
				if (index != std::numeric_limits<std::size_t>::max())
				{
					_ItemMap[index] = design;
				}
			}
		}
	}

	//=======================================================================
	void EditCommand_deleteItems::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.eventHandlerRegistry().unregisterEventHandler(design);
				design->setEdit(nullptr);
				_Edit.model().remove(design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}

	void EditCommand_deleteItems::undo(void)
	{
		bool invalidated = false;


		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				design->registerEventHandler(_Edit.eventHandlerRegistry());
				design->setEdit(&_Edit);
				_Edit.model().insert(index, design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_moveItems::EditCommand_moveItems(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items,
		Point const& offset
	) :
		_Edit(edit),
		_Items(items),
		_Offset(offset)
	{
	}

	//=======================================================================
	void EditCommand_moveItems::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				design->moveOffset(_Offset);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	void EditCommand_moveItems::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				design->moveOffset(-_Offset);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	//=======================================================================
	void EditCommand_moveItems::moveOffset(Point& offset)
	{
		bool invalidated = false;


		_Offset += offset;
		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				design->moveOffset(offset);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_bringItemsToFront::EditCommand_bringItemsToFront(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items
	) :
		_Edit(edit),
		_Items(items)
	{
		for (auto& item : _Items)
		{
			auto index = _Edit.model().indexOf(item);
			if (index != std::numeric_limits<std::size_t>::max())
			{
				std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
				if (design)
				{
					_ItemMap[index] = design;
				}
			}
		}
	}

	//=======================================================================
	void EditCommand_bringItemsToFront::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().insert(index + 1, design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	//=======================================================================
	void EditCommand_bringItemsToFront::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().insert(index, design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_sendItemsToBack::EditCommand_sendItemsToBack(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items
	) :
		_Edit(edit),
		_Items(items)
	{
		for (auto& item : _Items)
		{
			auto index = _Edit.model().indexOf(item);
			if (index != std::numeric_limits<std::size_t>::max())
			{
				std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
				if (design)
				{
					_ItemMap[index] = design;
				}
			}
		}
	}

	//=======================================================================
	void EditCommand_sendItemsToBack::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				if (index == 0)
				{
					_Edit.model().insert(index, design);
				}
				else
				{
					_Edit.model().insert(index - 1, design);
				}
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	//=======================================================================
	void EditCommand_sendItemsToBack::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().insert(index, design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_bringItemsToTop::EditCommand_bringItemsToTop(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items
	) :
		_Edit(edit),
		_Items(items)
	{
		for (auto& item : _Items)
		{
			auto index = _Edit.model().indexOf(item);
			if (index != std::numeric_limits<std::size_t>::max())
			{
				std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
				if (design)
				{
					_ItemMap[index] = design;
				}
			}
		}
	}

	//=======================================================================
	void EditCommand_bringItemsToTop::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().add(design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	void EditCommand_bringItemsToTop::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().insert(index, design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_sendItemsToBottom::EditCommand_sendItemsToBottom(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& items
	) :
		_Edit(edit),
		_Items(items)
	{
		for (auto& item : _Items)
		{
			auto index = _Edit.model().indexOf(item);
			if (index != std::numeric_limits<std::size_t>::max())
			{
				std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
				if (design)
				{
					_ItemMap[index] = design;
				}
			}
		}
	}

	//=======================================================================
	void EditCommand_sendItemsToBottom::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		std::size_t newIndex = 0;
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().insert(newIndex, design);
				newIndex++;
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	void EditCommand_sendItemsToBottom::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _Items)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().remove(design);
			}
		}
		for (auto& [index, item] : _ItemMap)
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				_Edit.model().insert(index, design);
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_selectItems::EditCommand_selectItems(
		Edit& edit,
		std::vector<std::shared_ptr<Item>> const& selectItems,
		std::vector<std::shared_ptr<Item>> const& preselectedItems
	) :
		_Edit(edit),
		_SelectItems(selectItems),
		_PreselectedItems(preselectedItems)
	{
	}

	//=======================================================================
	void EditCommand_selectItems::execute(void)
	{
		bool invalidated = false;


		for (auto& item : _Edit.model().items())
		{
			auto design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				auto found = std::find(
					_SelectItems.begin(),
					_SelectItems.end(),
					item);
				if (found != _SelectItems.end())
				{
					design->anchors().visible(true);
				}
				else
				{
					design->anchors().visible(false);
				}
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}

	void EditCommand_selectItems::undo(void)
	{
		bool invalidated = false;


		for (auto& item : _Edit.model().items())
		{
			auto design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				auto found = std::find(
					_PreselectedItems.begin(),
					_PreselectedItems.end(),
					item);
				if (found != _PreselectedItems.end())
				{
					design->anchors().visible(true);
				}
				else
				{
					design->anchors().visible(false);
				}
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_toggleItemSelection::EditCommand_toggleItemSelection(
		Edit& edit,
		std::shared_ptr<Item> const& item
	) :
		_Edit(edit),
		_Item(item)
	{
		auto design = std::dynamic_pointer_cast<Design>(item);
		if (design)
		{
			if (design->anchors().visible())
			{
				_IsSelected = true;
			}
			else
			{
				_IsSelected = false;
			}
		}
	}

	//=======================================================================
	void EditCommand_toggleItemSelection::execute(void)
	{
		bool invalidated = false;


		auto design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			if (_IsSelected)
			{
				design->anchors().visible(false);
			}
			else
			{
				design->anchors().visible(true);
			}
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}

	void EditCommand_toggleItemSelection::undo(void)
	{
		bool invalidated = false;


		auto design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			if (_IsSelected)
			{
				design->anchors().visible(true);
			}
			else
			{
				design->anchors().visible(false);
			}
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
		_Edit.notifyItemSelectionChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommand_moveItemAnchor::EditCommand_moveItemAnchor(
		Edit& edit,
		std::shared_ptr<Item> const& item,
		std::shared_ptr<Anchor> const& anchor,
		Point const& position
	) :
		_Edit(edit),
		_Item(item),
		_Anchor(anchor),
		_Position(position)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			_OldTargetPoints = design->getTarget()->getPoints();
			_OldDesignPoints = design->getPoints();
		}
	}

	//=======================================================================
	void EditCommand_moveItemAnchor::execute(void)
	{
		bool invalidated = false;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			design->moveAnchor(_Anchor, _Position);
			_NewTargetPoints = design->getTarget()->getPoints();
			_NewDesignPoints = design->getPoints();
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	void EditCommand_moveItemAnchor::undo(void)
	{
		bool invalidated = false;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			design->getTarget()->setPoints(_OldTargetPoints);
			design->setPoints(_OldDesignPoints);
			invalidated = true;
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}
	}

	//=======================================================================
	bool EditCommand_moveItemAnchor::moveItemAnchor(const Point& point)
	{
		bool invalidated = false;


		bool result = false;
		_Position = point;


		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_Item);
		if (design)
		{
			design->moveAnchor(_Anchor, _Position);

			_NewTargetPoints = design->getTarget()->getPoints();
			_NewDesignPoints = design->getPoints();

			if ((_OldTargetPoints == _NewTargetPoints) &&
				(_OldDesignPoints == _NewDesignPoints))
			{
				result = false;
			}
			else
			{
				result = true;
				invalidated = true;
			}
		}


		if (invalidated)
		{
			_Edit.notifyInvalidated();
		}


		return result;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditCommandManager::EditCommandManager(Edit& edit) :
		_Edit(edit)
	{
	}

	//=======================================================================
	void EditCommandManager::executeCommand(std::shared_ptr<EditCommand> command)
	{
		command->execute();


		EditSeed::Seed previousSeed = _Edit.editSeed().currentSeed();
		if (command->affectsModification())
		{
			_Edit.editSeed().touch();
		}
		EditSeed::Seed currentSeed = _Edit.editSeed().currentSeed();


		EditCommandRecord commandRecord(command, currentSeed, previousSeed);
		_UndoStack.push(commandRecord);
		while (!_RedoStack.empty())
		{
			_RedoStack.pop();
		}
	}

	//=======================================================================
	bool EditCommandManager::undo(void)
	{
		if (_UndoStack.empty())
		{
			return false;
		}

		auto commandRecord = _UndoStack.top();
		_UndoStack.pop();
		commandRecord.command->undo();
		_Edit.editSeed().restore(commandRecord.previousSeed);
		_RedoStack.push(commandRecord);

		return true;
	}

	bool EditCommandManager::redo(void)
	{
		if (_RedoStack.empty())
		{
			return false;
		}

		auto commandRecord = _RedoStack.top();
		_RedoStack.pop();
		commandRecord.command->execute();
		_Edit.editSeed().restore(commandRecord.currentSeed);
		_UndoStack.push(commandRecord);

		return true;
	}

	//=======================================================================
	void EditCommandManager::clear(void)
	{
		while (!_RedoStack.empty())
		{
			_RedoStack.pop();
		}
		while (!_UndoStack.empty())
		{
			_UndoStack.pop();
		}
	}

	//=======================================================================
	void EditCommandManager::addCommand(std::shared_ptr<EditCommand> command)
	{
		EditSeed::Seed previousSeed = _Edit.editSeed().currentSeed();
		if (command->affectsModification())
		{
			_Edit.editSeed().touch();
		}
		EditSeed::Seed currentSeed = _Edit.editSeed().currentSeed();


		EditCommandRecord commandRecord(command, currentSeed, previousSeed);
		_UndoStack.push(commandRecord);
		while (!_RedoStack.empty())
		{
			_RedoStack.pop();
		}
	}

	std::shared_ptr<EditCommand> EditCommandManager::getLastExecutedCommand(void)
	{
		if (_UndoStack.empty())
		{
			return nullptr;
		}

		return _UndoStack.top().command;
	}

	void EditCommandManager::deleteLastExecutedCommand(void)
	{
		if (!_UndoStack.empty())
		{
			_UndoStack.pop();
		}
	}
}
