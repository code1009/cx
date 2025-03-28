﻿#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UDPTerminal;

namespace app
{
class WebUIManager;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Model
{
public:
	std::unique_ptr<UDPTerminal> _UDPTerminal;
	std::shared_ptr<app::WebUIManager> _WebUIManager;

public:
	Model() = default;

public:
	virtual ~Model() = default;

public:
	void setWebUIManager(std::shared_ptr<app::WebUIManager> ptr)
	{
		_WebUIManager = ptr;
	}

public:
	void connect(std::wstring laddress, std::wstring lport, std::wstring raddress, std::wstring rport);
	void disconnect(void);
	void send(const std::vector<std::uint8_t>& data);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Model* getModel(void);

