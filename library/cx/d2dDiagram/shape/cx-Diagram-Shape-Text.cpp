/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "../cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	Text::Text() :
		Base(L"cx.Diagram.Shape.Text", L"글")
	{
		shapeStyle().text().textColor(Color(255, 0, 0, 0));
	}

	//=======================================================================
	std::shared_ptr<Item> Text::clone() const
	{
		auto item = std::make_shared<Text>();
		copyTo(item);
		return item;
	}

	void Text::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Text>(dest);
		Base::copyTo(other);
		other->text(text());
	}

	//=======================================================================
	void Text::drawContent(DrawingContext& dctx)
	{
		drawText(dctx);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	TextDesign::TextDesign() :
		BaseDesign(L"cx.Diagram.Shape.Text.Design")
	{
		auto target = std::make_shared<Text>();
		setTarget(target);
	}

	//=======================================================================
	std::shared_ptr<Item> TextDesign::clone() const
	{
		auto item = std::make_shared<TextDesign>();
		copyTo(item);
		return item;
	}

	void TextDesign::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<TextDesign>(dest);
		BaseDesign::copyTo(other);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class PropertiesBuilder_Text
	{
	private:
		std::shared_ptr<Properties>& _Properties;

	public:
		PropertiesBuilder_Text(std::shared_ptr<Properties>& properties) :
			_Properties(properties)
		{
		}

	public:
		void build(std::shared_ptr<Text> /*item*/)
		{
			auto property_shapeStyle_fill = _Properties->find(PropertyNames_Base::shapeStyle_fill);
			if (property_shapeStyle_fill)
			{
				property_shapeStyle_fill->visible(false);
			}
			auto property_shapeStyle_line = _Properties->find(PropertyNames_Base::shapeStyle_line);
			if (property_shapeStyle_line)
			{
				property_shapeStyle_line->visible(false);
			}
		}
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Text(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Text> target = std::dynamic_pointer_cast<Text>(item);
		auto properties = makeProperties_Base(target);

		PropertiesBuilder_Text propertiesBuilder(properties);
		propertiesBuilder.build(target);

		return properties;
	}

	std::shared_ptr<Properties> makeProperties_TextDesign(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<TextDesign>(item);
		std::shared_ptr<Text> target = std::dynamic_pointer_cast<Text>(design->getTarget());

		return makeProperties_Text(target);
	}
}



