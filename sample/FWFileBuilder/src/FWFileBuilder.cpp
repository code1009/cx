/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "FWFileBuilder.hpp"
#include "Config.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool FWFileBuilder::initialize(void)
{
	loadConfig();
	return true;
}

void FWFileBuilder::terminate(void)
{
}

//===========================================================================
bool FWFileBuilder::makeImage(
	std::wstring bl, 
	std::wstring sa, 
	std::wstring ma
	)
{
	std::vector<std::uint8_t> bl_data;
	std::vector<std::uint8_t> sa_data;
	std::vector<std::uint8_t> ma_data;

	if (!loadFile(bl, bl_data))
	{
		return false;
	}
	if (!loadFile(sa, sa_data))
	{
		return false;
	}
	if (!loadFile(ma, ma_data))
	{
		return false;
	}

	const std::size_t CPU_FLASH_BANK0_SIZE = 0x00200000;
	const std::size_t CPU_FLASH_BANK1_SIZE = 0x00200000;
	const std::size_t CPU_FLASH_SIZE = CPU_FLASH_BANK0_SIZE + CPU_FLASH_BANK1_SIZE;

	const std::size_t CPU_FLASH_IMAGE_ADDRESS_BEGIN_BOTTLOADER  = 0x00000000;
	const std::size_t CPU_FLASH_IMAGE_ADDRESS_BEGIN_SAFETY      = 0x00040000;
	const std::size_t CPU_FLASH_IMAGE_ADDRESS_BEGIN_MAINTENANCE = 0x00200000;

	const std::size_t CPU_FLASH_IMAGE_MAX_SIZE_BOTTLOADER       = 0x00040000;
	const std::size_t CPU_FLASH_IMAGE_MAX_SIZE_SAFETY           = 0x00200000-CPU_FLASH_IMAGE_MAX_SIZE_BOTTLOADER;
	const std::size_t CPU_FLASH_IMAGE_MAX_SIZE_MAINTENANCE      = 0x00200000;


	if (bl_data.size() > CPU_FLASH_IMAGE_MAX_SIZE_BOTTLOADER)
	{
		return false;
	}
	if (sa_data.size() > CPU_FLASH_IMAGE_MAX_SIZE_SAFETY)
	{
		return false;
	}
	if (ma_data.size() > CPU_FLASH_IMAGE_MAX_SIZE_MAINTENANCE)
	{
		return false;
	}

	_image.clear();
	_image.resize(CPU_FLASH_SIZE, 0xff);

	std::memcpy(&_image[CPU_FLASH_IMAGE_ADDRESS_BEGIN_BOTTLOADER ], bl_data.data(), bl_data.size());
	std::memcpy(&_image[CPU_FLASH_IMAGE_ADDRESS_BEGIN_SAFETY     ], sa_data.data(), sa_data.size());
	std::memcpy(&_image[CPU_FLASH_IMAGE_ADDRESS_BEGIN_MAINTENANCE], ma_data.data(), ma_data.size());

	getConfig()._BL_FilePath = bl;
	getConfig()._SA_FilePath = sa;
	getConfig()._MA_FilePath = ma;

	return true;
}

bool FWFileBuilder::saveImageFile(std::wstring filePath)
{
	if (!saveFile(filePath, _image))
	{
		return false;
	}

	getConfig()._Image_FilePath = filePath;

	saveConfig();

	return true;
}

bool FWFileBuilder::saveFile(std::wstring filePath, std::vector<std::uint8_t>& fileData)
{
	std::ofstream file(filePath, std::ios::binary);
	if (!file)
	{
		return false;
	}

	if (!file.write(reinterpret_cast<const char*>(fileData.data()), fileData.size()))
	{
		return false;
	}

	return true;
}

bool FWFileBuilder::loadFile(std::wstring filePath, std::vector<std::uint8_t>& fileData)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
	{
		return false;
	}

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	fileData.clear();
	fileData.resize(static_cast<size_t>(size));


	if (!file.read(reinterpret_cast<char*>(fileData.data()), size))
	{
		return false;
	}

	return true;
}
