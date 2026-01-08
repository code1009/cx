#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class ShapeStyle
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::shared_ptr<FillStyle> _FillStyle;
		std::shared_ptr<LineStyle> _LineStyle;
		std::shared_ptr<TextStyle> _TextStyle;

	public:
		ShapeStyle();

	public:
		ShapeStyle(const ShapeStyle& other);
		ShapeStyle& operator=(const ShapeStyle& other);
		ShapeStyle(ShapeStyle&& other) noexcept;
		ShapeStyle& operator=(ShapeStyle&& other) noexcept;

	public:
		void notifyPropertyChanged(void);

	private:
		void fill_attachPropertyChangedEventHandler(void);
		void fill_detachPropertyChangedEventHandler(void);
		void fill_onPropertyChanged(cx::ev::Event& event);

		void line_attachPropertyChangedEventHandler(void);
		void line_detachPropertyChangedEventHandler(void);
		void line_onPropertyChanged(cx::ev::Event& event);

		void text_attachPropertyChangedEventHandler(void);
		void text_detachPropertyChangedEventHandler(void);
		void text_onPropertyChanged(cx::ev::Event& event);

	public:
		FillStyle& fill() const;
		LineStyle& line() const;
		TextStyle& text() const;

	public:
		void copyTo(ShapeStyle& dest) const;
	};
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	void copyShapeStyle(
		const ShapeStyle& source,
		ShapeStyle& dest
	);
}
