#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand : public std::enable_shared_from_this<EditCommand>
	{
	public:
		EditCommand() = default;
		virtual ~EditCommand() = default;

	public:
		EditCommand(const EditCommand& other) = delete;
		EditCommand& operator=(const EditCommand& other) = delete;
		EditCommand(EditCommand&& other) noexcept = delete;
		EditCommand& operator=(EditCommand&& other) noexcept = delete;

	public:
		virtual void execute(void) = 0;
		virtual void undo(void) = 0;

	public:
		virtual bool affectsModification(void) const { return true; }
	};
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Edit;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_setViewProperties : public EditCommand
	{
	private:
		Edit& _Edit;
		ViewProperties _Current;
		ViewProperties _Previous;

	public:
		explicit EditCommand_setViewProperties(
			Edit& edit,
			ViewProperties const& current,
			ViewProperties const& previous
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_commitItemPropertiesChanged : public EditCommand
	{
	private:
		Edit& _Edit;
		std::shared_ptr<Item> _Item;
		std::shared_ptr<Item> _Current;
		std::shared_ptr<Item> _Previous;

	public:
		explicit EditCommand_commitItemPropertiesChanged(
			Edit& edit,
			std::shared_ptr<Item> const& item,
			std::shared_ptr<Item> const& current,
			std::shared_ptr<Item> const& previous
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;

	public:
		std::shared_ptr<Item> item(void) const { return _Item; }
		std::shared_ptr<Item> current(void) const { return _Current; }
		std::shared_ptr<Item> previous(void) const { return _Previous; }
		void current(std::shared_ptr<Item> const& current) { _Current = current; };
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_pasteItems : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;

	private:
		std::vector<std::shared_ptr<Item>> _PasteItems;

	public:
		explicit EditCommand_pasteItems(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_addItem : public EditCommand
	{
	private:
		Edit& _Edit;
		std::shared_ptr<Item> _Item;

	public:
		explicit EditCommand_addItem(
			Edit& edit,
			std::shared_ptr<Item> const& item
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_deleteItems : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;

	private:
		std::map<std::size_t, std::shared_ptr<Item>> _ItemMap;

	public:
		explicit EditCommand_deleteItems(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_moveItems : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;
		Point _Offset;

	public:
		explicit EditCommand_moveItems(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items,
			Point const& offset
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;

	public:
		std::vector<std::shared_ptr<Item>> getItems(void) { return _Items; };
		void moveOffset(Point& offset);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_bringItemsToFront : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;

	private:
		std::map<std::size_t, std::shared_ptr<Item>> _ItemMap;

	public:
		explicit EditCommand_bringItemsToFront(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_sendItemsToBack : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;

	private:
		std::map<std::size_t, std::shared_ptr<Item>> _ItemMap;

	public:
		explicit EditCommand_sendItemsToBack(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_bringItemsToTop : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;

	private:
		std::map<std::size_t, std::shared_ptr<Item>> _ItemMap;

	public:
		explicit EditCommand_bringItemsToTop(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_sendItemsToBottom : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _Items;

	private:
		std::map<std::size_t, std::shared_ptr<Item>> _ItemMap;

	public:
		explicit EditCommand_sendItemsToBottom(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& items
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_selectItems : public EditCommand
	{
	private:
		Edit& _Edit;
		std::vector<std::shared_ptr<Item>> _SelectItems;
		std::vector<std::shared_ptr<Item>> _PreselectedItems;

	public:
		explicit EditCommand_selectItems(
			Edit& edit,
			std::vector<std::shared_ptr<Item>> const& selectItems,
			std::vector<std::shared_ptr<Item>> const& preselectedItems
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;

	public:
		virtual bool affectsModification(void) const override{ return false; }
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_toggleItemSelection : public EditCommand
	{
	private:
		Edit& _Edit;
		std::shared_ptr<Item> _Item;

	private:
		bool _IsSelected{ false };

	public:
		explicit EditCommand_toggleItemSelection(
			Edit& edit,
			std::shared_ptr<Item> const& item
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;

	public:
		virtual bool affectsModification(void) const override { return false; }
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommand_moveItemAnchor : public EditCommand
	{
	private:
		Edit& _Edit;
		std::shared_ptr<Item> _Item;
		std::shared_ptr<Anchor> _Anchor;
		Point _Position;

	private:
		std::vector<Point> _OldTargetPoints;
		std::vector<Point> _OldDesignPoints;

		std::vector<Point> _NewTargetPoints;
		std::vector<Point> _NewDesignPoints;

	public:
		explicit EditCommand_moveItemAnchor(
			Edit& edit,
			std::shared_ptr<Item> const& item,
			std::shared_ptr<Anchor> const& anchor,
			Point const& position
		);

	public:
		virtual void execute(void) override;
		virtual void undo(void) override;

	public:
		std::shared_ptr<Item> getItem(void) { return _Item; };
		std::shared_ptr<Anchor> getAnchor(void) { return _Anchor; };
		Point getPosition(void) { return _Position; };

	public:
		bool moveItemAnchor(const Point& point);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class EditCommandManager
	{
	private:
		Edit& _Edit;

	private:
		class EditCommandRecord
		{
		public:
			std::shared_ptr<EditCommand> command;
			EditSeed::Seed currentSeed;
			EditSeed::Seed previousSeed;

		public:
			EditCommandRecord(const EditCommandRecord& other) = default;
			EditCommandRecord& operator=(const EditCommandRecord& other) = default;
			EditCommandRecord(EditCommandRecord&& other) noexcept = default;
			EditCommandRecord& operator=(EditCommandRecord&& other) noexcept = default;

		public:
			EditCommandRecord(
				std::shared_ptr<EditCommand> cmd, 
				EditSeed::Seed cs, 
				EditSeed::Seed ps
			) :
				command(std::move(cmd)), 
				currentSeed(cs), 
				previousSeed(ps)
			{
			}
		};

	private:
		std::stack<EditCommandRecord> _UndoStack;
		std::stack<EditCommandRecord> _RedoStack;

	public:
		explicit EditCommandManager(Edit& edit);

	public:
		virtual ~EditCommandManager() = default;

	public:
		EditCommandManager(const EditCommandManager& other) = delete;
		EditCommandManager& operator=(const EditCommandManager& other) = delete;
		EditCommandManager(EditCommandManager&& other) noexcept = delete;
		EditCommandManager& operator=(EditCommandManager&& other) noexcept = delete;

	public:
		void executeCommand(std::shared_ptr<EditCommand> command);

	public:
		bool undo(void);
		bool redo(void);

	public:
		void clear(void);

	public:
		void addCommand(std::shared_ptr<EditCommand> command);
		std::shared_ptr<EditCommand> getLastExecutedCommand(void);
		void deleteLastExecutedCommand(void);
	};
}




