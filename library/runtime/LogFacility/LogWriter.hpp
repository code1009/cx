﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class LogWriter
{
public:
	LogWriter() = default;

public:
	virtual ~LogWriter() = default;

public:
	virtual void write(LogItem& item);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


