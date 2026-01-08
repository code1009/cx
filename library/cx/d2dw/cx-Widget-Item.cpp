/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Status::Status(const Status& other) :
		//propertyChangedEventListener(other.propertyChangedEventListener)
		_Pressed(other._Pressed),
		_Selected(other._Selected),
		_Hovered(other._Hovered),
		_Focused(other._Focused)
	{
	}

	Status& Status::operator=(const Status& other)
	{
		if (this != &other)
		{
			//propertyChangedEventListener = other.propertyChangedEventListener;
			_Pressed = other._Pressed;
			_Selected = other._Selected;
			_Hovered = other._Hovered;
			_Focused = other._Focused;
		}
		return *this;
	}

	Status::Status(Status&& other) noexcept :
		//propertyChangedEventListener(std::move(other.propertyChangedEventListener))
		_Pressed(other._Pressed),
		_Selected(other._Selected),
		_Hovered(other._Hovered),
		_Focused(other._Focused)
	{
	}

	Status& Status::operator=(Status&& other) noexcept
	{
		if (this != &other)
		{
			//propertyChangedEventListener = std::move(other.propertyChangedEventListener);
			_Pressed = other._Pressed;
			_Selected = other._Selected;
			_Hovered = other._Hovered;
			_Focused = other._Focused;
		}
		return *this;
	}

	//=======================================================================
	void Status::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Status::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			Status_PropertyChangedEvent,
			nullptr
		);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Item::Item(StringView const& className) :
		_ClassName(className)
	{
	}

	//=======================================================================
	void Item::notifyPropertyChanged(void)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Item::notifyPropertyChanged:" << std::endl
			<< L"    className=" << className() << std::endl
			<< L"    this=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(this))
			;
		*/

		//CX_RUNTIME_LOG(cxLTrace) << L"Item::notifyPropertyChanged:" << className();
		propertyChangedEventListener.notify(
			Item_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void Item::status_attachPropertyChangedEventHandler(void)
	{
		_Status.propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Item::status_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void Item::status_detachPropertyChangedEventHandler(void)
	{
		_Status.propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Item::status_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Item::status_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Item::copyTo(std::shared_ptr<Item> const& dest) const
	{
		auto other = std::dynamic_pointer_cast<Item>(dest);
		if (!other)
		{
			CX_RUNTIME_ASSERT(0);
			return;
		}
		if (className() != other->className())
		{
			CX_RUNTIME_ASSERT(0);
			return;
		}
		other->_Name = _Name;
		other->_Visible = _Visible;
		//other->_Status = _Status;
		other->_Points = _Points;
	}

	/*
	virtual std::shared_ptr<Item> Item::clone() const = 0;
	{
		auto item = std::make_shared<T>();
		copyTo(item);
		return item;
	}
	*/

	//=======================================================================
	void Item::name(StringView const& newValue)
	{
		if (_Name == newValue)
		{
			return;
		}
		_Name = newValue;
		notifyPropertyChanged();
	}

	void Item::visible(bool const newValue)
	{
		if (_Visible == newValue)
		{
			return;
		}
		_Visible = newValue;
		notifyPropertyChanged();
	}

	//=======================================================================
	std::vector<Point>& Item::getPoints(void)
	{
		return _Points;
	}

	void Item::setPoints(std::vector<Point> const& points)
	{
		if (_Points == points)
		{
			return;
		}

		//_Points = points;

		std::size_t count;
		count = std::min(_Points.size(), points.size());
		for (std::size_t i = 0; i < count; i++)
		{
			if (_Points[i] != points[i])
			{
				_Points[i] = points[i];
			}
		}

		notifyPropertyChanged();
	}

	//=======================================================================
	Point const Item::getPoint(std::size_t const index) const
	{
		CX_RUNTIME_ASSERT(index < _Points.size());
		if (index < _Points.size())
		{
			return _Points[index];
		}
		return Point{ 0.0, 0.0 };
	}

	void Item::setPoint(std::size_t const index, Point const& p)
	{
		CX_RUNTIME_ASSERT(index < _Points.size());
		if (index < _Points.size())
		{
			if (_Points[index] != p)
			{
				_Points[index] = p;
				notifyPropertyChanged();
			}
		}
	}

	//=======================================================================
	void Item::getBounds(Point& p0, Point& p1)
	{
		calculateBounds(_Points, p0, p1);
	}

	void Item::setBounds(Point const& /*p0*/, Point const& /*p1*/)
	{
	}

	//=======================================================================
	bool Item::isPointIn(Point const& test)
	{
		return isPointInBounds(_Points, test);
	}

	void Item::moveOffset(Point const& offset)
	{
		if (isPointZero(offset))
		{
			return;
		}
		for (auto& p : _Points)
		{
			p += offset;
		}
		notifyPropertyChanged();
	}

	//=======================================================================
	void Item::registerEventHandler(cx::ev::target::EventHandlerRegistry& /*eventHandlerRegistry*/)
	{
	}

	//=======================================================================
	void Item::drawUnderlay(DrawingContext& dctx)
	{
		if (visible() == false)
		{
			return;
		}
		drawContentUnderlay(dctx);
	}

	void Item::draw(DrawingContext& dctx)
	{
		if (visible() == false)
		{
			return;
		}
		drawContent(dctx);
	}

	void Item::drawOverlay(DrawingContext& dctx)
	{
		if (visible() == false)
		{
			return;
		}
		drawContentOverlay(dctx);
	}

	void Item::drawContentUnderlay(DrawingContext& /*dctx*/)
	{
	}

	void Item::drawContent(DrawingContext& /*dctx*/)
	{
	}

	void Item::drawContentOverlay(DrawingContext& /*dctx*/)
	{
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::shared_ptr<Item> Items::at(std::size_t const index) const
	{
		if (index < _Items.size())
		{
			return _Items[index];
		}
		return nullptr;
	}

	bool Items::contains(std::shared_ptr<Item> const& test)
	{
		auto found = std::find(
			_Items.begin(),
			_Items.end(),
			test);
		if (found != _Items.end())
		{
			return true;
		}
		return false;
	}

	std::size_t Items::indexOf(std::shared_ptr<Item> const& item) const
	{
		auto found = std::find(
			_Items.begin(),
			_Items.end(),
			item);

		std::size_t index = std::numeric_limits<std::size_t>::max();
		if (found != _Items.end())
		{
			index = std::distance(_Items.begin(), found);
		}
		return index;
	}

	//=======================================================================
	void Items::add(std::shared_ptr<Item> const& item)
	{
		_Items.push_back(item);
	}

	void Items::insert(std::size_t const index, std::shared_ptr<Item> const& item)
	{
		if (index >= _Items.size())
		{
			add(item);
			return;
		}
		_Items.insert(_Items.begin() + index, item);
	}

	void Items::remove(std::shared_ptr<Item> const& item)
	{
		_Items.erase(std::remove(_Items.begin(), _Items.end(), item), _Items.end());
	}

	void Items::clear()
	{
		_Items.clear();
	}

	std::shared_ptr<Item> Items::find(Point const& test) const
	{
		for (auto it = _Items.rbegin(); it != _Items.rend(); it++)
		{
			auto item = *it;
			if (item->isPointIn(test))
			{
				return item;
			}
		}
		return nullptr;
	}

	void Items::draw(DrawingContext& dctx)
	{
		for (auto item : _Items)
		{
			item->drawUnderlay(dctx);
		}
		for (auto item : _Items)
		{
			item->draw(dctx);
		}
		for (auto item : _Items)
		{
			item->drawOverlay(dctx);
		}
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void Model::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Model::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			Model_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void Model::item_attachPropertyChangedEventHandler(std::shared_ptr<Item> const& item)
	{
		item->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Model::item_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Model::item_attachPropertyChangedEventHandler:" << std::endl
			<< L"    className=" << item->className() << std::endl
			<< L"    item=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(item.get()))
			;
		*/
	}

	void Model::item_detachPropertyChangedEventHandler(std::shared_ptr<Item> const& item)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Model::item_detachPropertyChangedEventHandler:" << std::endl
			<< L"    className=" << item->className() << std::endl
			<< L"    item=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(item.get()))
			;
		*/
		item->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Model::item_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Model::item_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void Model::add(std::shared_ptr<Item> const& item)
	{
		item_attachPropertyChangedEventHandler(item);
		_Items.push_back(item);
		notifyPropertyChanged();
	}

	void Model::insert(std::size_t const index, std::shared_ptr<Item> const& item)
	{
		if (index >= _Items.size())
		{
			add(item);
			return;
		}
		item_attachPropertyChangedEventHandler(item);
		_Items.insert(_Items.begin() + index, item);
		notifyPropertyChanged();
	}

	void Model::remove(std::shared_ptr<Item> const& item)
	{
		auto found = std::find(_Items.begin(), _Items.end(), item);
		if (found == _Items.end())
		{
			return;
		}
		item_detachPropertyChangedEventHandler(*found);
		_Items.erase(found);
		notifyPropertyChanged();
	}

	void Model::clear()
	{
		if (_Items.empty())
		{
			return;
		}
		for (auto& item : _Items)
		{
			item_detachPropertyChangedEventHandler(item);
		}
		_Items.clear();
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Item> Model::findIfVisible(Point const& test) const
	{
		for (auto it = _Items.rbegin(); it != _Items.rend(); it++)
		{
			auto item = *it;
			if (item->isPointIn(test))
			{
				if (item->visible())
				{
					return item;
				}
			}
		}
		return nullptr;
	}
}
