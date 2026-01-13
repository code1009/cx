/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	Label::Label() :
		Base(L"cx.Widget.UIControl.Label")
	{
	}

	//=======================================================================
	std::shared_ptr<Item> Label::clone() const
	{
		auto item = std::make_shared<Label>();
		copyTo(item);
		return item;
	}

	void Label::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Label>(dest);
		Base::copyTo(other);
	}

	//=======================================================================
	void Label::drawContent(DrawingContext& dctx)
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
	LabelDesign::LabelDesign() :
		BaseDesign(L"cx.Widget.UIControl.Label.Design")
	{
		auto target = std::make_shared<Label>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> LabelDesign::clone() const
	{
		auto item = std::make_shared<LabelDesign>();
		copyTo(item);
		return item;
	}

	void LabelDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<LabelDesign>(dest);
		BaseDesign::copyTo(other);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Label(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Label> target = std::dynamic_pointer_cast<Label>(item);
		auto properties = makeProperties_Base(target);
		return properties;
	}

	std::shared_ptr<Properties> makeProperties_LabelDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<LabelDesign>(item);
		std::shared_ptr<Label> target = std::dynamic_pointer_cast<Label>(design->getTarget());

		return makeProperties_Label(target);
	}
}
