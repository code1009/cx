/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	PropertiesManipulator::PropertiesManipulator(Edit& edit) :
		_Edit(edit),
		_Item(nullptr),
		_Properties(nullptr),
		_Previous(nullptr),
		_Current(nullptr)
	{
	}

	//=======================================================================
	void PropertiesManipulator::notifyPropertyChanged(void)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"PropertiesManipulator::notifyPropertyChanged";
		propertyChangedEventListener.notify(
			PropertiesManipulator_PropertyChangedEvent,
			nullptr
		);
	}

	//=======================================================================
	void PropertiesManipulator::properties_attachPropertyChangedEventHandler(void)
	{
		_Properties->propertyChangedEventListener.attach(
			reinterpret_cast<std::uintptr_t>(this),
			std::bind(
				&PropertiesManipulator::properties_onPropertyChanged,
				this,
				std::placeholders::_1
			)
		);
	}

	void PropertiesManipulator::properties_detachPropertyChangedEventHandler(void)
	{
		_Properties->propertyChangedEventListener.detach(
			reinterpret_cast<std::uintptr_t>(this)
		);
	}

	void PropertiesManipulator::properties_onPropertyChanged(cx::ev::Event& /*event*/)
	{
		commitPropertiesChanged();

		/*
		CX_RUNTIME_LOG(cxLTrace)
			<< L"PropertiesManipulator::properties_onPropertyChanged:" << event.eventType()
			;
		*/
		notifyPropertyChanged();
	}

	//=======================================================================
	void PropertiesManipulator::select(std::shared_ptr<Item> const& item)
	{
		Factory const& factory = _Edit.factory();


		if (_Item == item)
		{
			return;
		}
		if (nullptr == item)
		{
			_Item = nullptr;
			_Properties = nullptr;
			_Current = nullptr;
			_Previous = nullptr;
			return;
		}

		auto classInfo = factory.findByClassName(item->className());
		auto makeProperties = classInfo->makeProperties();
		if (nullptr == makeProperties)
		{
			_Item = nullptr;
			_Properties = nullptr;
			_Current = nullptr;
			_Previous = nullptr;
			return;
		}

		_Item = item;
		_Properties = makeProperties(item);
		properties_attachPropertyChangedEventHandler();


		_Current  = item->clone();
		_Previous = item->clone();


		/*
		std::shared_ptr<Design> design = std::dynamic_pointer_cast<Design>(item);
		CX_RUNTIME_LOG(cxLTrace)
			<< L"PropertiesManipulator::selectItem:" << std::endl
			<< L"    className=" << design->className() << std::endl
			<< L"    design=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(design.get())) << std::endl
			<< L"    target=" << std::format(L"{}", reinterpret_cast<std::uintptr_t>(design->getTarget().get()))
			;
		*/
	}

	void PropertiesManipulator::commitPropertiesChanged(void)
	{
		if (!_Item)
		{
			return;
		}

		_Current = _Item->clone();

		_Edit.commitItemPropertiesChanged(
			_Item,
			_Current,
			_Previous
		);

		_Previous = _Item->clone();
	}
}
