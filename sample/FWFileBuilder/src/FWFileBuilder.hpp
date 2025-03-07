#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class FWFileBuilder
{
public:
	FWFileBuilder() = default;

public:
	virtual ~FWFileBuilder() = default;

public:
	bool initialize(void);
	void terminate(void);
};
