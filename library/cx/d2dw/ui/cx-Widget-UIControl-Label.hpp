#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	class Label : public Base
	{
	public:
		Label();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual void drawContent(DrawingContext& dctx) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	class LabelDesign : public BaseDesign
	{
	public:
		LabelDesign();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Label(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_LabelDesign(std::shared_ptr<Item> item);
}





