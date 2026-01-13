/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	Button::Button() :
		Base(L"cx.Widget.UIControl.Button")
	{
	}

	//=======================================================================
	std::shared_ptr<Item> Button::clone() const
	{
		auto item = std::make_shared<Button>();
		copyTo(item);
		return item;
	}

	void Button::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Button>(dest);
		Base::copyTo(other);
	}

	//=======================================================================
	void Button::drawContent(DrawingContext& dctx)
	{
		auto& points = getPoints();
		Coord x = points[0].X;
		Coord y = points[0].Y;
		Coord w = points[1].X - points[0].X;
		Coord h = points[1].Y - points[0].Y;
		dctx.FillRectangle(x, y, w, h, uiControlStyle().fill().fillColor());
		dctx.DrawRectangle(x, y, w, h, uiControlStyle().line().lineColor(), uiControlStyle().line().lineSize());

		drawText(dctx);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	ButtonDesign::ButtonDesign() :
		BaseDesign(L"cx.Widget.UIControl.Button.Design")
	{
		auto target = std::make_shared<Button>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> ButtonDesign::clone() const
	{
		auto item = std::make_shared<ButtonDesign>();
		copyTo(item);
		return item;
	}

	void ButtonDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<ButtonDesign>(dest);
		BaseDesign::copyTo(other);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Button(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Button> target = std::dynamic_pointer_cast<Button>(item);
		auto properties = makeProperties_Base(target);
		return properties;
	}

	std::shared_ptr<Properties> makeProperties_ButtonDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<ButtonDesign>(item);
		std::shared_ptr<Button> target = std::dynamic_pointer_cast<Button>(design->getTarget());

		return makeProperties_Button(target);
	}
}
