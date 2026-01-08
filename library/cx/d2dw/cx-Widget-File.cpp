/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Widget.hpp"

//===========================================================================
#include <wil/resource.h>
#include <wil/com.h>

#include <xmllite.h>

//===========================================================================
#include <cx/component/xml.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
    //=======================================================================
    static const wchar_t* Root_TagName = L"도표";

    //=======================================================================
    static const wchar_t* View_TagName = L"문서";
    static const wchar_t* View_Width_AttrName = L"넓이";
    static const wchar_t* View_Height_AttrName = L"높이";
    static const wchar_t* View_BackgroundFillColor_AttrName = L"배경색";

    //=======================================================================
    static const wchar_t* Items_TagName = L"목록";

    static const wchar_t* Item_TagName = L"항목";
    static const wchar_t* Item_Type_AttrName = L"유형";

    static const wchar_t* Item_Property_TagName = L"속성";
    static const wchar_t* Item_Property_Name_AttrName = L"이름";
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
    class FileWriter
    {
    public:
        cx::xml::xml_saver _Saver;
        View& _View;
        ViewProperties _ViewProperties;

    public:
        explicit FileWriter(View& view) :
            _Saver( Root_TagName ),
            _View{ view }
        {
            _Saver.get_xml_context()->set_xml_root_tag_write_handler(
                std::bind(&FileWriter::writeTag_Root, this, std::placeholders::_1)
            );

            updateViewProperties();
        }

    public:
        FileWriter(const FileWriter& other) = delete;
        FileWriter& operator=(const FileWriter& other) = delete;
        FileWriter(FileWriter&& other) noexcept = delete;
        FileWriter& operator=(FileWriter&& other) noexcept = delete;

    public:
        Factory& factory()
        {
            return _View.factory();
        }
        Model& model()
        {
            return _View.model();
        }

    public:
        void updateViewProperties()
        {
            auto viewWidth  = _View.viewContext().width();
            auto viewHeight = _View.viewContext().height();
            auto viewBackgroundFillColor = _View.viewBackground().fillStyle().fillColor();
         
            _ViewProperties.width(viewWidth);
            _ViewProperties.height(viewHeight);
            _ViewProperties.backgroundFillColor(viewBackgroundFillColor);
		}

    public:
        std::shared_ptr<Properties> makeProperties(std::shared_ptr<Item> item)
        {
            auto classInfo = factory().findByClassName(item->className());
            auto makePropertiesFunction = classInfo->makeProperties();
            if (nullptr == makePropertiesFunction)
            {
                return nullptr;
            }

            return makePropertiesFunction(item);
        }

    public:
        bool WriteFile(std::wstring filePath)
        {
            return _Saver.save_xml_file(filePath);
        }

    public:
        bool writeTag_Root(cx::xml::xml_writer& writer)
        {
            bool rv;
            //-----------------------------------------------------------------------
            rv = cx::xml::write_xml_tag_open(writer, _Saver.get_xml_context()->get_xml_root_tag_name());
            if (true != rv)
            {
                return false;
            }
            rv = cx::xml::write_xml_endl(writer);
            if (true != rv)
            {
                return false;
            }



            //-----------------------------------------------------------------------
            rv = writeTag_View(writer);
            if (true != rv)
            {
                return false;
            }
            rv = writeTag_Items(writer);
            if (true != rv)
            {
                return false;
            }



            //-----------------------------------------------------------------------
            rv = cx::xml::write_xml_tag_close(writer);
            if (true != rv)
            {
                return false;
            }
            return true;
        }

        bool writeTag_View(cx::xml::xml_writer& writer)
        {
            bool rv;
            rv = cx::xml::write_xml_tag_open(writer, View_TagName, 1);
            if (true != rv)
            {
                return false;
            }


            rv = cx::xml::write_xml_attr_wstring(writer, View_Width_AttrName, cx::to_std_wstring(_ViewProperties.width()));
            if (true != rv)
            {
                return false;
            }
            rv = cx::xml::write_xml_attr_wstring(writer, View_Height_AttrName, cx::to_std_wstring(_ViewProperties.height()));
            if (true != rv)
            {
                return false;
            }
            rv = cx::xml::write_xml_attr_wstring(writer, View_BackgroundFillColor_AttrName, to_std_wstring(_ViewProperties.backgroundFillColor()));
            if (true != rv)
            {
                return false;
            }


            rv = cx::xml::write_xml_tag_close(writer, 0);
            if (true != rv)
            {
                return false;
            }
            return true;
        }

        bool writeTag_Items(cx::xml::xml_writer& writer)
        {
            bool rv;
            rv = cx::xml::write_xml_tag_open(writer, Items_TagName, 1);
            if (true != rv)
            {
                return false;
            }
            rv = cx::xml::write_xml_endl(writer);
            if (true != rv)
            {
                return false;
            }


            for (auto& item : _View.model().items())
            {
                rv = writeTag_Item(writer, item);
                if (true != rv)
                {
                    return false;
                }
            }


            rv = cx::xml::write_xml_tag_close(writer, 1);
            if (true != rv)
            {
                return false;
            }
            return true;
        }

        bool writeTag_Item(cx::xml::xml_writer& writer, std::shared_ptr<Item>& item)
        {
            auto properties = makeProperties(item);
            if (!properties)
            {
                return false;
            }
            auto property_className = properties->find(PropertyNames_Item::className);
            if (!property_className)
            {
                return false;
            }


            bool rv;
            rv = cx::xml::write_xml_tag_open(writer, Item_TagName, 2);
            if (true != rv)
            {
                return false;
            }

            rv = cx::xml::write_xml_attr_wstring(writer, Item_Type_AttrName, property_className->value());
            if (true != rv)
            {
                return false;
            }

            rv = cx::xml::write_xml_endl(writer);
            if (true != rv)
            {
                return false;
            }


            auto& items = properties->items();
            for (auto property : items)
            {
                if (property_className != property)
                {
                    rv = writeTag_Item_Property(writer, property);
                    if (true != rv)
                    {
                        return false;
                    }
                }
            }


            // single xml tag로 설정시에는 write_xml_tag_close()의 tab의 개수를 0으로 설정합니다.
            rv = cx::xml::write_xml_tag_close(writer, 2);
            if (true != rv)
            {
                return false;
            }
            return true;
        }

        bool writeTag_Item_Property(cx::xml::xml_writer& writer, std::shared_ptr<Property> property)
        {
            bool rv;
            rv = cx::xml::write_xml_tag_open(writer, Item_Property_TagName, 3);
            if (true != rv)
            {
                return false;
            }


            rv = cx::xml::write_xml_attr_wstring(writer, Item_Property_Name_AttrName, property->name());
            if (true != rv)
            {
                return false;
            }
            rv = writeTag_Item_Property_Value(writer, property);
            if (true != rv)
            {
                return false;
            }


            rv = cx::xml::write_xml_tag_close(writer, 0);
            if (true != rv)
            {
                return false;
            }
            return true;
        }

        bool writeTag_Item_Property_Value(cx::xml::xml_writer& writer, std::shared_ptr<Property> property)
        {
            HRESULT hr;
            hr = writer->WriteCData(property->value().c_str());
            if (FAILED(hr))
            {
                return false;
            }
            return true;
        }
    };
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
    //=======================================================================
    class FileReader
    {
    public:
        cx::xml::xml_loader _Loader;
        View& _View;
        bool _DesignMode = false;
        ViewProperties _ViewProperties;

    public:
        explicit FileReader(View& view, bool designMode) :
            _Loader{ Root_TagName },
            _View{ view },
            _DesignMode(designMode)
        {
            _Loader.get_xml_context()->set_xml_root_tag_read_handler(
                std::bind(&FileReader::read_tag_root, this, std::placeholders::_1, std::placeholders::_2)
            );
        }

    public:
        FileReader(const FileReader& other) = delete;
        FileReader& operator=(const FileReader& other) = delete;
        FileReader(FileReader&& other) noexcept = delete;
        FileReader& operator=(FileReader&& other) noexcept = delete;

    public:
        Factory& factory()
        {
            return _View.factory();
        }
        Model& model()
        {
            return _View.model();
        }

    public:
        void updateViewProperties()
        {
            _View.viewContext().setSize(_ViewProperties.width(), _ViewProperties.height());
            _View.viewBackground().fillStyle().fillColor(_ViewProperties.backgroundFillColor());
        }

    public:
        std::shared_ptr<Properties> makeProperties(std::shared_ptr<Item> item)
        {
            auto classInfo = factory().findByClassName(item->className());
            auto makePropertiesFunction = classInfo->makeProperties();
            if (nullptr == makePropertiesFunction)
            {
                return nullptr;
            }

            return makePropertiesFunction(item);
        }

    public:
        bool readFile(std::wstring filePath)
        {
            return _Loader.load_xml_file(filePath);
        }

    public:
        // root tag handler의 param은 nullptr로 설정되어 있습니다.
        bool read_tag_root(cx::xml::xml_reader& reader, void* /*param*/)
        {
            cx::xml::xml_read_handler_map childReadHandlerMap;
            childReadHandlerMap[View_TagName] = std::bind(&FileReader::readTag_View, this, std::placeholders::_1, std::placeholders::_2);
            childReadHandlerMap[Items_TagName] = std::bind(&FileReader::readTag_Items, this, std::placeholders::_1, std::placeholders::_2);

            return cx::xml::read_xml_child_tag(
                reader,
                _Loader.get_xml_context()->get_xml_root_tag_name(),
                childReadHandlerMap
            );
        }

        bool readTag_View(cx::xml::xml_reader& reader, void* /*param*/)
        {
            //---------------------------------------------------------------
            bool rv;

            std::wstring width;
            std::wstring height;
            std::wstring backgroundFillColor;
            rv = cx::xml::read_xml_attr_wstring(reader, View_Width_AttrName, width);
            if (false == rv)
            {
                return false;
            }
            rv = cx::xml::read_xml_attr_wstring(reader, View_Height_AttrName, height);
            if (false == rv)
            {
                return false;
            }
            rv = cx::xml::read_xml_attr_wstring(reader, View_BackgroundFillColor_AttrName, backgroundFillColor);
            if (false == rv)
            {
                return false;
            }


            //---------------------------------------------------------------
			_ViewProperties.width(to_Coord(width));
			_ViewProperties.height(to_Coord(height));
			_ViewProperties.backgroundFillColor(to_Color(backgroundFillColor));

            return true;
        }

        bool readTag_Items(cx::xml::xml_reader& reader, void* /*param*/)
        {
            cx::xml::xml_read_handler_map childReadHandlerMap;
            childReadHandlerMap[Item_TagName] = std::bind(&FileReader::readTag_Item, this, std::placeholders::_1, std::placeholders::_2);

            return cx::xml::read_xml_child_tag(
                reader,
                Items_TagName,
                childReadHandlerMap
            );
        }

    public:
		//-------------------------------------------------------------------
        struct MakeItemInfo
        {
            std::shared_ptr<Item> item;
            std::shared_ptr<Properties> properties;
        };

        bool readTag_Item(cx::xml::xml_reader& reader, void* /*param*/)
        {
            //---------------------------------------------------------------
            std::wstring type;


            bool rv;
            rv = cx::xml::read_xml_attr_wstring(reader, Item_Type_AttrName, type);
            if (false == rv)
            {
                return false;
            }


            //---------------------------------------------------------------
            std::wstring className;
            if (_DesignMode)
            {
                className = type + L".Design";
            }
            else
            {
                className = type;
            }


            //---------------------------------------------------------------
            MakeItemInfo makeItemInfo;
            makeItemInfo.item = factory().makeItemByClassName(className);
            if (!makeItemInfo.item)
            {
                return false;
            }
            makeItemInfo.properties = makeProperties(makeItemInfo.item);
            if (!makeItemInfo.properties)
            {
                return false;
            }


            //---------------------------------------------------------------
            cx::xml::xml_read_handler_map childReadHandlerMap;
            childReadHandlerMap[Item_Property_TagName] = std::bind(&FileReader::readTag_Item_Property, this, std::placeholders::_1, std::placeholders::_2);

            rv = cx::xml::read_xml_child_tag(
                reader,
                Item_TagName,
                childReadHandlerMap,
                &makeItemInfo
            );
            if (false == rv)
            {
                return false;
            }


            //---------------------------------------------------------------
            model().add(makeItemInfo.item);


            return true;
        }

        bool readTag_Item_Property(cx::xml::xml_reader& reader, void* param)
        {
            //---------------------------------------------------------------
            MakeItemInfo* makeItemInfo = reinterpret_cast<MakeItemInfo*>(param);


            //---------------------------------------------------------------
            bool rv;

            std::wstring name;


            rv = cx::xml::read_xml_attr_wstring(reader, Item_Property_Name_AttrName, name);
            if (false == rv)
            {
                return false;
            }


            //---------------------------------------------------------------
            std::wstring cdata;
            XmlNodeType node_type;
            while (S_OK == reader->Read(&node_type))
            {
                if (node_type == XmlNodeType_CDATA)
                {
                    const wchar_t* cdata_value = nullptr;
                    if (SUCCEEDED(reader->GetValue(&cdata_value, nullptr)))
                    {
                        cdata = cdata_value;
                        break;
                    }
                    else
                    {
                        return false;
                    }
                }
            }


            //---------------------------------------------------------------
            auto property = makeItemInfo->properties->find(name);
            if (!property)
            {
                return true;
            }
            property->value(cdata);


            return true;
        }
    };
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
    //=======================================================================
    bool saveFile(std::wstring filePath, View& view)
    {
        bool rv;
        FileWriter writer(view);
        rv = writer.WriteFile(filePath);
        if (false == rv)
        {
            return false;
        }
        return true;
    }

    bool loadFile(std::wstring filePath, View& view, bool designMode)
    {
        bool rv;
        FileReader reader(view, designMode);
        rv = reader.readFile(filePath);
        if (false == rv)
        {
            return false;
        }
        reader.updateViewProperties();

        return true;
    }
}

