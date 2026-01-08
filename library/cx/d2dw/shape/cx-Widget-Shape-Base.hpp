#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	class Base : public Item
	{
	private:
		String _Text;

	private:
		std::unique_ptr<ShapeStyle> _ShapeStyle;

	public:
		explicit Base(StringView const& className, StringView const& text = L"글");

	private:
		void shapeStyle_attachPropertyChangedEventHandler(void);
		void shapeStyle_detachPropertyChangedEventHandler(void);
		void shapeStyle_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;
	
	public:
		virtual void drawContent(DrawingContext& dctx) override;

	public:
		virtual ShapeStyle& shapeStyle() const { return *_ShapeStyle; }

	public:
		virtual String const& text(void) const { return _Text; }
		virtual void text(StringView const& newValue)
		{
			if (_Text == newValue)
			{
				return;
			}
			_Text = newValue;
			notifyPropertyChanged();
		}

	public:
		virtual void drawText(DrawingContext& dctx);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{
	//=======================================================================
	class BaseDesign : public Design
	{
	public:
		explicit BaseDesign(StringView const& className);

	public:
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual void moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position) override;

	public:
		virtual void updateTarget(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::Shape
{	
	//=======================================================================
	class PropertyFriendlyNames_Base
	{
	public:
		static constexpr StringView shapeStyle_fill = PropertyFriendlyNames::fillStyle;
		static constexpr StringView shapeStyle_line = PropertyFriendlyNames::lineStyle;
		static constexpr StringView shapeStyle_text = PropertyFriendlyNames::textStyle;
		static constexpr StringView text            = PropertyFriendlyNames::text;
	};

	//=======================================================================
	class PropertyNames_Base
	{
	public:
		static constexpr StringView shapeStyle_fill = L"Shape.shapeStyle.fill";
		static constexpr StringView shapeStyle_line = L"Shape.shapeStyle.line";
		static constexpr StringView shapeStyle_text = L"Shape.shapeStyle.text";
		static constexpr StringView text            = L"Shape.text";
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Base(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_BaseDesign(std::shared_ptr<Item> item);
}




