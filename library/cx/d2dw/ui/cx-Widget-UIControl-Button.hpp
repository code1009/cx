#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	class Button : public Base
	{
	public:
		Button();

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
	class ButtonDesign : public BaseDesign
	{
	public:
		ButtonDesign();

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
	std::shared_ptr<Properties> makeProperties_Button(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_ButtonDesign(std::shared_ptr<Item> item);
}





