#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class Line : public Item
	{
	private:
		Coord const _MinHitSize{ 10.0f };

	private:
		std::shared_ptr<ShapeStyle> _ShapeStyle;

	public:
		Line();

	private:
		void shapeStyle_attachPropertyChangedEventHandler(void);
		void shapeStyle_detachPropertyChangedEventHandler(void);
		void shapeStyle_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual bool isPointIn(const Point& test) override;

	public:
		virtual void drawContent(DrawingContext& dctx) override;

	public:
		ShapeStyle& shapeStyle() const { return *_ShapeStyle; }
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class LineDesign : public Design
	{
	public:
		LineDesign();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual void moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position) override;

	public:
		virtual void updateTarget(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Line(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_LineDesign(std::shared_ptr<Item> item);
}




