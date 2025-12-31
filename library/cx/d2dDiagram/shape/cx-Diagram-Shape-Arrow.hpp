#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class Arrow : public Item
	{
	private:
		Coord const _MinHitSize{ 10.0f };

	public:
		Coord _headWidth{ 25.0f };
		Coord _headLength{ 25.0f };
		bool _hasLTHead{ false };
		bool _hasRBHead{ true };

	private:
		std::shared_ptr<ShapeStyle> _ShapeStyle;

	public:
		Arrow();

	private:
		void attachPropertyChangedEventHandler(void);
		void detachPropertyChangedEventHandler(void);
		void onPropertyChanged(cx::ev::Event& event);

	private:
		void shapeStyle_attachPropertyChangedEventHandler(void);
		void shapeStyle_detachPropertyChangedEventHandler(void);
		void shapeStyle_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual bool isPointIn(const Point& test) override;

	public:
		virtual void drawContent(DrawingContext& dctx) override;

	public:
		ShapeStyle& shapeStyle() const { return *_ShapeStyle; }

	public:
		virtual void updatePoints(void);

	public:
		virtual void drawLine(DrawingContext& dctx);
		virtual void drawLTHead(DrawingContext& dctx);
		virtual void drawRBHead(DrawingContext& dctx);

	public:
		Coord headWidth(void) const { return _headWidth; }
		void headWidth(Coord const newValue)
		{
			if (_headWidth == newValue)
			{
				return;
			}
			_headWidth = newValue;
			notifyPropertyChanged();
		}
		Coord headLength(void) const { return _headLength; }
		void headLength(Coord const newValue)
		{
			if (_headLength == newValue)
			{
				return;
			}
			_headLength = newValue;
			notifyPropertyChanged();
		}
		bool hasLTHead(void) const { return _hasLTHead; }
		void hasLTHead(bool const newValue)
		{
			if (_hasLTHead == newValue)
			{
				return;
			}
			_hasLTHead = newValue;
			notifyPropertyChanged();
		}
		bool hasRBHead(void) const { return _hasRBHead; }
		void hasRBHead(bool const newValue)
		{
			if (_hasRBHead == newValue)
			{
				return;
			}
			_hasRBHead = newValue;
			notifyPropertyChanged();
		}
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	class ArrowDesign : public Design
	{
	public:
		ArrowDesign();

	public:
		virtual std::shared_ptr<Item> clone() const override;
		virtual void copyTo(std::shared_ptr<Item> const& dest) const override;

	public:
		virtual void moveAnchor(std::shared_ptr<Anchor>& anchor, const Point& position) override;

	public:
		virtual void updateTarget(void) override;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram::Shape
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Arrow(std::shared_ptr<Item> item);
	std::shared_ptr<Properties> makeProperties_ArrowDesign(std::shared_ptr<Item> item);
}




