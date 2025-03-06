/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_addWidget : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	WidgetSharedPtr _Widget;

public:
	WidgetDesignerCommand_addWidget(
		WidgetDesignerModel* widgetDesignerModel, 
		WidgetSharedPtr widget
	);

public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_deleteWidgets : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	std::vector<WidgetSharedPtr> _Widgets;

	std::map<std::size_t, WidgetSharedPtr> _WidgetMap;

public:
	WidgetDesignerCommand_deleteWidgets(
		WidgetDesignerModel* widgetDesignerModel, 
		std::vector<WidgetSharedPtr>& widgets
	);

public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_moveWidgets : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	std::vector<WidgetSharedPtr> _Widgets;
	Point _Offset;

public:
	WidgetDesignerCommand_moveWidgets(
		WidgetDesignerModel* widgetDesignerModel,
		std::vector<WidgetSharedPtr>& widgets,
		Point& offset
	);

public:
	virtual void execute(void);
	virtual void undo(void);

public:
	void moveOffset(Point& offset);
	std::vector<WidgetSharedPtr> getWidgets(void) { return _Widgets; };
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_selectWidgets : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	std::vector<WidgetSharedPtr> _PreselectedWidgets;
	std::vector<WidgetSharedPtr> _SelectedWidgets;

public:
	WidgetDesignerCommand_selectWidgets(
		WidgetDesignerModel* widgetDesignerModel,
		std::vector<WidgetSharedPtr>& preselectedWidgets,
		std::vector<WidgetSharedPtr>& selectedWidgets
	);

public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerCommand_toggleWidgetSelection : public WidgetDesignerCommand
{
private:
	WidgetDesignerModel* _WidgetDesignerModel;
	WidgetSharedPtr _Widget;

private:
	bool _IsSelected { false };

public:
	WidgetDesignerCommand_toggleWidgetSelection(
		WidgetDesignerModel* widgetDesignerModel,
		WidgetSharedPtr widget
	);

public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




