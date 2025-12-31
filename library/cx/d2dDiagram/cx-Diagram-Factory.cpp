/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	ClassInfo::ClassInfo(
		std::shared_ptr<Item> const& item, 
		MakePropertiesFunction const& makeProperties) :
		_Item(item),
		_MakeProperties(makeProperties)
	{
	}

	ClassInfo::ClassInfo(
		std::shared_ptr<Item> const& item, 
		MakePropertiesFunction const& makeProperties, 
		StringView const& friendlyName) :
		_Item(item),
		_MakeProperties(makeProperties),
		_FriendlyName(friendlyName)
	{
	}

	//=======================================================================
	String ClassInfo::className(void) const
	{
		if (!_Item)
		{
			return String{};
		}
		return _Item->className();
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::shared_ptr<ClassInfo> Factory::at(std::size_t const index) const
	{
		if (index < _Map.size())
		{
			auto it = _Map.begin();
			std::advance(it, index);
			return it->second;
		}
		return nullptr;
	}

	//=======================================================================
	void Factory::registerItem(StringView const& name, std::shared_ptr<Item> const& item, ClassInfo::MakePropertiesFunction const& makeProperties, StringView const& friendlyName)
	{
		if (!item)
		{
			return;
		}
		String keyName(name);
		if (_Map.find(keyName) != _Map.end())
		{
			return;
		}
		_Map[keyName] = std::make_shared<ClassInfo>(item, makeProperties, friendlyName);
	}

	void Factory::unregisterItem(StringView const& name)
	{
		auto found = _Map.find(String(name));
		if (found == _Map.end())
		{
			return;
		}
		_Map.erase(found);
	}

	void Factory::clear()
	{
		_Map.clear();
	}

	//=======================================================================
	std::shared_ptr<ClassInfo> Factory::find(StringView const& name) const
	{
		auto found = _Map.find(String(name));
		if (found == _Map.end())
		{
			return nullptr;
		}
		return found->second;
	}

	std::shared_ptr<Item> Factory::makeItem(StringView const& name)
	{
		auto classInfo = find(name);
		if (classInfo)
		{
			auto item = classInfo->item();
			if (item)
			{
				return item->clone();
			}
		}
		return nullptr;
	}

	//=======================================================================
	void Factory::registerItem(std::shared_ptr<Item> const& item, ClassInfo::MakePropertiesFunction const& makeProperties)
	{
		if (!item)
		{
			return;
		}
		registerItem(item->className(), item, makeProperties, L"");
	}

	std::shared_ptr<ClassInfo> Factory::findByClassName(StringView const& className) const
	{
		for (auto& pair : _Map)
		{
			if (pair.second->className() == className)
			{
				return pair.second;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Item> Factory::makeItemByClassName(StringView const& className)
	{
		auto classInfo = findByClassName(className);
		if (classInfo)
		{
			auto item = classInfo->item();
			if (item)
			{
				return item->clone();
			}
		}
		return nullptr;
	}

	//=======================================================================
	void Factory::registerItem(std::shared_ptr<Item> const& item, ClassInfo::MakePropertiesFunction const& makeProperties, StringView const& friendlyName)
	{
		if (!item)
		{
			return;
		}
		registerItem(friendlyName, item, makeProperties, friendlyName);
	}

	std::shared_ptr<ClassInfo> Factory::findByFriendlyName(StringView const& friendlyName) const
	{
		for (auto& pair : _Map)
		{
			if (pair.second->friendlyName() == friendlyName)
			{
				return pair.second;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Item> Factory::makeItemByFriendlyName(StringView const& friendlyName)
	{
		auto classInfo = findByFriendlyName(friendlyName);
		if (classInfo)
		{
			auto item = classInfo->item();
			if (item)
			{
				return item->clone();
			}
		}
		return nullptr;
	}
}




