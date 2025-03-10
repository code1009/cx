﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "VcFileGenerator.hpp"
#include "Config.hpp"

//===========================================================================
#include "VcFile\VcItem.hpp"
#include "VcFile\Generator.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool VcFileGenerator::initialize(void)
{
	loadConfig();
	return true;
}

void VcFileGenerator::terminate(void)
{
}

bool VcFileGenerator::generate(std::wstring sourceDirectory, std::wstring targetDirectory)
{
	VcFile::Generator generator(sourceDirectory, targetDirectory);
	bool rv;
	rv = generator.generate();
	if (rv == false)
	{
		return false;
	}


	getConfig()._Source_Directory = sourceDirectory;
	saveConfig();
	return true;
}
