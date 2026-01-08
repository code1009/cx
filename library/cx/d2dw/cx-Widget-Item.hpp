#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Status
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		bool _Pressed{ false };
		bool _Selected{ false };
		bool _Hovered{ false };
		bool _Focused{ false };

	public:
		Status() = default;

	public:
		Status(const Status& other);
		Status& operator=(const Status& other);
		Status(Status&& other) noexcept;
		Status& operator=(Status&& other) noexcept;

	protected:
		void notifyPropertyChanged(void);

	public:
		bool pressed() const { return _Pressed; }
		void pressed(bool const newValue)
		{
			if (_Pressed == newValue)
			{
				return;
			}
			_Pressed = newValue;
			notifyPropertyChanged();
		}
		bool selected() const { return _Selected; }
		void selected(bool const newValue)
		{
			if (_Selected == newValue)
			{
				return;
			}
			_Selected = newValue;
			notifyPropertyChanged();
		}
		bool hovered() const { return _Hovered; }
		void hovered(bool const newValue)
		{
			if (_Hovered == newValue)
			{
				return;
			}
			_Hovered = newValue;
			notifyPropertyChanged();
		}
		bool focused() const { return _Focused; }
		void focused(bool const newValue)
		{
			if (_Focused == newValue)
			{
				return;
			}
			_Focused = newValue;
			notifyPropertyChanged();
		}
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Item : public std::enable_shared_from_this<Item>
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		String _ClassName;
		String _Name;

	private:
		bool _Visible{ true };

	private:
		Status _Status;

	private:
		std::vector<Point> _Points;

	public:
		explicit Item(StringView const& className);

	public:
		virtual ~Item() = default;
	
	public:
		Item(const Item& other) = delete;
		Item& operator=(const Item& other) = delete;
		Item(Item&& other) noexcept = delete;
		Item& operator=(Item&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);
	private:
		void status_attachPropertyChangedEventHandler(void);
		void status_detachPropertyChangedEventHandler(void);
		void status_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual void copyTo(std::shared_ptr<Item> const& dest) const;
		virtual std::shared_ptr<Item> clone() const = 0;

	public:
		String className() const { return _ClassName; }

	public:
		String name() const { return _Name; }
		void name(StringView const& newValue);

		bool visible() const { return _Visible; }
		void visible(bool const newValue);

	public:
		Status& status(void) { return _Status; }

	public:
		virtual std::vector<Point>& getPoints(void);
		virtual void setPoints(std::vector<Point> const& points);

	public:
		virtual Point const getPoint(std::size_t const index) const;
		virtual void setPoint(std::size_t const index, Point const& p);

	public:
		virtual void getBounds(Point& p0, Point& p1);
		virtual void setBounds(Point const& p0, Point const& p1);

	public:
		virtual bool isPointIn(Point const& test);
		virtual void moveOffset(Point const& offset);

	public:
		virtual void registerEventHandler(cx::ev::target::EventHandlerRegistry& eventHandlerRegistry);

	public:
		virtual void drawUnderlay(DrawingContext& dctx);
		virtual void draw(DrawingContext& dctx);
		virtual void drawOverlay(DrawingContext& dctx);

		virtual void drawContentUnderlay(DrawingContext& dctx);
		virtual void drawContent(DrawingContext& dctx);
		virtual void drawContentOverlay(DrawingContext& dctx);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Items
	{
	protected:
		std::vector<std::shared_ptr<Item>> _Items;

	public:
		Items() = default;

	public:
		virtual ~Items() = default;

	public:
		Items(const Items& other) = delete;
		Items& operator=(const Items& other) = delete;
		Items(Items&& other) noexcept = delete;
		Items& operator=(Items&& other) noexcept = delete;

	public:
		std::vector<std::shared_ptr<Item>>& items(void) { return _Items; }
		bool empty(void) const { return _Items.empty(); }
		std::size_t size(void) const { return _Items.size(); }
		std::shared_ptr<Item> operator[](std::size_t const index) { return at(index); };
		std::shared_ptr<Item> at(std::size_t const index) const;
		bool contains(std::shared_ptr<Item> const& test);
		std::size_t indexOf(std::shared_ptr<Item> const& item) const;

	public:
		virtual void add(std::shared_ptr<Item> const& item);
		virtual void insert(std::size_t const index, std::shared_ptr<Item> const& item);
		virtual void remove(std::shared_ptr<Item> const& item);
		virtual void clear();

	public:
		std::shared_ptr<Item> find(const Point& test) const;

	public:
		virtual void draw(DrawingContext& dctx);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Model : public Items
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	public:
		Model() = default;

	public:
		void notifyPropertyChanged(void);
	private:
		void item_attachPropertyChangedEventHandler(std::shared_ptr<Item> const& item);
		void item_detachPropertyChangedEventHandler(std::shared_ptr<Item> const& item);
		void item_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual void add(std::shared_ptr<Item> const& item) override;
		virtual void insert(std::size_t const index, std::shared_ptr<Item> const& item) override;
		virtual void remove(std::shared_ptr<Item> const& item) override;
		virtual void clear() override;

	public:
		std::shared_ptr<Item> findIfVisible(const Point& test) const;
	};
}
