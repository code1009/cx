/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CX_RUNTIME_PREPROCESSOR_WCSTRING_L_DOUBLE_SHARP(x) L##x
#define CX_RUNTIME_PREPROCESSOR_WCSTRING(x) CX_RUNTIME_PREPROCESSOR_WCSTRING_L_DOUBLE_SHARP(x)

//===========================================================================
#define CX_RUNTIME_FILEW CX_RUNTIME_PREPROCESSOR_WCSTRING(__FILE__)
#define CX_RUNTIME_FUNCW CX_RUNTIME_PREPROCESSOR_WCSTRING(__FUNCTION__)

//===========================================================================
#define CX_RUNTIME_LOG(level)                \
	cx::runtime::Log(                        \
		cx::runtime::getLogger(),            \
		(level),                             \
		CX_RUNTIME_FILEW,                    \
		__LINE__,                            \
		CX_RUNTIME_FUNCW                     \
	)

#define CX_RUNTIME_LOGP(level,param)         \
	cx::runtime::Log(                        \
		cx::runtime::getLogger(),            \
		(level),                             \
		CX_RUNTIME_FILEW,                    \
		__LINE__,                            \
		CX_RUNTIME_FUNCW                     \
		(param)                              \
	)

//===========================================================================
#define CX_RUNTIME_TLOG(logger,level)        \
	cx::runtime::Log(                        \
		(logger),                            \
		(level),                             \
		CX_RUNTIME_FILEW,                    \
		__LINE__,                            \
		CX_RUNTIME_FUNCW                     \
	)

#define CX_RUNTIME_TLOGP(logger,level,param) \
	cx::runtime::Log(                        \
		(logger),                            \
		(level),                             \
		CX_RUNTIME_FILEW,                    \
		__LINE__,                            \
		CX_RUNTIME_FUNCW                     \
		(param)                              \
	)

//===========================================================================
#define LVerb cx::runtime::LogLevel::Verbose
#define LDbug cx::runtime::LogLevel::Debug
#define LNorm cx::runtime::LogLevel::Normal
#define LDump cx::runtime::LogLevel::Dump
#define LInfo cx::runtime::LogLevel::Information
#define LWarn cx::runtime::LogLevel::Warning
#define LErro cx::runtime::LogLevel::Error
#define LCrit cx::runtime::LogLevel::Critical





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (CX_RUNTIME_ASSERT_DISABLE==0)

#if _DEBUG

#define CX_RUNTIME_ASSERT(X) \
	do                                                \
	{                                                 \
		if (!(X))                                     \
		{                                             \
			_CrtDbgBreak();                           \
			cx::runtime::assertCondition(             \
				(X),                                  \
				CX_RUNTIME_PREPROCESSOR_WCSTRING(#X), \
				CX_RUNTIME_FILEW,                     \
				__LINE__,                             \
				CX_RUNTIME_FUNCW                      \
				);                                    \
		}                                             \
	}                                                 \
	while (0)

#else	

#define CX_RUNTIME_ASSERT(X)                          \
	do                                                \
	{                                                 \
		cx::runtime::assertCondition(                 \
			(X),                                      \
			CX_RUNTIME_PREPROCESSOR_WCSTRING(#X),     \
			CX_RUNTIME_FILEW,                         \
			__LINE__,                                 \
			CX_RUNTIME_FUNCW                          \
			);                                        \
	}                                                 \
	while (0)

#endif

#else

#define CX_RUNTIME_ASSERT cx::runtime::AssertNull

#endif


