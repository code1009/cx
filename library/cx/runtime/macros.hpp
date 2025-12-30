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
#define CX_RUNTIME_LOG(severity) \
	cx::runtime::log(                                  \
		cx::runtime::get_logger(),                     \
		(severity),                                    \
		CX_RUNTIME_WFILE,                              \
		__LINE__,                                      \
		CX_RUNTIME_WFUNC                               \
	)

#define CX_RUNTIME_LOG_PARAM(severity,param_pointer,param_size) \
	cx::runtime::log(                                  \
		cx::runtime::get_logger(),                     \
		(severity),                                    \
		CX_RUNTIME_WFILE,                              \
		__LINE__,                                      \
		CX_RUNTIME_WFUNC                               \
		(param_pointer)                                \
		(param_size)                                   \
	)

//===========================================================================
#define CX_RUNTIME_xLOG(logger,severity) \
	cx::runtime::log(                                  \
		(logger),                                      \
		(severity),                                    \
		CX_RUNTIME_WFILE,                              \
		__LINE__,                                      \
		CX_RUNTIME_WFUNC                               \
	)

#define CX_RUNTIME_xLOG_PARAM(logger,severity,param_pointer,param_size) \
	cx::runtime::log(                                  \
		(logger),                                      \
		(severity),                                    \
		CX_RUNTIME_WFILE,                              \
		__LINE__,                                      \
		CX_RUNTIME_WFUNC                               \
		(param_pointer)                                \
		(param_size)                                   \
	)

//===========================================================================
constexpr cx::runtime::log_severity cxLTrace    = cx::runtime::log_severity::trace   ;
constexpr cx::runtime::log_severity cxLDebug    = cx::runtime::log_severity::debug   ;
constexpr cx::runtime::log_severity cxLInfo     = cx::runtime::log_severity::info    ;
constexpr cx::runtime::log_severity cxLWarning  = cx::runtime::log_severity::warning ;
constexpr cx::runtime::log_severity cxLError    = cx::runtime::log_severity::error   ;
constexpr cx::runtime::log_severity cxLCritical = cx::runtime::log_severity::critical;





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
			cx::runtime::assert_condition(            \
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
			cx::runtime::assert_condition(            \
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

#define CX_RUNTIME_ASSERT cx::runtime::assert_null

#endif


