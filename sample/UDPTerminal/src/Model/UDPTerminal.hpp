#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Model;
class udp_unicast;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UDPTerminal
{
private:
	Model* _Model;
	std::wstring _LocalAddress;
	std::wstring _LocalPort;
	std::wstring _RemoteAddress;
	std::wstring _RemotePort;

private:
	std::unique_ptr<udp_unicast> _udp_unicast;

public:
	explicit UDPTerminal(Model* model, std::wstring laddress, std::wstring lport, std::wstring raddress, std::wstring rport);

public:
	virtual ~UDPTerminal();

public:
	void send(const std::vector<std::uint8_t>& data);

public:
	void postWebMessage(std::wstring message, std::wstring command = L"메시지");
};

