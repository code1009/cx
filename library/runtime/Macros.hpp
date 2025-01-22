/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define RUNTIME_PREPROCESSOR_WCSTRING_L_DOUBLE_SHARP(x) L##x
#define RUNTIME_PREPROCESSOR_WCSTRING(x) RUNTIME_PREPROCESSOR_WCSTRING_L_DOUBLE_SHARP(x)

//===========================================================================
#define RUNTIME_FILEW RUNTIME_PREPROCESSOR_WCSTRING(__FILE__)
#define RUNTIME_FUNCW RUNTIME_PREPROCESSOR_WCSTRING(__FUNCTION__)

//===========================================================================
#define RUNTIME_LOG(level) \
	runtime::Log(             \
		runtime::getLogger(), \
		(level),              \
		RUNTIME_FILEW,        \
		__LINE__,             \
		RUNTIME_FUNCW         \
	)

#define RUNTIME_LOGP(level,param) \
	runtime::Log(             \
		runtime::getLogger(), \
		(level),              \
		RUNTIME_FILEW,        \
		__LINE__,             \
		RUNTIME_FUNCW         \
		(param)               \
	)

//===========================================================================
#define RUNTIME_TLOG(logger,level) \
	runtime::Log(             \
		(logger),             \
		(level),              \
		RUNTIME_FILEW,        \
		__LINE__,             \
		RUNTIME_FUNCW         \
	)

#define RUNTIME_TLOGP(logger,level,param) \
	runtime::Log(             \
		(logger),             \
		(level),              \
		RUNTIME_FILEW,        \
		__LINE__,             \
		RUNTIME_FUNCW         \
		(param)               \
	)

//===========================================================================
#define LVerb runtime::LogLevel::Verbose
#define LDbug runtime::LogLevel::Debug
#define LNorm runtime::LogLevel::Normal
#define LDump runtime::LogLevel::Dump
#define LInfo runtime::LogLevel::Information
#define LWarn runtime::LogLevel::Warning
#define LErro runtime::LogLevel::Error
#define LCrit runtime::LogLevel::Critical





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (RUNTIME_ASSERT_DISABLE==0)

#if _DEBUG

#define RUNTIME_ASSERT(X) \
	do                                              \
	{                                               \
		if (!(X))                                   \
		{                                           \
			_CrtDbgBreak();                         \
			runtime::assertCondition(               \
				(X),                                \
				RUNTIME_PREPROCESSOR_WCSTRING(#X),  \
				RUNTIME_FILEW,                      \
				__LINE__,                           \
				RUNTIME_FUNCW                       \
				);                                  \
		}                                           \
	}                                               \
	while (0)

#else	

#define RUNTIME_ASSERT(X) \
	do                                         \
	{                                          \
		runtime::assertCondition(              \
			(X),                               \
			RUNTIME_PREPROCESSOR_WCSTRING(#X), \
			RUNTIME_FILEW,                     \
			__LINE__,                          \
			RUNTIME_FUNCW                      \
			);                                 \
	}                                          \
	while (0)

#endif

#else

#define RUNTIME_ASSERT runtime::AssertNull

#endif


