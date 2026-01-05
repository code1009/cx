#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainFrame : 
	public cx::wui::MessageMapWindowT<MainFrame, cx::wui::BaseWindow>
{
private:
	std::unique_ptr<View> _View;

	// diagram
private:
	// diagram command
	struct CommandInfo
	{
		std::wstring type;
		std::wstring label;
		std::wstring name;
	};

	// diagram editor window layout
private:
	const std::uint32_t _Command_GridColumnIndex{ 0 };
	const double _Command_GridColumnWidth{ 250 };

	const std::uint32_t _Property_GridColumnIndex{ 2 };
	const double _Property_GridColumnWidth{ 450 };
	const double _Property_GridColumnNameWidth{ 130 };

private:
	std::vector<CommandInfo> _CommandInfos;
	std::vector<std::wstring> _FontFamilies;

private:
	std::wstring _Diagram_FilePath;

private:
	std::unique_ptr<cx::Diagram::PropertiesManipulator> _Diagram_PropertiesManipulator;
	std::unique_ptr<cx::Diagram::Edit>                  _Diagram_Edit;

public:
	MainFrame();

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);	
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onKeyDown(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onAppAbout(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
