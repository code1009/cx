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

public:
	std::vector<std::uint8_t> _image;

public:
	bool makeImage(
		std::wstring bl,
		std::wstring sa,
		std::wstring ma
		);

	bool saveImageFile(std::wstring file_path);

public:
	bool saveFile(std::wstring filePath, std::vector<std::uint8_t>& fileData);
	bool loadFile(std::wstring filePath, std::vector<std::uint8_t>& fileData);
};
