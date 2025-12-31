#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Edit : public View
	{
	public:
		cx::ev::key::EventListener viewPropertiesChangedEventListener;
		cx::ev::key::EventListener itemSelectionChangedEventListener;
		cx::ev::key::EventListener modifiedEventListener;

	private:
		std::unique_ptr<EventGenerator> _EventGenerator;

	private:
		wchar_t const* const _DragDropFormat{ L"cx.Diagram" };
		std::unique_ptr<ViewGrid> _ViewGrid;
		std::unique_ptr<ViewStatus> _ViewStatus;
		std::unique_ptr<EditPointerTool> _EditPointerTool;
		std::unique_ptr<Items> _Clipboard;
		std::size_t _PasteOffsetStep{ 0 };
		EditSeed _EditSeed;

	private:
		std::shared_ptr<Item> _NewItem;
		Point _NewItemPosition;

	private:
		std::uint64_t _GridXSize{ 20 };
		std::uint64_t _GridYSize{ 20 };
		bool _SnapToGrid{ true };

	private:
		std::unique_ptr<EditCommandManager> _EditCommandManager;

	public:
		explicit Edit(Coord const w, Coord const h);

	public:
		void notifyViewPropertiesChanged(void);
		void notifyItemSelectionChanged(void);
		void notifyModified(void);

	private:
		void editSeed_attachModifiedEventHandler(void);
		void editSeed_detachModifiedEventHandler(void);
		void editSeed_onModified(cx::ev::Event& event);

	public:
		virtual EventGenerator& eventGenerator() override { return *_EventGenerator; }

	protected:
		virtual void drawBackground(DrawingContext& dctx) override;
		virtual void drawForeground(DrawingContext& dctx) override;
		virtual void drawOverlay(DrawingContext& dctx) override;

	public:
		wchar_t const* const getDragDropFormat() const { return _DragDropFormat; }

	public:
		EditCommandManager& editCommandManager(void) { return *_EditCommandManager; }
		EditPointerTool& editPointerTool(void) { return *_EditPointerTool; }
		Items& clipboard(void) { return *_Clipboard; }
		std::size_t pasteOffsetStep() const { return _PasteOffsetStep; }
		void pasteOffsetStep(std::size_t const step) { _PasteOffsetStep = step; }
		EditSeed& editSeed(void) { return _EditSeed; }
		ViewGrid& viewGrid(void) { return *_ViewGrid; }
		ViewStatus& viewStatus(void) { return *_ViewStatus; }

	public:
		void setViewProperties(ViewProperties const& viewProperties);
		void commitItemPropertiesChanged(std::shared_ptr<Item> const& item, std::shared_ptr<Item> const& current, std::shared_ptr<Item> const& previous);

	public:
		void undo(void);
		void redo(void);
		void cut(void);
		void copy(void);
		void paste(void);

	public:
		void bringToFront(void);
		void sendToBack(void);
		void bringToTop(void);
		void sendToBottom(void);

	public:
		bool snapToGrid(void) const;
		void snapToGrid(bool const enable);
		Point toGridPoint(Point const& p) const;
		Point toSnappedPoint(Point const& p) const;
		void getGridSize(std::uint64_t& x, std::uint64_t& y) const;
		void setGridSize(std::uint64_t const x, std::uint64_t const y);

	public:
		void selectAllItem(void);
		void deselectItem(void);
		void removeSelectedItem(void);

	public:
		void addItem(std::shared_ptr<Item> const& item);

	public:
		bool isSelectedItem(const std::shared_ptr<Item>& item);
		void selectSingleItem(const std::shared_ptr<Item>& item);
		void toggleItemSelection(const std::shared_ptr<Item>& item);

		void selectItemsInBounds(void);
		void moveSelectedItems(Point offset);

	public:
		std::vector<std::shared_ptr<Item>> getSelectedItems(void);
		std::vector<std::shared_ptr<Item>> getItemsInSelectionBounds(void);

	public:
		void moveItemAnchor(
			std::shared_ptr<Item> const& item,
			std::shared_ptr<Anchor> const& anchor,
			Point const& position
		);

	public:
		std::shared_ptr<Item> makeNewItemByFriendlyName(StringView const& friendlyName);
		void setNewItem(std::shared_ptr<Item> const& item);
		void dragOverNewItem(Coord const px, Coord const py);
		void dropNewItem(Coord const px, Coord const py);

	public:
		virtual void reset(void) override;
		virtual bool saveFile(std::wstring filePath) override;
		virtual bool loadFile(std::wstring filePath) override;
	};
}




