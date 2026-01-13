/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"

#include <cx/runtime/log_facility/log_facility.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	ViewContext::ViewContext(Coord const w, Coord const h, bool alignCenter) :
		_AlignCenter(alignCenter)
	{
		setSize(w, h);
	}

	ViewContext::ViewContext()
	{
	}

	//=======================================================================
	bool ViewContext::setSize(Coord const w, Coord const h)
	{
		if (w <= 0.0f || h <= 0.0f)
		{
			return false;
		}
		if (_Width == w && _Height == h)
		{
			return false;
		}

		_Width = w;
		_Height = h;

		bool rv = false;
		if (update())
		{
			rv = true;
		}
		if (setScale(1.0f))
		{
			rv = true;
		}
		return rv;
	}

	bool ViewContext::setScale(float const newScale)
	{
		if (newScale < 0.1f)
		{
			return false;
		}

		_Scale = newScale;

		bool rv = false;
		if (update())
		{
			rv = true;
		}
		return rv;
	}

	bool ViewContext::setWindowSize(Coord const w, Coord const h)
	{
		if (w < 1.0f)
		{
			return false;
		}
		if (w < 1.0f)
		{
			return false;
		}
		if (_WindowWidth == w && _WindowHeight == h)
		{
			return false;
		}

		_WindowWidth = w;
		_WindowHeight = h;

		bool rv = false;
		if (update())
		{
			rv = true;
		}
		return rv;
	}
	
	bool ViewContext::setWindowScrollOffset(Point const& offset)
	{
		if (offset.X < 0.0f)
		{
			return false;
		}
		if (offset.Y < 0.0f)
		{
			return false;
		}
		if (_WindowScrollOffset == offset)
		{
			return false;
		}

		_WindowScrollOffset = offset;

		bool rv = false;
		if (update())
		{
			rv = true;
		}
		return rv;
	}

	//=======================================================================
	void ViewContext::calcScaleRange(void)
	{
		// WinRT originate error - 0x80070057 : 
		// 'Cannot create CanvasImageSource sized 16512 x 9288; 
		// MaximumBitmapSizeInPixels for this device is 16384.'.
		// 200% upscale일 때 오류 방지
		Coord cx = 16384 / 2 / _Width;
		Coord cy =  9288 / 2 / _Height;
		float maxScaleF;
		if (cx < cy)
		{
			maxScaleF = cx;
		}
		else
		{
			maxScaleF = cy;
		}
		std::uint32_t maxScaleIx10 = static_cast<std::uint32_t>(maxScaleF * 10.0f);
		std::uint32_t maxScaleI = maxScaleIx10 / 10;
		_MaxScale = static_cast<float>(maxScaleI);
		_MinScale = 0.1f;
	}

	void ViewContext::calcScaledSize(void)
	{
		_ScaledWidth = _Width * _Scale;
		_ScaledHeight = _Height * _Scale;
	}

	void ViewContext::calcOffsetPosition(void)
	{
		if (_ScaledWidth < _WindowWidth)
		{
			_WindowScrollOffset.X = 0.0f;


			if (_AlignCenter)
			{
				_OffsetPosition.X = (_WindowWidth - _ScaledWidth) / 2.0f;
			}
			else
			{
				_OffsetPosition.X = 0.0f;
			}
		}
		else
		{
			if (_WindowScrollOffset.X < 0.0f)
			{
				_WindowScrollOffset.X = 0.0f;
			}
			if (_WindowScrollOffset.X >= (_ScaledWidth - _WindowWidth))
			{
				_WindowScrollOffset.X = _ScaledWidth - _WindowWidth;
			}


			_OffsetPosition.X = -(_WindowScrollOffset.X);
		}
		if (_ScaledHeight < _WindowHeight)
		{
			_WindowScrollOffset.Y = 0.0f;


			if (_AlignCenter)
			{
				_OffsetPosition.Y = (_WindowHeight - _ScaledHeight) / 2.0f;
			}
			else
			{
				_OffsetPosition.Y = 0.0f;
			}
		}
		else
		{
			if (_WindowScrollOffset.Y < 0.0f)
			{
				_WindowScrollOffset.Y = 0.0f;
			}
			if (_WindowScrollOffset.Y >= (_ScaledHeight - _WindowHeight))
			{
				_WindowScrollOffset.Y = _ScaledHeight - _WindowHeight;
			}

			_OffsetPosition.Y = -(_WindowScrollOffset.Y);
		}
	}

	void ViewContext::calcCanvasSize(void)
	{
		_CanvasWidth  = (_ScaledWidth < _WindowWidth) ? _WindowWidth : _ScaledWidth;
		_CanvasHeight = (_ScaledHeight < _WindowHeight) ? _WindowHeight : _ScaledHeight;
	}

	//=======================================================================
	bool ViewContext::update(void)
	{
		bool rv = false;


		float oldScale = _Scale;
		calcScaleRange();
		if (_Scale < _MinScale)
		{
			_Scale = _MinScale;
		}
		if (_Scale > _MaxScale)
		{
			_Scale = _MaxScale;
		}
		if (oldScale != _Scale)
		{
			rv = true;
		}


		Coord oldScaledWidth = _ScaledWidth;
		Coord oldScaledHeight = _ScaledHeight;
		calcScaledSize();
		if (oldScaledWidth != _ScaledWidth)
		{
			rv = true;
		}
		if (oldScaledHeight != _ScaledHeight)
		{
			rv = true;
		}


		Point oldOffsetPosition = _OffsetPosition;
		calcOffsetPosition();
		if (oldOffsetPosition != _OffsetPosition)
		{
			rv = true;
		}

		Coord oldCanvasWidth  = _CanvasWidth;
		Coord oldCanvasHeight = _CanvasHeight;
		calcCanvasSize();
		if (oldCanvasWidth != _CanvasWidth)
		{
			rv = true;
		}
		if (oldCanvasHeight != _CanvasHeight)
		{
			rv = true;
		}

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"ViewContext::update:" << std::endl
			<< std::format(L"    offsetPosition=({:.2f}, {:.2f})", _OffsetPosition.X, _OffsetPosition.Y) << std::endl
			<< std::format(L"    width={:.2f} height={:.2f}", _Width, _Height) << std::endl
			<< std::format(L"    scale={:.2f} min={:.2f} max={:.2f}", _Scale, _MinScale, _MaxScale) << std::endl
			<< std::format(L"    scaledWidth={:.2f}, scaledHeight={:.2f}", _ScaledWidth, _ScaledHeight) << std::endl
			<< std::format(L"    windowWidth={:.2f}, windowHeight={:.2f}", _WindowWidth, _WindowHeight) << std::endl
			<< std::format(L"    canvasWidth={:.2f}, canvasHeight={:.2f}", _CanvasWidth, _CanvasHeight) << std::endl
			;
		cx::runtime::log_facility_flush();
		*/

		return rv;
	}

	//=======================================================================
	Point ViewContext::fromWindow(Point const& window)
	{
		Point view;


		if (_ScaledWidth < _WindowWidth)
		{
			if (_AlignCenter)
			{
				view.X = (window.X / _Scale) - ((_WindowWidth - _ScaledWidth) / _Scale / 2);
			}
			else
			{
				view.X = (window.X / _Scale);
			}
		}
		else
		{
			view.X = (window.X / _Scale) + (_WindowScrollOffset.X / _Scale);
		}

		if (_ScaledHeight < _WindowHeight)
		{
			if (_AlignCenter)
			{
				view.Y = (window.Y / _Scale) - ((_WindowHeight - _ScaledHeight) / _Scale / 2);
			}
			else
			{
				view.Y = (window.Y / _Scale);
			}
		}
		else
		{
			view.Y = (window.Y / _Scale) + (_WindowScrollOffset.Y / _Scale);
		}

		return view;
	}

	Point ViewContext::toWindow(Point const& view)
	{
		Point window;


		if (_ScaledWidth < _WindowWidth)
		{
			if (_AlignCenter)
			{
				window.X = (view.X * _Scale) + ((_WindowWidth - _ScaledWidth) / 2);
			}
			else
			{
				window.X = (view.X * _Scale);
			}
		}
		else
		{
			window.X = (view.X * _Scale) - _WindowScrollOffset.X;
		}

		if (_ScaledHeight < _WindowHeight)
		{
			if (_AlignCenter)
			{
				window.Y = (view.Y * _Scale) + ((_WindowHeight - _ScaledHeight) / 2);
			}
			else
			{
				window.Y = (view.Y * _Scale);
			}
		}
		else
		{
			window.Y = (view.Y * _Scale) - _WindowScrollOffset.Y;
		}

		return window;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	ViewBackground::ViewBackground()
	{
		_FillStyle = std::make_shared<FillStyle>(DefaultViewBackgroundFillColor);
		_LineStyle = std::make_shared<LineStyle>(Colors::Black(), 0.0f);
	}

	//=======================================================================
	void ViewBackground::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"ViewBackground::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			ViewBackground_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void ViewBackground::fill_attachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&ViewBackground::fill_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void ViewBackground::fill_detachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void ViewBackground::fill_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"ViewBackground::fill_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void ViewBackground::line_attachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&ViewBackground::line_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void ViewBackground::line_detachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void ViewBackground::line_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"ViewBackground::line_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void ViewBackground::draw(DrawingContext& dctx, ViewContext& viewContext)
	{
		float viewCx = viewContext.width();
		float viewCy = viewContext.height();
		dctx.FillRectangle(0, 0, viewCx, viewCy, _FillStyle->fillColor());
		dctx.DrawRectangle(0, 0, viewCx, viewCy, _LineStyle->lineColor(), _LineStyle->lineSize());
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	ViewGrid::ViewGrid()
	{
		_Grid0LineStyle = std::make_shared<LineStyle>(Color{ 224, 128, 128, 224 }, 0.125f);
		_Grid1LineStyle = std::make_shared<LineStyle>(Color{ 224, 128, 128, 224 }, 0.25f);
		_Grid2LineStyle = std::make_shared<LineStyle>(Color{ 224, 128, 128, 224 }, 0.5f);
		_Grid3LineStyle = std::make_shared<LineStyle>(Color{ 224, 128, 128, 224 }, 1.0f);
		_GridTextStyle = std::make_shared<TextStyle>(
			Color{ 255, 128, 128, 224 },
			DrawingDefaults::fontFamily,
			10.0f,
			true
		);
	}

	//=======================================================================
	void ViewGrid::draw(DrawingContext& dctx, ViewContext& viewContext)
	{
		//-----------------------------------------------------------------------
		bool isSmallView = (viewContext.scale() < 1.0f) ? true : false;


		//-----------------------------------------------------------------------
		Coord line0Dx = 10.0f;
		Coord line0Dy = 10.0f;
		Coord line1Dx = 20.0f;
		Coord line1Dy = 20.0f;
		Coord line2Dx = 100.0f;
		Coord line2Dy = 100.0f;


		//-----------------------------------------------------------------------
		if (_ShowGridLine)
		{
			if (!isSmallView)
			{
				drawGridLine(
					dctx, viewContext, line0Dx, line0Dy, _Grid0LineStyle
				);
			}
			drawGridLine(
				dctx, viewContext, line1Dx, line1Dy, _Grid1LineStyle
			);
			drawGridLine(
				dctx, viewContext, line2Dx, line2Dy, _Grid2LineStyle
			);
		}


		//-----------------------------------------------------------------------
		if (_ShowCenterLine)
		{
			drawCenterLine(
				dctx, viewContext, _Grid3LineStyle
			);
		}


		//-----------------------------------------------------------------------
		if (_ShowOutline)
		{
			drawOutline(
				dctx, viewContext, _Grid3LineStyle
			);
		}


		//-----------------------------------------------------------------------
		if (_ShowCoordinate && _ShowGridLine)
		{
			if (!isSmallView)
			{
				drawCoordinate(
					dctx, viewContext, line2Dx, line2Dy
				);
			}
		}
	}

	void ViewGrid::drawGridLine(DrawingContext& dctx, ViewContext& viewContext, Coord lineDx, Coord lineDy, std::shared_ptr<LineStyle> const& lineStyle)
	{
		//-----------------------------------------------------------------------
		Coord viewCx;
		Coord viewCy;
		viewCx = viewContext.width();
		viewCy = viewContext.height();


		//-----------------------------------------------------------------------
		Coord ob;
		Coord oe;

		Coord cb;
		Coord ce;
		Coord ci;
		Coord cd;


		//-----------------------------------------------------------------------
		// Draw horizontal lines
		cd = static_cast<Coord>(lineDx);
		cb = static_cast<Coord>(0.0f);
		ce = static_cast<Coord>(viewCx);
		ob = static_cast<Coord>(0.0f);
		oe = static_cast<Coord>(viewCy);
		for (ci = cb; ci < ce; ci += cd)
		{
			dctx.DrawLine(ci, ob, ci, oe, lineStyle->lineColor(), lineStyle->lineSize());
		}
		// Draw vertical lines
		cd = static_cast<Coord>(lineDy);
		cb = static_cast<Coord>(0.0f);
		ce = static_cast<Coord>(viewCy);
		ob = static_cast<Coord>(0.0f);
		oe = static_cast<Coord>(viewCx);
		for (ci = cb; ci < ce; ci += cd)
		{
			dctx.DrawLine(ob, ci, oe, ci, lineStyle->lineColor(), lineStyle->lineSize());
		}
	}

	void ViewGrid::drawCenterLine(DrawingContext& dctx, ViewContext& viewContext, std::shared_ptr<LineStyle> const& lineStyle)
	{
		//-----------------------------------------------------------------------
		Coord viewCx;
		Coord viewCy;
		viewCx = viewContext.width();
		viewCy = viewContext.height();


		//-----------------------------------------------------------------------
		Coord ob;
		Coord oe;

		Coord cb;
		Coord ce;
		Coord ci;


		//-----------------------------------------------------------------------
		// Draw center lines
		cb = static_cast<Coord>(0.0f);
		ce = static_cast<Coord>(viewCx);
		ob = static_cast<Coord>(0.0f);
		oe = static_cast<Coord>(viewCy);
		ci = (ce - cb) / 2.0f;
		dctx.DrawLine(ci, ob, ci, oe, lineStyle->lineColor(), lineStyle->lineSize());

		cb = static_cast<Coord>(0.0f);
		ce = static_cast<Coord>(viewCy);
		ob = static_cast<Coord>(0.0f);
		oe = static_cast<Coord>(viewCx);
		ci = (ce - cb) / 2.0f;
		dctx.DrawLine(ob, ci, oe, ci, lineStyle->lineColor(), lineStyle->lineSize());
	}

	void ViewGrid::drawOutline(DrawingContext& dctx, ViewContext& viewContext, std::shared_ptr<LineStyle> const& lineStyle)
	{
		//-----------------------------------------------------------------------
		Coord viewCx;
		Coord viewCy;
		viewCx = viewContext.width();
		viewCy = viewContext.height();


		//-----------------------------------------------------------------------
		// Draw the outline of a rectangle.
		Coord left = static_cast<Coord>(0.0f);
		Coord top = static_cast<Coord>(0.0f);
		Coord right = static_cast<Coord>(viewCx);
		Coord bottom = static_cast<Coord>(viewCy);
		dctx.DrawRectangle(
			left, top, right - left, bottom - top,
			lineStyle->lineColor(), lineStyle->lineSize()
		);
	}

	void ViewGrid::drawCoordinate(DrawingContext& dctx, ViewContext& viewContext, Coord lineDx, Coord lineDy)
	{
		//-----------------------------------------------------------------------
		Coord viewCx;
		Coord viewCy;
		viewCx = viewContext.width();
		viewCy = viewContext.height();


		//-----------------------------------------------------------------------
		Coord viewportX;
		Coord viewportY;
		Coord viewportCx;
		Coord viewportCy;
		viewportX = -viewContext.offsetPosition().X;
		viewportY = -viewContext.offsetPosition().Y;
		viewportCx = viewContext.offsetPosition().X * 2 + viewContext.width();
		viewportCy = viewContext.offsetPosition().Y * 2 + viewContext.height();


		//-----------------------------------------------------------------------
		Coord left = static_cast<Coord>(0.0f);
		Coord top = static_cast<Coord>(0.0f);
		Coord right = static_cast<Coord>(viewCx);
		Coord bottom = static_cast<Coord>(viewCy);


		//-----------------------------------------------------------------------
		// Draw coordinate text
		Coord tx = static_cast<Coord>(0.0f);
		Coord ty = static_cast<Coord>(0.0f);
		Coord tbx = static_cast<Coord>(0.0f);
		Coord tby = static_cast<Coord>(0.0f);
		Coord tcx = static_cast<Coord>(viewCx);
		Coord tcy = static_cast<Coord>(viewCy);
		Coord tdx = static_cast<Coord>(lineDx);
		Coord tdy = static_cast<Coord>(lineDy);
		wchar_t text[128];
		for (ty = tby; ty < tcy; ty += tdy)
		{
			for (tx = tbx; tx < tcx; tx += tdx)
			{
				left = tx + 2.0f;
				top = ty + 2.0f;
				right = left + 50.0f;
				bottom = top + 15.0f;

				swprintf_s(text, L"(%d,%d)",
					static_cast<int>(tx / tdx),
					static_cast<int>(ty / tdy));

				dctx.DrawText(
					text,
					left, top,
					right - left, bottom - top,
					_GridTextStyle->textColor(),
					_GridTextStyle->textFormat()
				);
			}
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	void FPSCounter::calculate(void)
	{
		_FrameDrawCount++;


		auto currentDrawTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = currentDrawTime - _LastDrawTime;


		if (elapsed.count() >= 1.0f)
		{
			_FPS = _FrameDrawCount / elapsed.count();
			_FrameDrawCount = 0;
			_LastDrawTime = currentDrawTime;
		}
	}

	//=======================================================================
	ViewStatus::ViewStatus()
	{
		_StatusFillStyle = std::make_shared<FillStyle>(Color{ 128, 192, 192, 255 });
		_StatusLineStyle = std::make_shared<LineStyle>(Colors::Blue(), 1.0f);
		_StatusTextStyle = std::make_shared<TextStyle>(
			Colors::Black(),
			DrawingDefaults::fontFamily,
			12.0f,
			true,
			false,
			TextHAlignment::Left,
			TextVAlignment::Center
		);
	}

	//=======================================================================
	void ViewStatus::draw(DrawingContext& dctx, ViewContext& viewContext)
	{
		_FPSCounter.calculate();


		if (!_Show)
		{
			return;
		}


		Point pointerCurrentPosition = viewContext.pointerCurrentPosition();
		Point viewCurrentPosition = viewContext.fromWindow(pointerCurrentPosition);
		Point windowCurrentPosition = viewContext.toWindow(viewCurrentPosition);


		StringStream textStream;
		textStream
			<< L"포인터: "
			<< std::format(L"({:.1f}, {:.1f})",
				pointerCurrentPosition.X,
				pointerCurrentPosition.Y
			)
			<< L" "
			<< L"v="
			<< std::format(L"({:.1f}, {:.1f})",
				viewCurrentPosition.X,
				viewCurrentPosition.Y
			)
			<< L" "
			<< L"w="
			<< std::format(L"({:.1f}, {:.1f})",
				windowCurrentPosition.X,
				windowCurrentPosition.Y
			)
			<< std::endl


			<< L"뷰: "
			<< std::format(L"위치=({:.2f}, {:.2f}) 크기=({:.2f}, {:.2f}) x{:.1f}",
				viewContext.offsetPosition().X,
				viewContext.offsetPosition().Y,
				viewContext.width(),
				viewContext.height(),
				viewContext.scale()
			)
			<< std::endl


			<< L"캔버스: "
			<< std::format(L"({:.2f}, {:.2f})",
				viewContext.canvasWidth(),
				viewContext.canvasHeight()
			)
			<< L" / "
			<< L"FPS: "
			<< std::format(L"{:.2f}",
				_FPSCounter.fps()
			)
			<< std::endl


			<< L"윈도우: "
			<< std::format(L"({:.2f}, {:.2f}) 스크롤=({:.2f}, {:.2f})",
				viewContext.windowWidth(),
				viewContext.windowHeight(),
				viewContext.windowScrollOffset().X,
				viewContext.windowScrollOffset().Y
			)
			<< std::endl
			;


		Coord space = 20.0f;
		Coord boxCx = 500;
		Coord boxCy = 80;
		Coord cx;
		Coord cy;
		cx = boxCx + space * 2;
		cy = boxCy + space * 2;


		Coord windowCx = viewContext.windowWidth();
		Coord windowCy = viewContext.windowHeight();

		Coord left;
		Coord top;
		Coord right;
		Coord bottom;

		//left = space;
		left = (windowCx > cx) ? windowCx - cx + space : space;
		right = left + boxCx;
		//top = space;
		top = (windowCy > cy) ? windowCy - cy + space : space;
		bottom = top + boxCy;


		Coord rounded = 5.0f;
		dctx.FillRoundedRectangle(left, top, right - left, bottom - top, rounded, rounded,
			_StatusFillStyle->fillColor()
		);
		dctx.DrawRoundedRectangle(left, top, right - left, bottom - top, rounded, rounded,
			_StatusLineStyle->lineColor(), _StatusLineStyle->lineSize()
		);

		left += 10.0f;
		top += 20.0f;
		right -= 10.0f;
		bottom -= 10.0f;
		dctx.DrawText(
			textStream.str().c_str(),
			left, top,
			right - left, bottom - top,
			_StatusTextStyle->textColor(),
			_StatusTextStyle->textFormat()
		);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	View::View(Coord const w, Coord const h, bool alignCenter)
	{
		_ViewContext = std::make_unique<ViewContext>(w, h, alignCenter);
		_Model = std::make_unique<Model>();
		_ViewBackground = std::make_unique<ViewBackground>();
		_Factory = std::make_unique<Factory>();
		_EventGenerator = std::make_unique<EventGenerator>(*this, true);

		_EventDispatcher = std::make_unique<cx::ev::target::EventDispatcher>();
		_EventHandlerRegistry = std::make_unique<cx::ev::target::EventHandlerRegistry>(*_EventDispatcher);


		model_attachPropertyChangedEventHandler();
		viewBackground_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	void View::notifyInvalidated(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"View::notifyInvalidated";
		invalidatedEventListener.notify(
			View_InvalidatedEvent,
			nullptr
		);
	}

	//=======================================================================
	void View::model_attachPropertyChangedEventHandler(void)
	{
		_Model->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&View::model_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void View::model_detachPropertyChangedEventHandler(void)
	{
		_Model->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void View::model_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"View::model_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyInvalidated();
	}

	//=======================================================================
	void View::viewBackground_attachPropertyChangedEventHandler(void)
	{
		_ViewBackground->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&View::viewBackground_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void View::viewBackground_detachPropertyChangedEventHandler(void)
	{
		_ViewBackground->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void View::viewBackground_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"View::viewBackground_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyInvalidated();
	}

	//=======================================================================
	void View::setTransform(DrawingContext& dctx)
	{
#if 0
		winrt::Windows::Foundation::Numerics::float3x2 scale = winrt::Windows::Foundation::Numerics::make_float3x2_scale(
			viewContext().scale(),
			viewContext().scale()
		);
		winrt::Windows::Foundation::Numerics::float3x2 translate = winrt::Windows::Foundation::Numerics::make_float3x2_translation(
			viewContext().offsetPosition().X,
			viewContext().offsetPosition().Y
		);
		dctx.Transform(scale * translate);
#endif
		// TODO
		float scale = viewContext().scale();
		Coord translationX = viewContext().offsetPosition().X;
		Coord translationY = viewContext().offsetPosition().Y;


		D2D1::Matrix3x2F matrixScale;
		D2D1::Matrix3x2F matrixTranslate;
		D2D1::Matrix3x2F matrix;
		//matrix = D2D1::Matrix3x2F::Identity();
		matrixScale =
			D2D1::Matrix3x2F::Scale(
				static_cast<FLOAT>(scale),
				static_cast<FLOAT>(scale)
			);
		matrixTranslate =
			D2D1::Matrix3x2F::Translation(
				static_cast<FLOAT>(translationX),
				static_cast<FLOAT>(translationY)
			);
		matrix = matrixScale * matrixTranslate;
		dctx.Transform(matrix);
	}

	void View::resetTransform(DrawingContext& dctx)
	{
		D2D1::Matrix3x2F matrix;
		matrix = D2D1::Matrix3x2F::Identity();
		dctx.Transform(matrix);
	}

	//=======================================================================
	void View::draw(DrawingContext& dctx)
	{
		setTransform(dctx);
		drawBackground(dctx);
		drawForeground(dctx);

		resetTransform(dctx);
		drawOverlay(dctx);
	}

	void View::drawBackground(DrawingContext& dctx)
	{
		viewBackground().draw(dctx, viewContext());
	}

	void View::drawForeground(DrawingContext& dctx)
	{
		model().draw(dctx);
	}

	void View::drawOverlay(DrawingContext& /*dctx*/)
	{
	}

	//=======================================================================
	void View::reset(void)
	{
		model().items().clear();
		viewContext().setSize(DefaultViewWidth, DefaultViewHeight);
		viewContext().setScale(1.0f);
		viewBackground().fillStyle().fillColor(DefaultViewBackgroundFillColor);
		notifyInvalidated();
	}

	bool View::saveFile(std::wstring filePath)
	{
		bool rv = cx::Widget::saveFile(filePath, *this);
		if (!rv)
		{
			CX_RUNTIME_LOG(cxLError) << L"cx::Widget::saveFile() failed";
		}
		return rv;
	}

	bool View::loadFile(std::wstring filePath)
	{
		model().items().clear();
		bool rv = cx::Widget::loadFile(filePath, *this);
		if (!rv)
		{
			CX_RUNTIME_LOG(cxLError) << L"cx::Widget::loadFile() failed";
		}
		auto& items = model().items();
		for (auto& item : items)
		{
			item->registerEventHandler(eventHandlerRegistry());
		}
		notifyInvalidated();
		return rv;
	}
}
