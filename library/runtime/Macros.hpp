/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CX_RUNTIME_PREPROCESSOR_WCSTRING_L_DOUBLE_SHARP(x) L##x
#define CX_RUNTIME_PREPROCESSOR_WCSTRING(x) CX_RUNTIME_PREPROCESSOR_WCSTRING_L_DOUBLE_SHARP(x)





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CX_RUNTIME_WFILE CX_RUNTIME_PREPROCESSOR_WCSTRING(__FILE__)
#define CX_RUNTIME_WFUNC CX_RUNTIME_PREPROCESSOR_WCSTRING(__FUNCTION__)





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CX_RUNTIME_LOG(level)                \
	cx::runtime::Log(                        \
		cx::runtime::getLogger(),            \
		(level),                             \
		CX_RUNTIME_WFILE,                    \
		__LINE__,                            \
		CX_RUNTIME_WFUNC                     \
	)

#define CX_RUNTIME_LOGP(level,param)         \
	cx::runtime::Log(                        \
		cx::runtime::getLogger(),            \
		(level),                             \
		CX_RUNTIME_WFILE,                    \
		__LINE__,                            \
		CX_RUNTIME_WFUNC                     \
		(param)                              \
	)

//===========================================================================
#define CX_RUNTIME_TLOG(logger,level)        \
	cx::runtime::Log(                        \
		(logger),                            \
		(level),                             \
		CX_RUNTIME_WFILE,                    \
		__LINE__,                            \
		CX_RUNTIME_WFUNC                     \
	)

#define CX_RUNTIME_TLOGP(logger,level,param) \
	cx::runtime::Log(                        \
		(logger),                            \
		(level),                             \
		CX_RUNTIME_WFILE,                    \
		__LINE__,                            \
		CX_RUNTIME_WFUNC                     \
		(param)                              \
	)

//===========================================================================
constexpr cx::runtime::LogLevel cxLVerbose  = cx::runtime::LogLevel::Verbose;
constexpr cx::runtime::LogLevel cxLDebug    = cx::runtime::LogLevel::Debug;
constexpr cx::runtime::LogLevel cxLNormal   = cx::runtime::LogLevel::Normal;
constexpr cx::runtime::LogLevel cxLDump     = cx::runtime::LogLevel::Dump;
constexpr cx::runtime::LogLevel cxLInfo     = cx::runtime::LogLevel::Information;
constexpr cx::runtime::LogLevel cxLWarning  = cx::runtime::LogLevel::Warning;
constexpr cx::runtime::LogLevel cxLError    = cx::runtime::LogLevel::Error;
constexpr cx::runtime::LogLevel cxLCritical = cx::runtime::LogLevel::Critical;

constexpr cx::runtime::LogLevel cxLDefault = cxLDebug;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define CX_RUNTIME_ASSERT_ENABLE 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (CX_RUNTIME_ASSERT_ENABLE==1)

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
				CX_RUNTIME_WFILE,                     \
				__LINE__,                             \
				CX_RUNTIME_WFUNC                      \
				);                                    \
		}                                             \
	}                                                 \
	while (0)

#else	

#define CX_RUNTIME_ASSERT(X)                          \
	do                                                \
	{                                                 \
		if (!(X))                                     \
		{                                             \
			cx::runtime::assertCondition(             \
				(X),                                  \
				CX_RUNTIME_PREPROCESSOR_WCSTRING(#X), \
				CX_RUNTIME_WFILE,                     \
				__LINE__,                             \
				CX_RUNTIME_WFUNC                      \
				);                                    \
		}                                             \
	}                                                 \
	while (0)

#endif

#else

#define CX_RUNTIME_ASSERT cx::runtime::AssertNull

#endif


