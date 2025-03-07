#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MyBox : public cx::wui::MessageMapWindowT<MyBox, cx::wui::ModalIndirectBaseDialog>
{
public:
	cx::wui::MemoryDialogTemplate _DialogTemplate;

public:
	MyBox();

public:
	void initializeDialogTemplate(void);

public:
	void registerWindowMessageMap(void);
	void onInitDialog(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
};
