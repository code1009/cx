/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Edit::Edit(Coord const w, Coord const h) :
		View(w, h)
	{
		_EventGenerator = std::make_unique<EventGenerator>(*this, false);

		_ViewGrid = std::make_unique<ViewGrid>();
		_ViewStatus = std::make_unique<ViewStatus>();
		_EditCommandManager = std::make_unique<EditCommandManager>(*this);
		_EditPointerTool = std::make_unique<EditPointerTool>(*this);
		_Clipboard = std::make_unique<Items>();


		editSeed_attachModifiedEventHandler();
	}

	//=======================================================================
	void Edit::notifyViewPropertiesChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Edit::notifyViewPropertiesChanged";
		viewPropertiesChangedEventListener.notify(
			Edit_ViewPropertiesChanged,
			nullptr
		);
	}

	void Edit::notifyItemSelectionChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Edit::notifyItemSelectionChanged";
		itemSelectionChangedEventListener.notify(
			Edit_ItemSelectionChangedEvent,
			nullptr
		);
	}

	void Edit::notifyModified(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Edit::notifyModified";
		modifiedEventListener.notify(
			Edit_ModifiedEvent,
			nullptr
		);
	}

	void Edit::editSeed_attachModifiedEventHandler(void)
	{
		_EditSeed.modifiedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Edit::editSeed_onModified,
				this,
				std::placeholders::_1
			)
		);
	}

	void Edit::editSeed_detachModifiedEventHandler(void)
	{
		_EditSeed.modifiedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);	
	}

	void Edit::editSeed_onModified(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Edit::editSeed_onModified:" << event.eventType()
			;
		*/
		notifyModified();
	}

	//=======================================================================
	void Edit::drawBackground(DrawingContext& dctx)
	{
		View::drawBackground(dctx);
		_ViewGrid->draw(dctx, viewContext());
	}

	void Edit::drawForeground(DrawingContext& dctx)
	{
		for (auto item : model().items())
		{
			item->drawUnderlay(dctx);
		}
		if (_NewItem)
		{
			_NewItem->drawUnderlay(dctx);
		}
		for (auto item : model().items())
		{
			item->draw(dctx);
		}
		if (_NewItem)
		{
			_NewItem->draw(dctx);
		}
		for (auto item : model().items())
		{
			item->drawOverlay(dctx);
		}
		if (_NewItem)
		{
			_NewItem->drawOverlay(dctx);
		}

		_EditPointerTool->draw(dctx);
	}

	void Edit::drawOverlay(DrawingContext& dctx)
	{
		_ViewStatus->draw(dctx, viewContext());
	}

	//=======================================================================
	// Call from Window Command
	void Edit::setViewProperties(ViewProperties const& viewProperties)
	{
		auto viewWidth = viewContext().width();
		auto viewHeight = viewContext().height();
		auto viewBackgroundFillColor = viewBackground().fillStyle().fillColor();


		ViewProperties previous(
			viewWidth,
			viewHeight,
			viewBackgroundFillColor
			);


		auto command =
			std::make_shared<EditCommand_setViewProperties>(
				*this,
				viewProperties,
				previous
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	// Call from Properties Manipulator
	void Edit::commitItemPropertiesChanged(std::shared_ptr<Item> const& item, std::shared_ptr<Item> const& current, std::shared_ptr<Item> const& previous)
	{
		auto command =
			editCommandManager().getLastExecutedCommand();

		auto command_commitItemPropertiesChanged =
			std::dynamic_pointer_cast<EditCommand_commitItemPropertiesChanged>(command);
		if (command_commitItemPropertiesChanged)
		{
			if (item==command_commitItemPropertiesChanged->item())
			{
				command_commitItemPropertiesChanged->current(current);
				return;
			}
		}


		command =
			std::make_shared<EditCommand_commitItemPropertiesChanged>(
				*this,
				item,
				current,
				previous
			);
		editCommandManager().addCommand(command);
	}

	//=======================================================================
	// Call from Window Command
	void Edit::undo(void)
	{
		if (editCommandManager().undo())
		{
			notifyInvalidated();
		}
	}

	void Edit::redo(void)
	{
		if (editCommandManager().redo())
		{
			notifyInvalidated();
		}
	}

	//=======================================================================
	// Call from Window Command
	void Edit::cut(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		clipboard().clear();
		for (auto& item : selectedItems)
		{
			clipboard().items().push_back(item);
		}
		std::shared_ptr<EditCommand_deleteItems> command =
			std::make_shared<EditCommand_deleteItems>(
				*this,
				selectedItems
			);
		editCommandManager().executeCommand(command);
		pasteOffsetStep(0);
	}

	void Edit::copy(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		clipboard().clear();
		for (auto& item : selectedItems)
		{
			clipboard().items().push_back(item);
		}
		pasteOffsetStep(1);
	}

	void Edit::paste(void)
	{
		if (clipboard().items().empty())
		{
			return;
		}
		deselectItem();
		std::shared_ptr<EditCommand_pasteItems> command =
			std::make_shared<EditCommand_pasteItems>(
				*this,
				clipboard().items()
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	// Call from Window Command
	void Edit::bringToFront(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		std::shared_ptr<EditCommand_bringItemsToFront> command =
			std::make_shared<EditCommand_bringItemsToFront>(
				*this,
				selectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::sendToBack(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		std::shared_ptr<EditCommand_sendItemsToBack> command =
			std::make_shared<EditCommand_sendItemsToBack>(
				*this,
				selectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::bringToTop(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		std::shared_ptr<EditCommand_bringItemsToTop> command =
			std::make_shared<EditCommand_bringItemsToTop>(
				*this,
				selectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::sendToBottom(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		std::shared_ptr<EditCommand_sendItemsToBottom> command =
			std::make_shared<EditCommand_sendItemsToBottom>(
				*this,
				selectedItems
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	bool Edit::snapToGrid(void) const
	{
		return _SnapToGrid;
	}

	void Edit::snapToGrid(bool const enable)
	{
		_SnapToGrid = enable;
	}

	Point Edit::toGridPoint(Point const& p) const
	{
		Point toPoint;
		std::int64_t x;
		std::int64_t y;
		bool xMinus = false;
		bool yMinus = false;


		x = static_cast<std::int64_t>(p.X);
		y = static_cast<std::int64_t>(p.Y);

		if (x < 0)
		{
			xMinus = true;
			x = x * -1;
		}
		if (y < 0)
		{
			yMinus = true;
			y = y * -1;
		}

		x = ((x + _GridXSize / 2) / _GridXSize) * _GridXSize;
		y = ((y + _GridYSize / 2) / _GridYSize) * _GridYSize;

		if (xMinus)
		{
			x = x * -1;
		}
		if (yMinus)
		{
			y = y * -1;
		}

		toPoint.X = static_cast<Coord>(x);
		toPoint.Y = static_cast<Coord>(y);

		return toPoint;
	}

	Point Edit::toSnappedPoint(Point const& p) const
	{
		if (snapToGrid())
		{
			return toGridPoint(p);
		}
		return p;
	}

	void Edit::getGridSize(std::uint64_t& x, std::uint64_t& y) const
	{
		x = _GridXSize;
		y = _GridYSize;
	}

	void Edit::setGridSize(std::uint64_t const x, std::uint64_t const y)
	{
		_GridXSize = x;
		_GridYSize = y;
	}

	//=======================================================================
	// Call from Window Command
	void Edit::selectAllItem(void)
	{
		if (model().items().empty())
		{
			return;
		}
		auto selectItems = model().items();
		auto preselectedItems = getSelectedItems();
		std::shared_ptr<EditCommand_selectItems> command =
			std::make_shared<EditCommand_selectItems>(
				*this,
				selectItems,
				preselectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::deselectItem(void)
	{
		if (model().items().empty())
		{
			return;
		}
		std::vector<std::shared_ptr<Item>> emptyItems;
		auto selectItems = emptyItems;
		auto preselectedItems = getSelectedItems();
		std::shared_ptr<EditCommand_selectItems> command =
			std::make_shared<EditCommand_selectItems>(
				*this,
				selectItems,
				preselectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::removeSelectedItem(void)
	{
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}
		std::shared_ptr<EditCommand_deleteItems> command =
			std::make_shared<EditCommand_deleteItems>(
				*this,
				selectedItems
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	// Call from dropNewItem()
	void Edit::addItem(std::shared_ptr<Item> const& item)
	{
		std::shared_ptr<EditCommand_addItem> command =
			std::make_shared<EditCommand_addItem>(
				*this,
				item
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	// Call from EditPointerTool
	bool Edit::isSelectedItem(const std::shared_ptr<Item>& item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
		if (design)
		{
			return design->anchors().visible();
		}
		return false;
	}

	void Edit::selectSingleItem(const std::shared_ptr<Item>& item)
	{
		std::vector<std::shared_ptr<Item>> selectItems;
		selectItems.push_back(item);
		auto preselectedItems = getSelectedItems();
		std::shared_ptr<EditCommand_selectItems> command =
			std::make_shared<EditCommand_selectItems>(
				*this,
				selectItems,
				preselectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::toggleItemSelection(const std::shared_ptr<Item>& item)
	{
		std::shared_ptr<EditCommand_toggleItemSelection> command =
			std::make_shared<EditCommand_toggleItemSelection>(
				*this,
				item
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::selectItemsInBounds(void)
	{
		auto selectItems = getItemsInSelectionBounds();
		if (selectItems.empty())
		{
			deselectItem();
			return;
		}
		auto preselectedItems = getSelectedItems();
		std::shared_ptr<EditCommand_selectItems> command =
			std::make_shared<EditCommand_selectItems>(
				*this,
				selectItems,
				preselectedItems
			);
		editCommandManager().executeCommand(command);
	}

	void Edit::moveSelectedItems(Point offset)
	{
		if (isPointZero(offset))
		{
			return;
		}
		auto selectedItems = getSelectedItems();
		if (selectedItems.empty())
		{
			return;
		}

		auto command =
			editCommandManager().getLastExecutedCommand();

		auto command_moveItems =
			std::dynamic_pointer_cast<EditCommand_moveItems>(command);
		if (command_moveItems)
		{
			if (selectedItems == command_moveItems->getItems())
			{
				command_moveItems->moveOffset(offset);
				return;
			}
		}

		command =
			std::make_shared<EditCommand_moveItems>(
				*this,
				selectedItems,
				offset
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	std::vector<std::shared_ptr<Item>> Edit::getItemsInSelectionBounds(void)
	{
		Point p0;
		Point p1;
		editPointerTool().getSelectBounds(p0, p1);

		std::vector<std::shared_ptr<Item>> selectedItems;
		for (auto& item : model().items())
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				if (arePointsInBounds(p0, p1, design->getPoints()))
				{
					selectedItems.push_back(item);
				}
			}
		}
		return selectedItems;
	}

	std::vector<std::shared_ptr<Item>> Edit::getSelectedItems(void)
	{
		std::vector<std::shared_ptr<Item>> selectedItems;
		for (auto& item : model().items())
		{
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
			if (design)
			{
				if (design->anchors().visible())
				{
					selectedItems.push_back(item);
				}
			}
		}
		return selectedItems;
	}

	//=======================================================================
	// Call from Design.onItemPointerReleased() and Design.onItemPointerMoved()
	void Edit::moveItemAnchor(
		std::shared_ptr<Item> const& item,
		std::shared_ptr<Anchor> const& anchor,
		Point const& position
	)
	{
		auto command =
			editCommandManager().getLastExecutedCommand();

		auto command_moveItemAnchor =
			std::dynamic_pointer_cast<EditCommand_moveItemAnchor>(command);
		if (command_moveItemAnchor)
		{
			if ((item == command_moveItemAnchor->getItem()) &&
				(anchor == command_moveItemAnchor->getAnchor())
				)
			{
				if (position == command_moveItemAnchor->getPosition())
				{
					return;
				}

				bool rv;
				rv = command_moveItemAnchor->moveItemAnchor(position);
				if (!rv)
				{
					editCommandManager().deleteLastExecutedCommand();
				}
				return;
			}
		}

		command =
			std::make_shared<EditCommand_moveItemAnchor>(
				*this,
				item,
				anchor,
				position
			);
		editCommandManager().executeCommand(command);
	}

	//=======================================================================
	// Call from Canvas Drag & Drop Event
	std::shared_ptr<Item> Edit::makeNewItemByFriendlyName(StringView const& friendlyName)
	{
		setPointZero(_NewItemPosition);
		_NewItem = factory().makeItemByFriendlyName(friendlyName);
		return _NewItem;
	}

	void Edit::setNewItem(std::shared_ptr<Item> const& item)
	{
		_NewItem = item;
	}

	void Edit::dragOverNewItem(Coord const px, Coord const py)
	{
		Point pointerPosition(px, py);
		Point position = viewContext().fromWindow(pointerPosition);
		Point snappedPosition = toSnappedPoint(position);
		Point offset = snappedPosition - _NewItemPosition;
		_NewItemPosition = snappedPosition;
		if (_NewItem)
		{
			_NewItem->moveOffset(offset);
			notifyInvalidated();
		}
	}

	void Edit::dropNewItem(Coord const px, Coord const py)
	{
		Point pointerPosition(px, py);
		Point position = viewContext().fromWindow(pointerPosition);
		Point snappedPosition = toSnappedPoint(position);
		Point offset = snappedPosition - _NewItemPosition;
		_NewItemPosition = snappedPosition;
		if (_NewItem)
		{
			deselectItem();


			_NewItem->moveOffset(offset);
			addItem(_NewItem);
			//selectSingleItem(_NewItem);
			std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(_NewItem);
			if (design)
			{
				design->anchors().visible(true);
				notifyItemSelectionChanged();
			}


			_NewItem = nullptr;
		}
	}

	//=======================================================================
	void Edit::reset(void)
	{
		View::reset();
		editSeed().touch();
		editSeed().commit();
	}

	bool Edit::saveFile(std::wstring filePath)
	{
		bool rv = View::saveFile(filePath);
		if (rv)
		{
			editSeed().commit();
		}
		return rv;
	}

	bool Edit::loadFile(std::wstring filePath)
	{
		model().items().clear();
		editCommandManager().clear();


		bool rv = cx::Diagram::loadFile(filePath, *this, true);
		if (!rv)
		{
			CX_RUNTIME_LOG(cxLError) << L"cx::Diagram::loadFile() failed";
		}
		auto& items = model().items();
		for (auto& item : items)
		{
			auto design = std::dynamic_pointer_cast<cx::Diagram::Design>(item);
			if (design)
			{
				design->setEdit(this);
				design->registerEventHandler(eventHandlerRegistry());
				design->updateTarget();
			}
		}


		editSeed().touch();
		editSeed().commit();


		notifyInvalidated();
		notifyItemSelectionChanged();
		notifyModified();

		return rv;
	}
}
