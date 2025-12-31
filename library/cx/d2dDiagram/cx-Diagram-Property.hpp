#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class PropertyTypes
	{
	public:
		static constexpr StringView Int8      = L"Int8";
		static constexpr StringView UInt8     = L"UInt8";
		static constexpr StringView Int16     = L"Int16";
		static constexpr StringView UInt16    = L"UInt16";
		static constexpr StringView Int32     = L"Int32";
		static constexpr StringView UInt32    = L"UInt32";
		static constexpr StringView Int64     = L"Int64";
		static constexpr StringView UInt64    = L"UInt64";
		static constexpr StringView Float     = L"Float";
		static constexpr StringView Double    = L"Double";
		static constexpr StringView Bool      = L"Bool";
		static constexpr StringView String    = L"String";
		static constexpr StringView Point     = L"Point";
		static constexpr StringView Points    = L"Points";
		static constexpr StringView Color     = L"Color";
		static constexpr StringView FillStyle = L"FillStyle";
		static constexpr StringView LineStyle = L"LineStyle";
		static constexpr StringView TextStyle = L"TextStyle";
	};

	//=======================================================================
	class PropertyFriendlyNames
	{
	public:
		static constexpr StringView fillStyle                = L"채움 모양";
		static constexpr StringView fillStyle_fillColor      = L"책움 색상";
		static constexpr StringView lineStyle                = L"선 모양";
		static constexpr StringView lineStyle_lineColor      = L"선 색상";
		static constexpr StringView lineStyle_lineSize       = L"선 크기";
		static constexpr StringView textStyle                = L"글자 모양";
		static constexpr StringView textStyle_textColor      = L"글자 색상";
		static constexpr StringView textStyle_fontFamily     = L"글꼴 이름";
		static constexpr StringView textStyle_fontSize       = L"글꼴 크기";
		static constexpr StringView textStyle_fontBold       = L"글꼴 굵게";
		static constexpr StringView textStyle_fontItalic     = L"글꼴 기울임";
		static constexpr StringView textStyle_textHAlignment = L"가로 정렬";
		static constexpr StringView textHAlignment_Left      = L"왼쪽";
		static constexpr StringView textHAlignment_Center    = L"가운데";
		static constexpr StringView textHAlignment_Right     = L"오른쪽";
		static constexpr StringView textStyle_textVAlignment = L"세로 정렬";
		static constexpr StringView textVAlignment_Top       = L"위쪽";
		static constexpr StringView textVAlignment_Center    = L"가운데";
		static constexpr StringView textVAlignment_Bottom    = L"아래쪽";
		static constexpr StringView color                    = L"색상";
		static constexpr StringView point                    = L"좌표";
		static constexpr StringView points                   = L"좌표들";
		static constexpr StringView size                     = L"크기";
		static constexpr StringView text                     = L"글자";
	};

	//=======================================================================
	class PropertyFriendlyNames_Item
	{
	public:
		static constexpr StringView className = L"종류";
		static constexpr StringView name      = L"이름";
		static constexpr StringView visible   = L"보이기";
		static constexpr StringView points    = L"좌표들";
	};
	class PropertyNames_Item
	{
	public:
		static constexpr StringView className = L"Item.className";
		static constexpr StringView name      = L"Item.name";
		static constexpr StringView visible   = L"Item.visible";
		static constexpr StringView points    = L"Item.points";
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Property : public std::enable_shared_from_this<Property>
	{
	public:
		using GetPropertyFunction = std::function<String const (void)>;
		using SetPropertyFunction = std::function<void(StringView const&)>;

	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		bool   _Visible{ true };
		String _FriendlyName;
		String _Name;
		String _Type;
		String _Value;
		bool   _ReadOnly{ true };

	private:
		bool _gotValue{ false };

	private:
		GetPropertyFunction _GetProperty;
		SetPropertyFunction _SetProperty;

	public:
		explicit Property(
			bool visible,
			StringView const& friendlyName,
			StringView const& name,
			StringView const& type,
			GetPropertyFunction const& get);

		explicit Property(
			bool visible,
			StringView const& friendlyName,
			StringView const& name,
			StringView const& type,
			GetPropertyFunction const& get,
			SetPropertyFunction const& set);

	public:
		virtual ~Property() = default;

	public:
		Property(const Property& other) = delete;
		Property& operator=(const Property& other) = delete;
		Property(Property&& other) noexcept = delete;
		Property& operator=(Property&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);

	public:
		bool visible(void) const { return _Visible; }
		void visible(bool newValue) { _Visible = newValue; }

		String friendlyName(void) const { return _FriendlyName; }
		void friendlyName(StringView const& newValue) { _FriendlyName = newValue; }

		String name() const { return _Name; }
		void name(StringView const& newValue) { _Name = newValue; }

		String type() const { return _Type; }
		void type(StringView const& newValue) { _Type = newValue; }

		bool readOnly(void) const { return _ReadOnly; }
		void readOnly(bool newValue) { _ReadOnly = newValue; }

	public:
		String value(void);
		void value(String const& newValue);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	class Properties : public std::enable_shared_from_this<Properties>
	{
	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::vector<std::shared_ptr<Property>> _Items;

	public:
		Properties();

	public:
		Properties(const Properties& other) = delete;
		Properties& operator=(const Properties& other) = delete;
		Properties(Properties&& other) noexcept = delete;
		Properties& operator=(Properties&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);

	private:
		void property_attachPropertyChangedEventHandler(std::shared_ptr<Property> const& item);
		void property_detachPropertyChangedEventHandler(std::shared_ptr<Property> const& item);
		void property_onPropertyChanged(cx::ev::Event& event);

	public:
		std::vector<std::shared_ptr<Property>>& items(void) { return _Items; }
		bool empty(void) const { return _Items.empty(); }
		std::size_t size(void) const { return _Items.size(); }
		std::shared_ptr<Property> operator[](std::size_t const index) { return at(index); };
		std::shared_ptr<Property> at(std::size_t const index) const;
		bool contains(std::shared_ptr<Property> const& test);
		std::size_t indexOf(std::shared_ptr<Property> const& item) const;

	public:
		void add(std::shared_ptr<Property> const& item);
		void insert(std::size_t const index, std::shared_ptr<Property> const& item);
		void remove(std::shared_ptr<Property> const& item);
		void clear(void);

	public:
		std::shared_ptr<Property> find(StringView const& propertyName);
	};
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::shared_ptr<Properties> makeProperties_Item(std::shared_ptr<Item> item);
}





