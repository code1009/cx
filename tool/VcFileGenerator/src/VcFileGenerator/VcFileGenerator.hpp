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
	bool saveFile(std::wstring filePath, std::vector<std::uint8_t>& fileData);
	bool loadFile(std::wstring filePath, std::vector<std::uint8_t>& fileData);
};
