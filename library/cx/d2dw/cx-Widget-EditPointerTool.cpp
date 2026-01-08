/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditPointerToolStyle::EditPointerToolStyle()
	{
		_FillStyle = std::make_shared<FillStyle>(Color{ 128, 224, 224, 255 });
		_LineStyle = std::make_shared<LineStyle>(Colors::Blue(), 1.0f);
		_TextStyle = std::make_shared<TextStyle>(
			Colors::Black(),
			DrawingDefaults::fontFamily,
			12.0f
		);

		fill_attachPropertyChangedEventHandler();
		line_attachPropertyChangedEventHandler();
		text_attachPropertyChangedEventHandler();
	}

	//=======================================================================
	EditPointerToolStyle::EditPointerToolStyle(const EditPointerToolStyle& other) :
		//propertyChangedEventListener(other.propertyChangedEventListener),
		_FillStyle(nullptr),
		_LineStyle(nullptr),
		_TextStyle(nullptr)
	{
		if (other._FillStyle) _FillStyle = std::make_shared<FillStyle>(*other._FillStyle);
		if (other._LineStyle) _LineStyle = std::make_shared<LineStyle>(*other._LineStyle);
		if (other._TextStyle) _TextStyle = std::make_shared<TextStyle>(*other._TextStyle);
	}

	EditPointerToolStyle& EditPointerToolStyle::operator=(const EditPointerToolStyle& other)
	{
		if (this != &other)
		{
			//propertyChangedEventListener = other.propertyChangedEventListener;

			if (other._FillStyle)
				_FillStyle = std::make_shared<FillStyle>(*other._FillStyle);
			else
				_FillStyle.reset();

			if (other._LineStyle)
				_LineStyle = std::make_shared<LineStyle>(*other._LineStyle);
			else
				_LineStyle.reset();

			if (other._TextStyle)
				_TextStyle = std::make_shared<TextStyle>(*other._TextStyle);
			else
				_TextStyle.reset();
		}
		return *this;
	}

	EditPointerToolStyle::EditPointerToolStyle(EditPointerToolStyle&& other) noexcept :
		//propertyChangedEventListener(std::move(other.propertyChangedEventListener)),
		_FillStyle(std::move(other._FillStyle)),
		_LineStyle(std::move(other._LineStyle)),
		_TextStyle(std::move(other._TextStyle))
	{
	}

	EditPointerToolStyle& EditPointerToolStyle::operator=(EditPointerToolStyle&& other) noexcept
	{
		if (this != &other)
		{
			//propertyChangedEventListener = std::move(other.propertyChangedEventListener);
			_FillStyle = std::move(other._FillStyle);
			_LineStyle = std::move(other._LineStyle);
			_TextStyle = std::move(other._TextStyle);
		}
		return *this;
	}

	//=======================================================================
	void EditPointerToolStyle::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"EditPointerToolStyle::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			EditPointerToolStyle_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void EditPointerToolStyle::fill_attachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&EditPointerToolStyle::fill_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void EditPointerToolStyle::fill_detachPropertyChangedEventHandler(void)
	{
		_FillStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void EditPointerToolStyle::fill_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"EditPointerToolStyle::fill_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void EditPointerToolStyle::line_attachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&EditPointerToolStyle::line_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void EditPointerToolStyle::line_detachPropertyChangedEventHandler(void)
	{
		_LineStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void EditPointerToolStyle::line_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"EditPointerToolStyle::line_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void EditPointerToolStyle::text_attachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&EditPointerToolStyle::text_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void EditPointerToolStyle::text_detachPropertyChangedEventHandler(void)
	{
		_TextStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void EditPointerToolStyle::text_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"EditPointerToolStyle::text_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	FillStyle& EditPointerToolStyle::fill() const
	{
		return *_FillStyle;
	}

	LineStyle& EditPointerToolStyle::line() const
	{
		return *_LineStyle;
	}

	TextStyle& EditPointerToolStyle::text() const
	{
		return *_TextStyle;
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	EditPointerTool::EditPointerTool(Edit& edit) :
		_Edit{ edit }
	{
		_EditPointerToolStyle = std::make_unique<EditPointerToolStyle>();
		editPointerToolStyle_attachPropertyChangedEventHandler();

		registerEventHandler();
	}

	//=======================================================================
	void EditPointerTool::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"EditPointerTool::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			EditPointerTool_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void EditPointerTool::editPointerToolStyle_attachPropertyChangedEventHandler(void)
	{
		_EditPointerToolStyle->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&EditPointerTool::editPointerToolStyle_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void EditPointerTool::editPointerToolStyle_detachPropertyChangedEventHandler(void)
	{
		_EditPointerToolStyle->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void EditPointerTool::editPointerToolStyle_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"EditPointerTool::editPointerToolStyle_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void EditPointerTool::getSelectBounds(Point& p0, Point& p1)
	{
		p0 = _p0;
		p1 = _p1;
	}

	//=======================================================================
	void EditPointerTool::registerEventHandler(void)
	{
		_Edit.eventHandlerRegistry().registerEventHandler(
			PointerMovedEvent,
			nullptr,
			std::bind(&EditPointerTool::onPointerMoved, this, std::placeholders::_1)
		);
		_Edit.eventHandlerRegistry().registerEventHandler(
			PointerLButtonDownEvent,
			nullptr,
			std::bind(&EditPointerTool::onPointerLButtonDown, this, std::placeholders::_1)
		);
		_Edit.eventHandlerRegistry().registerEventHandler(
			PointerLButtonUpEvent,
			nullptr,
			std::bind(&EditPointerTool::onPointerLButtonUp, this, std::placeholders::_1)
		);
	}

	//=======================================================================
	void EditPointerTool::onPointerMoved(cx::ev::Event& event)
	{
		std::shared_ptr<PointerEventData> eventData = event.eventDataAs<PointerEventData>();


		_p1 = eventData->_PointerPosition;


		Point _PointerPoint = getEdit().toSnappedPoint(eventData->_PointerPosition);
		Point offset = _PointerPoint - _PointerPressedPosition;
		_PointerPressedPosition = _PointerPoint;


		if (_Action_SelectSingle)
		{
			getEdit().moveSelectedItems(offset);
		}
		if (_Action_MoveSelection)
		{
			getEdit().moveSelectedItems(offset);
		}
	}

	void EditPointerTool::onPointerLButtonDown(cx::ev::Event& event)
	{
		std::shared_ptr<PointerEventData> eventData = event.eventDataAs<PointerEventData>();


		_p0 = eventData->_PointerPosition;
		_p1 = eventData->_PointerPosition;


		Point _PointerPoint = getEdit().toSnappedPoint(eventData->_PointerPosition);
		_PointerPressedPosition = _PointerPoint;


		_Action_SelectMultiple = false;
		_Action_SelectSingle = false;
		_Action_ToggleSelection = false;
		_Action_MoveSelection = false;


		auto item = getEdit().model().find(eventData->_PointerPosition);
		auto design = std::dynamic_pointer_cast<Design>(item);


		bool designAnchorPressed = false;
		if (design)
		{
			if (design->anchors().isPointIn(eventData->_PointerPosition))
			{
				designAnchorPressed = true;
			}
		}
		if (designAnchorPressed)
		{
			return;
		}


		if ((!eventData->_ShiftKeyPressed) && (!eventData->_CtrlKeyPressed))
		{
			if (nullptr == design)
			{
				_Action_SelectMultiple = true;
			}
			else
			{
				if (getEdit().isSelectedItem(item))
				{
					_Action_MoveSelection = true;
				}
				else
				{
					_Action_SelectSingle = true;
				}
			}
		}
		else if ((eventData->_ShiftKeyPressed) && (!eventData->_CtrlKeyPressed))
		{
			_Action_SelectMultiple = true;
		}
		else if ((!eventData->_ShiftKeyPressed) && (eventData->_CtrlKeyPressed))
		{
			if (nullptr != design)
			{
				_Action_ToggleSelection = true;
			}
		}


		if (_Action_SelectSingle)
		{
			getEdit().selectSingleItem(item);
		}
		if (_Action_ToggleSelection)
		{
			getEdit().toggleItemSelection(item);
		}
	}

	void EditPointerTool::onPointerLButtonUp(cx::ev::Event& event)
	{
		std::shared_ptr<PointerEventData> eventData = event.eventDataAs<PointerEventData>();


		_p1 = eventData->_PointerPosition;


		Point _PointerPoint = getEdit().toSnappedPoint(eventData->_PointerPosition);
		Point offset = _PointerPoint - _PointerPressedPosition;
		_PointerPressedPosition = _PointerPoint;


		if (_Action_SelectMultiple)
		{
			getEdit().selectItemsInBounds();
		}
		if (_Action_SelectSingle)
		{
			getEdit().moveSelectedItems(offset);
		}
		if (_Action_MoveSelection)
		{
			getEdit().moveSelectedItems(offset);
		}


		_Action_SelectMultiple = false;
		_Action_SelectSingle = false;
		_Action_ToggleSelection = false;
		_Action_MoveSelection = false;


		setPointZero(_PointerPressedPosition);
		setPointZero(_p0);
		setPointZero(_p1);
	}

	//=======================================================================
	void EditPointerTool::drawSelectBounds(DrawingContext& dctx)
	{
		//-------------------------------------------------------------------
		Point p0;
		Point p1;
		calculateBounds(_p0, _p1, p0, p1);


		//-------------------------------------------------------------------
		Coord x = p0.X;
		Coord y = p0.Y;
		Coord w = p1.X - p0.X;
		Coord h = p1.Y - p0.Y;
		dctx.FillRectangle(x, y, w, h, _EditPointerToolStyle->fill().fillColor());
		dctx.DrawRectangle(x, y, w, h, _EditPointerToolStyle->line().lineColor(), _EditPointerToolStyle->line().lineSize());
	}

	void EditPointerTool::draw(DrawingContext& dctx)
	{
		if (_Action_SelectMultiple)
		{
			drawSelectBounds(dctx);
		}
	}
}




