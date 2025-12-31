#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class BasicTemplate : public Base
	{
	public:
		using DrawShapeFunction = std::function<void(DrawingContext&, std::shared_ptr<Item>)>;

	private:
		DrawShapeFunction _DrawShape;

	public:
		explicit BasicTemplate(StringView const& className, DrawShapeFunction const& drawShape);

	public:
		virtual std::shared_ptr<Item> clone() const override;

	public:
		virtual void drawContent(DrawingContext& dctx) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class BasicTemplateDesign : public BaseDesign
	{
	public:
		using MakeTargetFunction = std::function<std::shared_ptr<Item>(void)>;

	private:
		MakeTargetFunction _MakeTarget;

	public:
		explicit BasicTemplateDesign(StringView const& className, MakeTargetFunction const& makeTarget);

	public:
		virtual std::shared_ptr<Item> clone() const override;
	};
}
