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
Widget::Widget(std::wstring className):
	_ClassName{ std::move(className) }
{
}

//===========================================================================
void Widget::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	widget->_Name = _Name;
	widget->_Points = _Points;
	widget->_Visible = _Visible;

	widget->_PropertyChangedHandler = _PropertyChangedHandler;
}

//===========================================================================
std::wstring Widget::getClassName(void) const
{
	return _ClassName;
}

std::wstring Widget::getName(void) const 
{
	return _Name;
}

void Widget::setName(std::wstring name) 
{
	if (_Name != name)
	{
		_Name = name;

		setPropertyChanged();
	}
}

bool Widget::getVisible(void) const
{
	return _Visible;
}

void Widget::setVisible(bool visible)
{
	if (_Visible != visible)
	{
		_Visible = visible;

		setPropertyChanged();
	}
}

std::vector<Point>& Widget::getPoints(void)
{
	return _Points;
}

void Widget::setPoints(std::vector<Point> points)
{
	if (_Points != points)
	{
		_Points = points;

		setPropertyChanged();
	}
}

void Widget::setPoint(std::size_t n, Point p)
{
	if (n < _Points.size())
	{
		if (_Points[n] != p)
		{
			_Points[n] = p;

			setPropertyChanged();
		}
	}
}

//===========================================================================
void Widget::getBounds(Point& p0, Point& p1)
{
	calculateBounds(_Points, p0, p1);
}

void Widget::setBounds(const Point& p0, const Point& p1)
{
}

bool Widget::isPointIn(const Point& test) const
{
	return isPointInBounds(_Points, test);
}

void Widget::moveOffset(const Point& offset)
{
	if (offset.is_zero())
	{
		return;
	}


	for (auto& p : _Points) 
	{
		p += offset;
	}


	setPropertyChanged();
}

//===========================================================================
void Widget::setPropertyChangedHandler(PropertyChangedHandler handler)
{
	_PropertyChangedHandler = handler;
}

void Widget::setPropertyChanged(std::uint32_t code, void* data)
{
	if (_PropertyChangedHandler)
	{
		PropertyChangedParam param;
		param._Sender = this;
		param._Code = code;
		param._Data = data;
		_PropertyChangedHandler(&param);
	}
}

//===========================================================================
void Widget::registerEventHandler(WidgetDocument* doc)
{
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




