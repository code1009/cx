#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class ClassInfo : public std::enable_shared_from_this<ClassInfo>
	{
	public:
		using MakePropertiesFunction = 
			std::function<
				std::shared_ptr<Properties>(std::shared_ptr<Item> item)
			>;

	private:
		std::shared_ptr<Item> _Item;
		MakePropertiesFunction _MakeProperties;
		String _FriendlyName;

	public:
		explicit ClassInfo(
			std::shared_ptr<Item> const& item,
			MakePropertiesFunction const& makeProperties
		);

		explicit ClassInfo(
			std::shared_ptr<Item> const& item,
			MakePropertiesFunction const& makeProperties,
			StringView const& friendlyName
		);

	public:
		ClassInfo() = default;

	public:
		virtual ~ClassInfo() = default;

	public:
		ClassInfo(const ClassInfo& other) = delete;
		ClassInfo& operator=(const ClassInfo& other) = delete;
		ClassInfo(ClassInfo&& other) noexcept = delete;
		ClassInfo& operator=(ClassInfo&& other) noexcept = delete;

	public:
		std::shared_ptr<Item> item(void) const { return _Item; }
		void item(std::shared_ptr<Item> const& newValue) { _Item = newValue; }

		MakePropertiesFunction makeProperties(void) const { return _MakeProperties; }
		void makeProperties(MakePropertiesFunction const& newValue) { _MakeProperties = newValue; }

		String friendlyName(void) const { return _FriendlyName; }
		void friendlyName(StringView const& newValue) { _FriendlyName = newValue; }

	public:
		String className(void) const;
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class Factory
	{
	private:
		std::map < String, std::shared_ptr<ClassInfo> > _Map;

	public:
		Factory() = default;

	public:
		virtual ~Factory() = default;

	public:
		Factory(const Factory& other) = delete;
		Factory& operator=(const Factory& other) = delete;
		Factory(Factory&& other) noexcept = delete;
		Factory& operator=(Factory&& other) noexcept = delete;

	public:
		bool empty(void) const { return _Map.empty(); }
		std::size_t size(void) const { return _Map.size(); }
		std::shared_ptr<ClassInfo> operator[](std::size_t const index) { return at(index); };
		std::shared_ptr<ClassInfo> at(std::size_t const index) const;

	public:
		void registerItem(StringView const& name, std::shared_ptr<Item> const& item, ClassInfo::MakePropertiesFunction const& makeProperties, StringView const& friendlyName);
		void unregisterItem(StringView const& name);
		void clear();

	public:
		std::shared_ptr<ClassInfo> find(StringView const& name) const;
		std::shared_ptr<Item> makeItem(StringView const& name);

	public:
		void registerItem(std::shared_ptr<Item> const& item, ClassInfo::MakePropertiesFunction const& makeProperties);
		std::shared_ptr<ClassInfo> findByClassName(StringView const& className) const;
		std::shared_ptr<Item> makeItemByClassName(StringView const& className);

	public:
		void registerItem(std::shared_ptr<Item> const& item, ClassInfo::MakePropertiesFunction const& makeProperties, StringView const& friendlyName);
		std::shared_ptr<ClassInfo> findByFriendlyName(StringView const& friendlyName) const;
		std::shared_ptr<Item> makeItemByFriendlyName(StringView const& friendlyName);
	};
}

