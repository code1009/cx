#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	class Edit;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Design : public Item
	{
	private:
		Edit* _Edit{ nullptr };

	private:
		std::shared_ptr<Item> _Target;
		std::unique_ptr<Anchors> _Anchors;
		std::shared_ptr<Anchor> _AnchorPointerPressed;
		Point _AnchorPointerPressedPosition;

	public:
		explicit Design(StringView const& className);

	private:
		void anchors_attachPropertyChangedEventHandler(void);
		void anchors_detachPropertyChangedEventHandler(void);
		void anchors_onPropertyChanged(cx::ev::Event& event);
	private:
		void target_attachPropertyChangedEventHandler(void);
		void target_detachPropertyChangedEventHandler(void);
		void target_onPropertyChanged(cx::ev::Event& event);

	public:
		Edit& getEdit() const { return *_Edit; }
		void setEdit(Edit* edit) { _Edit = edit; }

	public:
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual bool isPointIn(const Point& test) override;
		virtual void moveOffset(Point const& offset) override;

	public:
		virtual void registerEventHandler(cx::ev::target::EventHandlerRegistry& eventHandlerRegistry) override;

	public:
		virtual void drawContentUnderlay(DrawingContext& dctx) override;
		virtual void drawContent(DrawingContext& dctx) override;
		virtual void drawContentOverlay(DrawingContext& dctx) override;

	public:
		virtual void onItemPointerPressed(cx::ev::Event& event);
		virtual void onItemPointerReleased(cx::ev::Event& event);
		virtual void onItemPointerDragging(cx::ev::Event& event);

	public:
		virtual Anchors& anchors(void) const { return *_Anchors; };
		virtual void moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& p);

	public:
		std::shared_ptr<Item> getTarget(void) const { return _Target; }
		void setTarget(std::shared_ptr<Item> const& target);
		virtual void updateTarget(void);
	};
}
