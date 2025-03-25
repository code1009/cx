#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Model;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UDPTerminal
{
private:
	Model* _Model;
	std::wstring _Address;
	std::wstring _Port;

public:
	explicit UDPTerminal(Model* model, std::wstring address, std::wstring port);

public:
	virtual ~UDPTerminal() = default;

	//-----------------------------------------------------------------------
public:
	virtual bool initialize(void);
	virtual void terminate(void);
};

