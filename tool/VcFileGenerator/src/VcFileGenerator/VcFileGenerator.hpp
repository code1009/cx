#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcFileGenerator
{
public:
	VcFileGenerator() = default;

public:
	virtual ~VcFileGenerator() = default;

public:
	bool initialize(void);
	void terminate(void);

public:
	bool generate(std::wstring sourceDirectory, std::wstring targetDirectory, std::wstring projectName);
};
