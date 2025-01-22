#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MyBox : public wui::MessageMapWindowT<MyBox, wui::ModalIndirectBaseDialog>
{
public:
	wui::MemoryDialogTemplate _DialogTemplate;

public:
	MyBox();

public:
	void initializeDialogTemplate(void);

public:
	void registerWindowMessageMap(void);
	void onInitDialog(wui::WindowMessage& windowMessage);
	void onDestroy(wui::WindowMessage& windowMessage);
	void onClose(wui::WindowMessage& windowMessage);
	void onCommand(wui::WindowMessage& windowMessage);
};
