#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	using AnchorType = std::uint32_t;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class AnchorTypes
	{
	public:
		static constexpr AnchorType Unknown = 0;
		static constexpr AnchorType Free = 1;
		static constexpr AnchorType LT = 2;
		static constexpr AnchorType LB = 3;
		static constexpr AnchorType RB = 4;
		static constexpr AnchorType RT = 5;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class AnchorStyle
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::shared_ptr<FillStyle> _FillStyle;
		std::shared_ptr<LineStyle> _LineStyle;
		std::shared_ptr<TextStyle> _TextStyle;

	public:
		AnchorStyle();

	public:
		AnchorStyle(const AnchorStyle& other);
		AnchorStyle& operator=(const AnchorStyle& other);
		AnchorStyle(AnchorStyle&& other) noexcept;
		AnchorStyle& operator=(AnchorStyle&& other) noexcept;

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
		void notifyPropertyChanged(void);

	public:
		FillStyle& fill() const { return *_FillStyle; }
		LineStyle& line() const { return *_LineStyle; }
		TextStyle& text() const { return *_TextStyle; }
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Anchor : public std::enable_shared_from_this<Anchor>
	{
	private:
		Coord const _DefaultSize{ 12.0f };
		Coord const _DefaultHalfSize{ _DefaultSize / 2.0f };

	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		Point& _Point;
		AnchorType _AnchorType;
		std::uint32_t _Id{ 0 };
		String _Text;

	private:
		bool _Visible{ false };

	private:
		std::unique_ptr<AnchorStyle> _AnchorStyle;

	public:
		explicit Anchor(Point& p, AnchorType const anchorType, std::uint32_t const id, StringView const& text = L"");

	public:
		Anchor(const Anchor& other) = delete;
		Anchor& operator=(const Anchor& other) = delete;
		Anchor(Anchor&& other) noexcept = delete;
		Anchor& operator=(Anchor&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);
	private:
		void anchorStyle_attachPropertyChangedEventHandler(void);
		void anchorStyle_detachPropertyChangedEventHandler(void);
		void anchorStyle_onPropertyChanged(cx::ev::Event& event);

	public:
		Point const& point(void) const { return _Point; }
		AnchorType anchorType(void) const { return _AnchorType; }
		std::uint32_t id(void) const { return _Id; }
		String const& text(void) const { return _Text; }
		void text(StringView const& newValue)
		{
			if (_Text == newValue)
			{
				return;
			}
			_Text = newValue;
			notifyPropertyChanged();
		}

	public:
		bool visible(void) const { return _Visible; }
		void visible(bool const newValue);

	public:
		AnchorStyle& anchorStyle(void) const { return *_AnchorStyle; }

	public:
		void calcBounds(Point& p0, Point& p1) const;
		bool isPointIn(Point const& test);

	public:
		void draw(DrawingContext& dctx);
	};
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Anchors
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::vector<std::shared_ptr<Anchor>> _Items;
		bool _Visible{ false };
		std::unique_ptr<AnchorStyle> _AnchorStyle;

	public:
		Anchors();

	public:
		Anchors(const Anchors& other) = delete;
		Anchors& operator=(const Anchors& other) = delete;
		Anchors(Anchors&& other) noexcept = delete;
		Anchors& operator=(Anchors&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);
	private:
		void anchorStyle_attachPropertyChangedEventHandler(void);
		void anchorStyle_detachPropertyChangedEventHandler(void);
		void anchorStyle_onPropertyChanged(cx::ev::Event& event);
	private:
		void anchor_attachPropertyChangedEventHandler(std::shared_ptr<Anchor> const& item);
		void anchor_detachPropertyChangedEventHandler(std::shared_ptr<Anchor> const& item);
		void anchor_onPropertyChanged(cx::ev::Event& event);

	public:
		std::vector<std::shared_ptr<Anchor>>& items(void) { return _Items; }
		bool empty(void) const { return _Items.empty(); }
		std::size_t size(void) const { return _Items.size(); }
		std::shared_ptr<Anchor> operator[](std::size_t const index) const { return at(index); };
		std::shared_ptr<Anchor> at(std::size_t const index) const;
		bool contains(std::shared_ptr<Anchor> const& test);
		std::size_t indexOf(std::shared_ptr<Anchor> const& item) const;

	public:
		void add(std::shared_ptr<Anchor> const& item);
		void insert(std::size_t const index, std::shared_ptr<Anchor> const& item);
		void remove(std::shared_ptr<Anchor> const& item);
		void clear(void);

	public:
		std::shared_ptr<Anchor> find(Point const& test) const;

	public:
		bool visible(void) const { return _Visible; }
		void visible(bool const newValue);

	public:
		AnchorStyle& anchorStyle(void) const { return *_AnchorStyle; }

	public:
		bool isPointIn(const Point& test);

	public:
		void draw(DrawingContext& dctx);

	public:
		void drawPoints(DrawingContext& dctx);

	public:
		void calculatePointsBounds(Point& p0, Point& p1) const;
		void drawPointsBounds(DrawingContext& dctx);
	};
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	bool setRectangleAnchorPoint(
		Point& lt,
		Point& lb,
		Point& rb,
		Point& rt,
		const AnchorType anchorType,
		const Point& p
	);

	//=======================================================================
	bool setHLineAnchorPoint(
		Point& lt,
		Point& rb,
		const AnchorType anchorType,
		const Point& p
	);
	bool setVLineAnchorPoint(
		Point& lt,
		Point& rb,
		const AnchorType anchorType,
		const Point& p
	);
}




