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
	WidgetDesignerCommand_addWidget(WidgetDesignerModel* widgetDesignerModel, WidgetSharedPtr widget);

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
	WidgetDesignerCommand_deleteWidgets(WidgetDesignerModel* widgetDesignerModel, std::vector<WidgetSharedPtr>& widgets);

public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




