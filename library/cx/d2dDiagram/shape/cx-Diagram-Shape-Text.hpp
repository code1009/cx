#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class Text : public Base
	{
	public:
		Text();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual void drawContent(DrawingContext& dctx) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class TextDesign : public BaseDesign
	{
	public:
		TextDesign();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Text(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_TextDesign(std::shared_ptr<Item> item);
}

