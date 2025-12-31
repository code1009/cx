/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Diagram.hpp"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	BasicTemplate::BasicTemplate(StringView const& className, DrawShapeFunction const& drawShape) :
		Base(className),
		_DrawShape(drawShape)
	{
	}

	//=======================================================================
	std::shared_ptr<Item> BasicTemplate::clone() const
	{
		auto item = std::make_shared<BasicTemplate>(className(), _DrawShape);
		copyTo(item);
		return item;
	}

	//=======================================================================
	void BasicTemplate::drawContent(DrawingContext& dctx)
	{
		_DrawShape(dctx, shared_from_this());
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	BasicTemplateDesign::BasicTemplateDesign(StringView const& className, MakeTargetFunction const& makeTarget) :
		BaseDesign(className),
		_MakeTarget(makeTarget)
	{
		setTarget(_MakeTarget());
	}

	//=======================================================================
	std::shared_ptr<Item> BasicTemplateDesign::clone() const
	{
		auto item = std::make_shared<BasicTemplateDesign>(className(), _MakeTarget);
		copyTo(item);
		return item;
	}
}
