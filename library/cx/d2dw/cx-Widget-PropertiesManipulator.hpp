#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class Edit;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	class PropertiesManipulator
	{
	private:
		Edit& _Edit;

	public:
		cx::ev::key::EventListener propertyChangedEventListener;

	private:
		std::shared_ptr<Item> _Item;
		std::shared_ptr<Properties> _Properties;

	private:
		std::shared_ptr<Item> _Previous;
		std::shared_ptr<Item> _Current;

	public:
		explicit PropertiesManipulator(Edit& edit);

	public:
		PropertiesManipulator(const PropertiesManipulator& other) = delete;
		PropertiesManipulator& operator=(const PropertiesManipulator& other) = delete;
		PropertiesManipulator(PropertiesManipulator&& other) noexcept = delete;
		PropertiesManipulator& operator=(PropertiesManipulator&& other) noexcept = delete;

	public:
		void notifyPropertyChanged(void);
	private:
		void properties_attachPropertyChangedEventHandler(void);
		void properties_detachPropertyChangedEventHandler(void);
		void properties_onPropertyChanged(cx::ev::Event& event);

	public:
		std::shared_ptr<Properties> getProperties(void) { return _Properties; }

	public:
		void select(std::shared_ptr<Item> const& item);

	private:
		void commitPropertiesChanged(void);
	};
}
