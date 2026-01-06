#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Coord const DefaultViewWidth = 1920.0f;
	Coord const DefaultViewHeight = 1000.0f;
	Color const DefaultViewBackgroundFillColor = Colors::Snow();

	//=======================================================================
	class ViewContext
	{
	private:
		Point _OffsetPosition{ 0.0f, 0.0f };
		Coord _Width { DefaultViewWidth  };
		Coord _Height{ DefaultViewHeight };

		float _Scale   { 1.0f };
		float _MinScale{ 1.0f };
		float _MaxScale{ 1.0f };
		float const _ZoomStep{ 0.1f };

		Coord _ScaledWidth { DefaultViewWidth  };
		Coord _ScaledHeight{ DefaultViewHeight };

		Point _WindowScrollOffset{ 0.0f, 0.0f };
		Coord _WindowWidth { 1.0f };
		Coord _WindowHeight{ 1.0f };

		Coord _CanvasWidth{ 1.0f };
		Coord _CanvasHeight{ 1.0f };

	private:
		bool  _PointerCaptured{ false };
		Point _PointerCapturedPosition{ 0.0f, 0.0f };
		Point _PointerReleasedPosition{ 0.0f, 0.0f };
		Point _PointerCurrentPosition{ 0.0f, 0.0f };

	public:
		ViewContext();

	public:
		explicit ViewContext(Coord const w, Coord const h);

	public:
		ViewContext(const ViewContext& other) = delete;
		ViewContext& operator=(const ViewContext& other) = delete;
		ViewContext(ViewContext&& other) noexcept = delete;
		ViewContext& operator=(ViewContext&& other) noexcept = delete;

	public:
		Point offsetPosition() const { return _OffsetPosition; }
		Coord width() const { return _Width; }
		Coord height() const { return _Height; }
		float maxScale(void) { return _MaxScale; }
		float minScale(void) { return _MinScale; }
		float scale(void) const { return _Scale; }
		Coord scaledWidth() const { return _ScaledWidth; }
		Coord scaledHeight() const { return _ScaledHeight; }
		Point windowScrollOffset() const { return _WindowScrollOffset; }
		Coord windowWidth() const { return _WindowWidth; }
		Coord windowHeight() const { return _WindowHeight; }
		Coord canvasWidth() const { return _CanvasWidth; }
		Coord canvasHeight() const { return _CanvasHeight; }

	public:
		bool setSize(Coord const w, Coord const h);

	public:
		bool setScale(float const newScale);
		bool zoomIn(void) { return setScale(scale() + _ZoomStep); }
		bool zoomOut(void) { return setScale(scale() - _ZoomStep); }

	public:
		bool setWindowSize(Coord const w, Coord const h);
		void setWindowScrollOffset(Point const& offset) { _WindowScrollOffset = offset; }

	private:
		void calcScaleRange(void);
		void calcScaledSize(void);
		void calcOffsetPosition(void);
		void calcCanvasSize(void);

	public:
		bool update(void);

	public:
		Point pointerCurrentPosition() const { return _PointerCurrentPosition; }
		void pointerCurrentPosition(Point const& position) { _PointerCurrentPosition = position; }

	public:
		Point fromWindow(Point const& window);
		Point toWindow(Point const& view);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class ViewBackground
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::shared_ptr<LineStyle> _LineStyle;
		std::shared_ptr<FillStyle> _FillStyle;

	public:
		ViewBackground();

	public:
		ViewBackground(const ViewBackground& other) = delete;
		ViewBackground& operator=(const ViewBackground& other) = delete;
		ViewBackground(ViewBackground&& other) noexcept = delete;
		ViewBackground& operator=(ViewBackground&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);

	private:
		void fill_attachPropertyChangedEventHandler(void);
		void fill_detachPropertyChangedEventHandler(void);
		void fill_onPropertyChanged(cx::ev::Event& event);

		void line_attachPropertyChangedEventHandler(void);
		void line_detachPropertyChangedEventHandler(void);
		void line_onPropertyChanged(cx::ev::Event& event);

	public:
		void draw(DrawingContext& dctx, ViewContext& viewContext);

	public:
		LineStyle& lineStyle() const { return *_LineStyle; }
		FillStyle& fillStyle() const { return *_FillStyle; }
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class ViewGrid
	{
	private:
		bool _ShowGridLine{ true };
		bool _ShowCenterLine{ true };
		bool _ShowOutline{ true };
		bool _ShowCoordinate{ false };

	private:
		std::shared_ptr<LineStyle> _Grid0LineStyle;
		std::shared_ptr<LineStyle> _Grid1LineStyle;
		std::shared_ptr<LineStyle> _Grid2LineStyle;
		std::shared_ptr<LineStyle> _Grid3LineStyle;
		std::shared_ptr<TextStyle> _GridTextStyle;

	public:
		ViewGrid();

	public:
		ViewGrid(const ViewGrid& other) = delete;
		ViewGrid& operator=(const ViewGrid& other) = delete;
		ViewGrid(ViewGrid&& other) noexcept = delete;
		ViewGrid& operator=(ViewGrid&& other) noexcept = delete;

	public:
		void showGridLine(bool newValue) { _ShowGridLine = newValue; }
		void showCenterLine(bool newValue) { _ShowCenterLine = newValue; }
		void showOutline(bool newValue) { _ShowOutline = newValue; }
		void showCoordinate(bool newValue) { _ShowCoordinate = newValue; }

		bool showGridLine() const { return _ShowGridLine; }
		bool showCenterLine() const { return _ShowCenterLine; }
		bool showOutline() const { return _ShowOutline; }
		bool showCoordinate() const { return _ShowCoordinate; }

	public:
		void draw(DrawingContext& dctx, ViewContext& viewContext);

	public:
		void drawGridLine(DrawingContext& dctx, ViewContext& viewContext, Coord lineDx, Coord lineDy, std::shared_ptr<LineStyle> const& lineStyle);
		void drawCenterLine(DrawingContext& dctx, ViewContext& viewContext, std::shared_ptr<LineStyle> const& lineStyle);
		void drawOutline(DrawingContext& dctx, ViewContext& viewContext, std::shared_ptr<LineStyle> const& lineStyle);
		void drawCoordinate(DrawingContext& dctx, ViewContext& viewContext, Coord lineDx, Coord lineDy);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class FPSCounter
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> _LastDrawTime;
		std::int64_t                                       _FrameDrawCount{ 0 };
		float                                              _FPS{ 0.0f };

	public:
		FPSCounter() = default;

	public:
		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) noexcept = delete;
		FPSCounter& operator=(FPSCounter&& other) noexcept = delete;

	public:
		void calculate(void);
		float fps() const { return _FPS; }
	};

	//=======================================================================
	class ViewStatus
	{
	private:
		bool _Show{ false };

	private:
		FPSCounter _FPSCounter;

	private:
		std::shared_ptr<FillStyle> _StatusFillStyle;
		std::shared_ptr<LineStyle> _StatusLineStyle;
		std::shared_ptr<TextStyle> _StatusTextStyle;

	public:
		ViewStatus();

	public:
		ViewStatus(const ViewStatus& other) = delete;
		ViewStatus& operator=(const ViewStatus& other) = delete;
		ViewStatus(ViewStatus&& other) noexcept = delete;
		ViewStatus& operator=(ViewStatus&& other) noexcept = delete;

	public:
		void show(bool newValue) { _Show = newValue; }
		bool show() const { return _Show; }

	public:
		void draw(DrawingContext& dctx, ViewContext& viewContext);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class ViewProperties
	{
	private:
		Coord _Width{ DefaultViewWidth };
		Coord _Height{ DefaultViewHeight };
		Color _BackgroundFillColor{ DefaultViewBackgroundFillColor };

	public:
		ViewProperties() = default;

	public:
		ViewProperties(Coord width, Coord height, Color backgroundFillColor) :
			_Width(width),
			_Height(height),
			_BackgroundFillColor(backgroundFillColor)
		{
		}

	public:
		ViewProperties(const ViewProperties& other) = default;
		ViewProperties& operator=(const ViewProperties& other) = default;
		ViewProperties(ViewProperties&& other) noexcept = default;
		ViewProperties& operator=(ViewProperties&& other) noexcept = default;

	public:
		Coord width() const
		{
			return _Width;
		}
		Coord height() const
		{
			return _Height;
		}
		Color backgroundFillColor() const
		{
			return _BackgroundFillColor;
		}
		void width(Coord width)
		{
			_Width = width;
		}
		void height(Coord height)
		{
			_Height = height;
		}
		void backgroundFillColor(Color backgroundFillColor)
		{
			_BackgroundFillColor = backgroundFillColor;
		}
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class View
	{
	public:
		cx::ev::key::EventListener invalidatedEventListener;

	private:
		std::unique_ptr<ViewContext> _ViewContext;
		std::unique_ptr<ViewBackground> _ViewBackground;
		std::unique_ptr<Model> _Model;
		std::unique_ptr<Factory> _Factory;
		std::unique_ptr<EventGenerator> _EventGenerator;

	private:
		std::unique_ptr<cx::ev::target::EventDispatcher> _EventDispatcher;
		std::unique_ptr<cx::ev::target::EventHandlerRegistry> _EventHandlerRegistry;

	public:
		explicit View(Coord const w, Coord const h);

	public:
		virtual ~View() = default;
	
	public:
		View(const View& other) = delete;
		View& operator=(const View& other) = delete;
		View(View&& other) noexcept = delete;
		View& operator=(View&& other) noexcept = delete;

	public:
		void notifyInvalidated(void);

	private:
		void model_attachPropertyChangedEventHandler(void);
		void model_detachPropertyChangedEventHandler(void);
		void model_onPropertyChanged(cx::ev::Event& event);

	private:
		void viewBackground_attachPropertyChangedEventHandler(void);
		void viewBackground_detachPropertyChangedEventHandler(void);
		void viewBackground_onPropertyChanged(cx::ev::Event& event);

	public:
		virtual ViewContext& viewContext() { return *_ViewContext; }
		virtual ViewBackground& viewBackground() { return *_ViewBackground; }
		virtual Factory& factory() { return *_Factory; }
		virtual EventGenerator& eventGenerator() { return *_EventGenerator; }
		virtual Model& model() { return *_Model; }
		virtual cx::ev::target::EventDispatcher& eventDispatcher() { return *_EventDispatcher; }
		virtual cx::ev::target::EventHandlerRegistry& eventHandlerRegistry() { return *_EventHandlerRegistry; }

	protected:
		virtual void setTransform(DrawingContext& dctx);
		virtual void resetTransform(DrawingContext& dctx);

	public:
		virtual void draw(DrawingContext& dctx);

	protected:
		virtual void drawBackground(DrawingContext& dctx);
		virtual void drawForeground(DrawingContext& dctx);
		virtual void drawOverlay(DrawingContext& dctx);

	public:
		virtual void reset(void);
		virtual bool saveFile(std::wstring filePath);
		virtual bool loadFile(std::wstring filePath);
	};
}
