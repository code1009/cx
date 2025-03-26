/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

//===========================================================================
#include "UDPTerminal.hpp"
#include "Model.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void Model::connect(std::wstring laddress, std::wstring lport, std::wstring raddress, std::wstring rport)
{
	if (_UDPTerminal)
	{
		_UDPTerminal.reset();
	}
	_UDPTerminal = std::make_unique<UDPTerminal>(this, laddress, lport, raddress, rport);
}

void Model::disconnect(void)
{
	if (_UDPTerminal)
	{
		_UDPTerminal.reset();
	}
}

void Model::send(const std::vector<std::uint8_t>& data)
{
	if (_UDPTerminal)
	{
		_UDPTerminal->send(data);
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Model* getModel(void)
{
	static Model instance;


	return &instance;
}
