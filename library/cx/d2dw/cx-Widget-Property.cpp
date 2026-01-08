/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Property::Property(
		bool visible,
		StringView const& friendlyName,
		StringView const& name,
		StringView const& type,
		GetPropertyFunction const& get) :
		_Visible(visible),
		_FriendlyName(friendlyName),
		_Name(name),
		_Type(type),
		_ReadOnly(true),
		_GetProperty(get)
	{
	}
	Property::Property(
		bool visible,
		StringView const& friendlyName,
		StringView const& name,
		StringView const& type,
		GetPropertyFunction const& get,
		SetPropertyFunction const& set) :
		_Visible(visible),
		_FriendlyName(friendlyName),
		_Name(name),
		_Type(type),
		_ReadOnly(false),
		_GetProperty(get),
		_SetProperty(set)
	{
	}

	//=======================================================================
	void Property::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Property::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			Property_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	String Property::value(void)
	{
		_gotValue = true;

		
		if (_GetProperty)
		{
			_Value = _GetProperty();
		}
		return _Value;
	}

	void Property::value(String const& newValue)
	{
		if (!_gotValue)
		{
			value();
		}


		if (_Value == newValue)
		{
			return;
		}
		_Value = newValue;
		if (_SetProperty)
		{
			_SetProperty(_Value);
		}
		notifyPropertyChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Properties::Properties()
	{
	}

	void Properties::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"Properties::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			Properties_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void Properties::property_attachPropertyChangedEventHandler(std::shared_ptr<Property> const& item)
	{
		item->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&Properties::property_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Properties::property_attachPropertyChangedEventHandler:" << std::endl
			<< L"    name=" << item->name() << std::endl
			<< L"    item=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(item.get())) << std::endl
			;
		*/
	}

	void Properties::property_detachPropertyChangedEventHandler(std::shared_ptr<Property> const& item)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Properties::property_detachPropertyChangedEventHandler:" << std::endl
			<< L"    name=" << item->name() << std::endl
			<< L"    item=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(item.get())) << std::endl
			;
		*/
		item->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void Properties::property_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Properties::property_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Property> Properties::at(std::size_t const index) const
	{
		if (index < _Items.size())
		{
			return _Items.at(index);
		}
		return nullptr;
	}

	std::size_t Properties::indexOf(std::shared_ptr<Property> const& item) const
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

	bool Properties::contains(std::shared_ptr<Property> const& test)
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

	//=======================================================================
	void Properties::add(std::shared_ptr<Property> const& item)
	{
		property_attachPropertyChangedEventHandler(item);
		_Items.push_back(item);
		notifyPropertyChanged();
	}

	void Properties::insert(std::size_t const index, std::shared_ptr<Property> const& item)
	{
		if (index >= _Items.size())
		{
			add(item);
			return;
		}
		property_attachPropertyChangedEventHandler(item);
		_Items.insert(_Items.begin() + index, item);
		notifyPropertyChanged();
	}

	void Properties::remove(std::shared_ptr<Property> const& item)
	{
		auto found = std::find(_Items.begin(), _Items.end(), item);
		if (found == _Items.end())
		{
			return;
		}
		property_detachPropertyChangedEventHandler(*found);
		_Items.erase(found);
		notifyPropertyChanged();
	}

	void Properties::clear()
	{
		if (_Items.empty())
		{
			return;
		}
		for (auto& item : _Items)
		{
			property_detachPropertyChangedEventHandler(item);
		}
		_Items.clear();
		notifyPropertyChanged();
	}

	//=======================================================================
	std::shared_ptr<Property> Properties::find(StringView const& propertyName)
	{
		for (auto& property : _Items)
		{
			if (property->name() == propertyName)
			{
				return property;
			}
		}
		return nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class PropertiesBuilder_Item
	{
	private:
		std::shared_ptr<Properties>& _Properties;

	public:
		PropertiesBuilder_Item(std::shared_ptr<Properties>& properties) :
			_Properties(properties)
		{
		}

	public:
		void build(std::shared_ptr<Item> item)
		{
			build_className(item);
			build_name(item);
			build_visible(item);
			build_points(item);
		}
		void build_className(std::shared_ptr<Item> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				false,
				String(PropertyFriendlyNames_Item::className),
				String(PropertyNames_Item::className),
				String(PropertyTypes::String),
				[item]() -> String
				{
					return item->className();
				}
			);

			_Properties->add(property);
		}
		void build_name(std::shared_ptr<Item> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				false,
				String(PropertyFriendlyNames_Item::name),
				String(PropertyNames_Item::name),
				String(PropertyTypes::String),
				[item]() -> String
				{
					return item->name();
				},
				[item](StringView const& newValue)
				{
					item->name(newValue);
				}
			);

			_Properties->add(property);
		}
		void build_visible(std::shared_ptr<Item> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				true,
				String(PropertyFriendlyNames_Item::visible),
				String(PropertyNames_Item::visible),
				String(PropertyTypes::Bool),
				[item]() -> String
				{
					return cx::to_std_wstring(item->visible());
				},
				[item](StringView const& newValue)
				{
					item->visible(cx::to_bool(String(newValue)));
				}
			);

			_Properties->add(property);
		}
		void build_points(std::shared_ptr<Item> item)
		{
			std::shared_ptr<Property> property;

			property = std::make_shared<Property>(
				false,
				String(PropertyFriendlyNames_Item::points),
				String(PropertyNames_Item::points),
				String(PropertyTypes::Points),
				[item]() -> String
				{
					return to_std_wstring(item->getPoints());
				},
				[item](StringView const& newValue)
				{
					item->setPoints(to_Points(String(newValue)));
				}
			);

			_Properties->add(property);
		}
	};

	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Item(std::shared_ptr<Item> item)
	{
		std::shared_ptr<Properties> properties = std::make_shared<Properties>();

		PropertiesBuilder_Item propertiesBuilder(properties);
		propertiesBuilder.build(item);

		return properties;
	}
}





