#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	class EditPointerToolStyle
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::shared_ptr<FillStyle> _FillStyle;
		std::shared_ptr<LineStyle> _LineStyle;
		std::shared_ptr<TextStyle> _TextStyle;

	public:
		EditPointerToolStyle();

	public:
		EditPointerToolStyle(const EditPointerToolStyle& other);
		EditPointerToolStyle& operator=(const EditPointerToolStyle& other);
		EditPointerToolStyle(EditPointerToolStyle&& other) noexcept;
		EditPointerToolStyle& operator=(EditPointerToolStyle&& other) noexcept;

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
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	class Edit;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	class EditPointerTool
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	protected:
		Edit& _Edit;

	protected:
		// move and select
		Point _p0{};
		Point _p1{};
		Point _PointerPressedPosition{};

	protected:
		bool _Action_SelectMultiple{ false };
		bool _Action_SelectSingle{ false };
		bool _Action_ToggleSelection{ false };
		bool _Action_MoveSelection{ false };

	private:
		std::unique_ptr<EditPointerToolStyle> _EditPointerToolStyle;

	public:
		explicit EditPointerTool(Edit& _Edit);

	public:
		virtual ~EditPointerTool() = default;

	public:
		EditPointerTool(const EditPointerTool& other) = delete;
		EditPointerTool& operator=(const EditPointerTool& other) = delete;
		EditPointerTool(EditPointerTool&& other) noexcept = delete;
		EditPointerTool& operator=(EditPointerTool&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);
	private:
		void editPointerToolStyle_attachPropertyChangedEventHandler(void);
		void editPointerToolStyle_detachPropertyChangedEventHandler(void);
		void editPointerToolStyle_onPropertyChanged(cx::ev::Event& event);

	public:
		Edit& getEdit() const { return _Edit; }

	public:
		void getSelectBounds(Point& p0, Point& p1);

	public:
		void registerEventHandler(void);

	public:
		void onPointerMoved(cx::ev::Event& event);
		void onPointerLButtonDown(cx::ev::Event& event);
		void onPointerLButtonUp(cx::ev::Event& event);

	protected:
		void drawSelectBounds(DrawingContext& dctx);

	public:
		void draw(DrawingContext& dctx);
	};
}




