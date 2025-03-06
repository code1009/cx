/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerNoOpCommand : public WidgetDesignerCommand
{
public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerAddCommand : public WidgetDesignerCommand
{
private:
	WidgetDocument* _Document;
	WidgetSharedPtr _Widget;

public:
	WidgetDesignerAddCommand(WidgetDocument* document, WidgetSharedPtr widget);

public:
	virtual void execute(void);
	virtual void undo(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




