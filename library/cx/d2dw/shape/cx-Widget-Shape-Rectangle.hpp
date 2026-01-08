#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	class Rectangle : public Base
	{
	public:
		Rectangle();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual void drawContent(DrawingContext& dctx) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	class RectangleDesign : public BaseDesign
	{
	public:
		RectangleDesign();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Rectangle(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_RectangleDesign(std::shared_ptr<Item> item);
}





