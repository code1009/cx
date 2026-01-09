#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget::UIControl
{
	//=======================================================================
	class Base : public Item
	{
	private:
		String _Text;

	private:
		std::unique_ptr<UIControlStyle> _UIControlStyle;

	public:
		explicit Base(StringView const& className, StringView const& text = L"글");

	private:
		void uiControlStyle_attachPropertyChangedEventHandler(void);
		void uiControlStyle_detachPropertyChangedEventHandler(void);
		void uiControlStyle_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;
	
	public:
		virtual void drawContent(DrawingContext& dctx) override;

	public:
		virtual UIControlStyle& uiControlStyle() const { return *_UIControlStyle; }

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
namespace cx::Widget::UIControl
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
namespace cx::Widget::UIControl
{	
	//=======================================================================
	class PropertyFriendlyNames_Base
	{
	public:
		static constexpr StringView uiControlStyle_fill = PropertyFriendlyNames::fillStyle;
		static constexpr StringView uiControlStyle_line = PropertyFriendlyNames::lineStyle;
		static constexpr StringView uiControlStyle_text = PropertyFriendlyNames::textStyle;
		static constexpr StringView text                = PropertyFriendlyNames::text;
	};

	//=======================================================================
	class PropertyNames_Base
	{
	public:
		static constexpr StringView uiControlStyle_fill = L"UIControl.uiControlStyle.fill";
		static constexpr StringView uiControlStyle_line = L"UIControl.uiControlStyle.line";
		static constexpr StringView uiControlStyle_text = L"UIControl.uiControlStyle.text";
		static constexpr StringView text                = L"UIControl.text";
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Base(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_BaseDesign(std::shared_ptr<Item> item);
}




