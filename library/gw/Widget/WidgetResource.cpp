/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetResource::WidgetResource(std::wstring id, Type type, IUnknown* pointer):
	_Id     {std::move(id)}, 
	_Type   {type         }, 
	_Pointer{pointer      } 
{
}

//===========================================================================
WidgetResource::~WidgetResource()
{
	release();
}

void WidgetResource::release() 
{
	if (_Pointer) 
	{
		_Pointer->Release();
		_Pointer = nullptr;
	}
}

//===========================================================================
std::wstring WidgetResource::getId(void)
{
	return _Id;
}

WidgetResource::Type WidgetResource::getType(void)
{
	return _Type;
}

IUnknown* WidgetResource::getPointer(void)
{
	return _Pointer;
}

void WidgetResource::setPointer(IUnknown* pointer)
{
	_Pointer = pointer;
}

//===========================================================================
IDWriteTextFormat* WidgetResource::getTextFormat(void)
{
	if (_Type != Type::TextFormat)
	{
		return nullptr;
	}

	return static_cast<IDWriteTextFormat*>(_Pointer);
}

ID2D1SolidColorBrush* WidgetResource::getSolidColorBrush(void)
{
	if (_Type != Type::SolidColorBrush)
	{
		return nullptr;
	}

	return static_cast<ID2D1SolidColorBrush*>(_Pointer);
}

ID2D1StrokeStyle* WidgetResource::getStrokeStyle(void) 
{
	if (_Type != Type::StrokeStyle)
	{
		return nullptr;
	}

	return static_cast<ID2D1StrokeStyle*>(_Pointer);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
# CreateDeviceIndependentResources()
- ID2D1PathGeometry
- IDWriteTextFormat
- ...
This method is used to create resources which are not bound to any device.
Their lifetime effectively extends for the duration of the app.
These resources include the D2D, DWrite factories;
and a DWrite Text Format object
(used for identifying particular font characteristics) and
a D2D geometry.

# CreateDeviceResources()
- ID2D1SolidColorBrush
- ID2D1BitmapBrush
- ...
This method creates resources which are bound to a particular D3D device.
It's all centralized here, in case the resources need to be recreated
in case of D3D device loss(eg.display change, remoting, removal of video card, etc).
*/

//===========================================================================
static std::wstring id_WidgetFillStyle_SolidColorBrush(WidgetFillStyle* style)
{
	std::wstring id;
	id = L"SolidColorBrush:";
	id += to_std_wstring(*style);
	return id;
}

static std::wstring id_WidgetTextStyle_TextFormat(WidgetTextStyle* style)
{
	std::wstring id;
	id = L"TextFormat:";
	id += to_std_wstring(*style);
	return id;
}

static std::wstring id_WidgetLineStyle_SolidColorBrush(WidgetLineStyle* style)
{
	std::wstring id;
	id = L"SolidColorBrush:";
	id += to_std_wstring(*style);
	return id;
}

static std::wstring id_WidgetLineStyle_StrokeStyle(WidgetLineStyle* style)
{
	std::wstring id;
	id = L"StrokeStyle:";
	id += to_std_wstring(*style);
	return id;
}

static std::wstring id_WidgetTextStyle_SolidColorBrush(WidgetTextStyle* style)
{
	std::wstring id;
	id = L"SolidColorBrush:";
	id += to_std_wstring(*style);
	return id;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetResourceMap::WidgetResourceMap(Context* ctx) :
	_Context{ ctx}
{
}

//===========================================================================
WidgetResourceMap::~WidgetResourceMap()
{
	clear();
}

//===========================================================================
void WidgetResourceMap::discard(void)
{
	for (auto& pair : _WidgetResourceSharedPtrMap) 
	{
		pair.second->release();
	}
}

void WidgetResourceMap::clear(void)
{
	discard();

	_WidgetResourceSharedPtrMap.clear();
}

WidgetResourceSharedPtr WidgetResourceMap::registerWidgetResource(std::wstring id, WidgetResource::Type type)
{
	WidgetResourceSharedPtr widgetResource;
	auto it = _WidgetResourceSharedPtrMap.find(id);


	if (it != _WidgetResourceSharedPtrMap.end())
	{
		widgetResource = it->second;
	}
	else
	{
		widgetResource = std::make_shared<WidgetResource>(
			id,
			type
		);

		_WidgetResourceSharedPtrMap[id] = widgetResource;
	}


	return widgetResource;
}


//===========================================================================
WidgetResource* WidgetResourceMap::getWidgetFillStyle_SolidColorBrush(WidgetFillStyle* style)
{
	//--------------------------------------------------------------------------
	std::wstring id = id_WidgetFillStyle_SolidColorBrush(style);
	WidgetResourceSharedPtr widgetResource = registerWidgetResource(id, WidgetResource::Type::SolidColorBrush);
	if (widgetResource->getPointer())
	{
		return widgetResource.get();
	}


	//--------------------------------------------------------------------------
	HRESULT hr;
	ID2D1SolidColorBrush* pointer = nullptr;
	
	hr = _Context->getD2dRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(style->_Color._r, style->_Color._g, style->_Color._b, style->_Color._a),
		&pointer
	);
	if (FAILED(hr))
	{
		return nullptr;
	}


	//--------------------------------------------------------------------------
	widgetResource->setPointer(pointer);
	return widgetResource.get();
}

//===========================================================================
WidgetResource* WidgetResourceMap::getWidgetLineStyle_SolidColorBrush(WidgetLineStyle* style)
{
	//--------------------------------------------------------------------------
	std::wstring id = id_WidgetLineStyle_SolidColorBrush(style);
	WidgetResourceSharedPtr widgetResource = registerWidgetResource(id, WidgetResource::Type::SolidColorBrush);
	if (widgetResource->getPointer())
	{
		return widgetResource.get();
	}


	//--------------------------------------------------------------------------
	HRESULT hr;
	ID2D1SolidColorBrush* pointer = nullptr;

	hr = _Context->getD2dRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(style->_Color._r, style->_Color._g, style->_Color._b, style->_Color._a),
		&pointer
	);
	if (FAILED(hr))
	{
		return nullptr;
	}


	//--------------------------------------------------------------------------
	widgetResource->setPointer(pointer);


	return widgetResource.get();
}

WidgetResource* WidgetResourceMap::getWidgetLineStyle_StrokeStyle(WidgetLineStyle* style) 
{
	//--------------------------------------------------------------------------
	std::wstring id = id_WidgetLineStyle_StrokeStyle(style);
	WidgetResourceSharedPtr widgetResource = registerWidgetResource(id, WidgetResource::Type::StrokeStyle);
	if (widgetResource->getPointer())
	{
		return widgetResource.get();
	}


	//--------------------------------------------------------------------------
	HRESULT hr;
	ID2D1StrokeStyle* pointer = nullptr;
	D2D1_STROKE_STYLE_PROPERTIES strokeStyleProperties = D2D1::StrokeStyleProperties();

	strokeStyleProperties.startCap = D2D1_CAP_STYLE_FLAT;
	strokeStyleProperties.endCap   = D2D1_CAP_STYLE_FLAT;
	strokeStyleProperties.dashCap  = D2D1_CAP_STYLE_FLAT;
	strokeStyleProperties.lineJoin = D2D1_LINE_JOIN_MITER;

	strokeStyleProperties.startCap = D2D1_CAP_STYLE_ROUND;
	strokeStyleProperties.endCap   = D2D1_CAP_STYLE_ROUND;
	strokeStyleProperties.dashCap  = D2D1_CAP_STYLE_ROUND;
	strokeStyleProperties.lineJoin = D2D1_LINE_JOIN_ROUND;

	strokeStyleProperties.dashStyle = static_cast<D2D1_DASH_STYLE>(style->getDash());
	strokeStyleProperties.dashStyle = D2D1_DASH_STYLE_SOLID;
	strokeStyleProperties.dashStyle = D2D1_DASH_STYLE_DOT;

	hr = _Context->getD2dFactory()->CreateStrokeStyle(
		&strokeStyleProperties,
		nullptr,
		0,
		&pointer
	);
	if (FAILED(hr))
	{
		return nullptr;
	}


	//--------------------------------------------------------------------------
	widgetResource->setPointer(pointer);
	return widgetResource.get();
}

//===========================================================================
WidgetResource* WidgetResourceMap::getWidgetTextStyle_SolidColorBrush(WidgetTextStyle* style)
{
	//--------------------------------------------------------------------------
	std::wstring id = id_WidgetTextStyle_SolidColorBrush(style);
	WidgetResourceSharedPtr widgetResource = registerWidgetResource(id, WidgetResource::Type::SolidColorBrush);
	if (widgetResource->getPointer())
	{
		return widgetResource.get();
	}


	//--------------------------------------------------------------------------
	HRESULT hr;
	ID2D1SolidColorBrush* pointer = nullptr;

	hr = _Context->getD2dRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(style->_Color._r, style->_Color._g, style->_Color._b, style->_Color._a),
		&pointer
	);
	if (FAILED(hr))
	{
		return nullptr;
	}


	//--------------------------------------------------------------------------
	widgetResource->setPointer(pointer);
	return widgetResource.get();
}

WidgetResource* WidgetResourceMap::getWidgetTextStyle_TextFormat(WidgetTextStyle* style)
{
	//--------------------------------------------------------------------------
	std::wstring id = id_WidgetTextStyle_TextFormat(style);
	WidgetResourceSharedPtr widgetResource = registerWidgetResource(id, WidgetResource::Type::TextFormat);
	if (widgetResource->getPointer())
	{
		return widgetResource.get();
	}


	//--------------------------------------------------------------------------
	HRESULT hr;
	IDWriteTextFormat* pointer = nullptr;

	hr = _Context->getDWriteFactory()->CreateTextFormat(
		style->getFontFace().c_str(),
		nullptr,
		DWRITE_FONT_WEIGHT_ULTRA_BLACK, // DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		style->getFontSize(),
		L"ko-kr",
		&pointer
	);
	if (FAILED(hr))
	{
		return nullptr;
	}
	pointer->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pointer->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter


	//--------------------------------------------------------------------------
	widgetResource->setPointer(pointer);
	return widgetResource.get();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




