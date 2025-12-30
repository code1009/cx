/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/log_facility/log_facility.hpp>

//===========================================================================
#include <cx/rpn/cx-rpn.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_DEBUG)
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Debug/cx_static_library.lib")
#else
#pragma comment(lib, "../../library/lib/cx_static_library/x64/Release/cx_static_library.lib")
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void rpn_dump_tokens(cx::rpn::token_vector_t& tokens)
{
	cx::rpn::token_vector_t::iterator i;

	for (i = tokens.begin(); i != tokens.end(); i++)
	{
		const cx::rpn::rpn_char_t* name;

		switch ((*i).first)
		{
		case cx::rpn::_token_parenthesis_open: name = L"parenthesis_open           : "; break;
		case cx::rpn::_token_parenthesis_close: name = L"parenthesis_close          : "; break;
		case cx::rpn::_token_function_argument_separator: name = L"function_argument_separator: "; break;
		case cx::rpn::_token_operator: name = L"operator                   : "; break;
		case cx::rpn::_token_numeric: name = L"numeric                    : "; break;
		case cx::rpn::_token_string: name = L"string                     : "; break;
		case cx::rpn::_token_identifier: name = L"identifier                 : "; break;
		default: name = L"?                          : "; break;
		}
		CX_RUNTIME_LOG(cxLInfo)
			<< name
			<< (*i).second
			;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::size_t _rpn_tokenizer_max_size = 0u;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool rpn0_calc(cx::rpn::rpn_string_t expression)
{
	bool rv;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"expression: " << expression.c_str()
		;


	//-----------------------------------------------------------------------
	cx::rpn::tokenizer tokenizer;
	std::vector<cx::rpn::rpn_char_t> rpn_data;
	rpn_data.resize(expression.size() * 2);
	tokenizer._buffer = rpn_data.data();
	tokenizer._buffer_max_size = rpn_data.size();
	rv = tokenizer.tokenize(expression.data(), expression.size());
	if (_rpn_tokenizer_max_size < tokenizer._buffer_offset)
	{
		_rpn_tokenizer_max_size = tokenizer._buffer_offset;
	}
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"tokenize failed: "
			<< std::endl
			;
		return false;
	}


	//-----------------------------------------------------------------------
	cx::rpn::token_vector_t rpn;
	cx::rpn::parser parser;
	parser._on_error =
		[](const cx::rpn::rpn_char_t* message, const cx::rpn::rpn_char_t* param)
		{
			CX_RUNTIME_LOG(cxLInfo)
				<< L"parser error: " << message
				;
			if (param)
			{
				CX_RUNTIME_LOG(cxLInfo)
					<< L", " << param
					;
			}
		};

	rv = parser.parse(tokenizer._tokens, rpn);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"parse failed: "
			<< std::endl
			;
		return false;
	}


	//-----------------------------------------------------------------------
	cx::rpn::evaluator evaluator;
	rv = evaluator.create(rpn);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"evaluator create failed "
			<< std::endl
			;
		evaluator.destroy();
		return false;
	}
	cx::rpn::value_t value = evaluator.interpret(nullptr);
	evaluator.destroy();


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"result: " << value
		;
	CX_RUNTIME_LOG(cxLInfo) << L" ";

	rpn_dump_tokens(tokenizer._tokens);
	CX_RUNTIME_LOG(cxLInfo) << L" ";
	rpn_dump_tokens(rpn);

	CX_RUNTIME_LOG(cxLInfo)
		<< std::endl
		;

	return true;
}

void rpn0(void)
{
#if 1==CX_RPN_UNICODE
	cx::rpn::rpn_string_t expression1 = LR"__(2 * (3 + 4))__";
#else
	cx::rpn::rpn_string_t expression1 = R"__(2 * (3 + 4))__";
#endif
	rpn0_calc(expression1);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rpn_context : public cx::rpn::context
{
public:
	std::map<cx::rpn::rpn_string_t, cx::rpn::value_t> _variables;

public:
	virtual bool set_value(const cx::rpn::rpn_char_t* s, const cx::rpn::value_t& v) override
	{
		auto it = _variables.find(s);
		if (it != _variables.end())
		{
			(*it).second = v;
			return true;
		}

		return false;
	}

	virtual bool get_value(const cx::rpn::rpn_char_t* s, cx::rpn::value_t& v) override
	{
		auto it = _variables.find(s);
		if (it != _variables.end())
		{
			v = (*it).second;
			return true;
		}

		v = 0u;
		return false;
	}
};

//===========================================================================
bool rpn1_calc(cx::rpn::rpn_string_t expression, rpn_context& context)
{
	bool rv;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"expression: " << expression.c_str()
		;


	//-----------------------------------------------------------------------
	cx::rpn::tokenizer tokenizer;
	std::vector<cx::rpn::rpn_char_t> rpn_data;
	rpn_data.resize(expression.size() * 2);
	tokenizer._buffer = rpn_data.data();
	tokenizer._buffer_max_size = rpn_data.size();
	rv = tokenizer.tokenize(expression.data(), expression.size());
	if (_rpn_tokenizer_max_size < tokenizer._buffer_offset)
	{
		_rpn_tokenizer_max_size = tokenizer._buffer_offset;
	}
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"tokenize failed: "
			<< std::endl
			;
		return false;
	}


	//-----------------------------------------------------------------------
	cx::rpn::token_vector_t rpn;
	cx::rpn::parser parser;
	parser._on_error =
		[](const cx::rpn::rpn_char_t* message, const cx::rpn::rpn_char_t* param)
		{
			CX_RUNTIME_LOG(cxLInfo)
				<< L"parser error: " << message
				;
			if (param)
			{
				CX_RUNTIME_LOG(cxLInfo)
					<< L", " << param
					;
			}
		};

	rv = parser.parse(tokenizer._tokens, rpn);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"parse failed: "
			<< std::endl
			;
		return false;
	}


	//-----------------------------------------------------------------------
	cx::rpn::evaluator evaluator;
	rv = evaluator.create(rpn);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"evaluator create failed "
			<< std::endl
			;
		evaluator.destroy();
		return false;
	}
	cx::rpn::value_t value = evaluator.interpret(&context);
	evaluator.destroy();


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"result: " << value
		;
	CX_RUNTIME_LOG(cxLInfo) << L" ";

	rpn_dump_tokens(tokenizer._tokens);
	CX_RUNTIME_LOG(cxLInfo) << L" ";
	rpn_dump_tokens(rpn);

	CX_RUNTIME_LOG(cxLInfo)
		<< std::endl
		;

	return true;
}

void rpn1(void)
{
	rpn_context context;
	context._variables[CX_RPN_TEXT("a")] = 2u;
	context._variables[CX_RPN_TEXT("b")] = 10u;
	context._variables[CX_RPN_TEXT("c")] = 5u;


	for (auto& e : context._variables)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"variable: " << e.first.c_str() << L" = " << e.second
			;
	}
	CX_RUNTIME_LOG(cxLInfo) << L" ";


#if 1==CX_RPN_UNICODE
	cx::rpn::rpn_string_t expression1 = LR"__(a * (b + c))__";
#else
	cx::rpn::rpn_string_t expression1 = R"__(a * (b + c))__";
#endif
	rpn1_calc(expression1, context);


	cx::rpn::rpn_string_t expression2 = CX_RPN_TEXT("3 + b / c");
	rpn1_calc(expression2, context);


	cx::rpn::rpn_string_t expression3 = CX_RPN_TEXT("b + (0-a)");
	rpn1_calc(expression3, context);


	cx::rpn::rpn_string_t expression4 = CX_RPN_TEXT("b + (-a)");
	rpn1_calc(expression4, context);


	cx::rpn::rpn_string_t expression5 = CX_RPN_TEXT("b + ((0-a)");
	rpn1_calc(expression5, context);


	cx::rpn::rpn_string_t expression6 = CX_RPN_TEXT("a # b");
	rpn1_calc(expression6, context);


	cx::rpn::rpn_string_t expression7 = CX_RPN_TEXT("a + b * 3");
	rpn1_calc(expression7, context);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rpn_parser : public cx::rpn::parser
{
public:
	virtual bool is_function(const cx::rpn::token_t& token) override
	{
		const cx::rpn::rpn_char_t* symbol = token.second;

		if (token.first == cx::rpn::_token_identifier)
		{
			if (0 == cx::rpn::token_strcmp(symbol, CX_RPN_TEXT("max"))) { return true; }
			else if (0 == cx::rpn::token_strcmp(symbol, CX_RPN_TEXT("min"))) { return true; }
		}

		return false;
	}
};

//===========================================================================
struct _rpn_max
{
	cx::rpn::value_t operator()(cx::rpn::value_t operand0, cx::rpn::value_t operand1) const
	{
		if (operand0 < operand1)
		{
			return operand1;
		}
		return operand0;
	}
};

struct _rpn_min
{
	cx::rpn::value_t operator()(cx::rpn::value_t operand0, cx::rpn::value_t operand1) const
	{
		if (operand0 > operand1)
		{
			return operand1;
		}
		return operand0;
	}
};

//===========================================================================
class rpn_evaluator : public cx::rpn::evaluator
{
public:
	virtual bool is_function(const cx::rpn::token_t& token) override
	{
		const cx::rpn::rpn_char_t* symbol = token.second;

		if (token.first == cx::rpn::_token_identifier)
		{
			if (0 == cx::rpn::token_strcmp(symbol, CX_RPN_TEXT("max"))) { return true; }
			else if (0 == cx::rpn::token_strcmp(symbol, CX_RPN_TEXT("min"))) { return true; }
		}

		return false;
	}

	virtual cx::rpn::expression* create_function(const cx::rpn::token_t& token, cx::rpn::expression_stack_t& stack) override
	{
		const cx::rpn::rpn_char_t* symbol = token.second;
		cx::rpn::expression* operand0;
		cx::rpn::expression* operand1;

		// binary_function_operator
		if (2u <= stack.size())
		{
			if (0 == cx::rpn::token_strcmp(symbol, CX_RPN_TEXT("max"))) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new cx::rpn::binary_function_operator< _rpn_max >(operand0, operand1); }
			else if (0 == cx::rpn::token_strcmp(symbol, CX_RPN_TEXT("min"))) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new cx::rpn::binary_function_operator< _rpn_min >(operand0, operand1); }
		}

		return nullptr;
	}
};

//===========================================================================
bool rpn2_calc(cx::rpn::rpn_string_t expression, rpn_context& context)
{
	bool rv;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"expression: " << expression.c_str()
		;


	//-----------------------------------------------------------------------
	cx::rpn::tokenizer tokenizer;
	std::vector<cx::rpn::rpn_char_t> rpn_data;
	rpn_data.resize(expression.size() * 2);
	tokenizer._buffer = rpn_data.data();
	tokenizer._buffer_max_size = rpn_data.size();
	rv = tokenizer.tokenize(expression.data(), expression.size());
	if (_rpn_tokenizer_max_size < tokenizer._buffer_offset)
	{
		_rpn_tokenizer_max_size = tokenizer._buffer_offset;
	}
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"tokenize failed: "
			<< std::endl
			;
		return false;
	}


	//-----------------------------------------------------------------------
	cx::rpn::token_vector_t rpn;
	rpn_parser parser;
	parser._on_error =
		[](const cx::rpn::rpn_char_t* message, const cx::rpn::rpn_char_t* param)
		{
			CX_RUNTIME_LOG(cxLInfo)
				<< L"parser error: " << message
				;
			if (param)
			{
				CX_RUNTIME_LOG(cxLInfo)
					<< L" " << param
					;
			}
		};

	rv = parser.parse(tokenizer._tokens, rpn);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"parse failed: "
			<< std::endl
			;
		return false;
	}


	//-----------------------------------------------------------------------
	rpn_evaluator evaluator;
	rv = evaluator.create(rpn);
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"evaluator create failed "
			<< std::endl
			;
		evaluator.destroy();
		return false;
	}
	cx::rpn::value_t value = evaluator.interpret(&context);
	evaluator.destroy();


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"result: " << value
		;
	CX_RUNTIME_LOG(cxLInfo) << L" ";

	rpn_dump_tokens(tokenizer._tokens);
	CX_RUNTIME_LOG(cxLInfo) << L" ";
	rpn_dump_tokens(rpn);

	CX_RUNTIME_LOG(cxLInfo)
		<< std::endl
		;

	return true;
}

void rpn2(void)
{
	rpn_context context;
	context._variables[CX_RPN_TEXT("a")] = 2u;
	context._variables[CX_RPN_TEXT("b")] = 10u;
	context._variables[CX_RPN_TEXT("c")] = 5u;


	for (auto& e : context._variables)
	{
		CX_RUNTIME_LOG(cxLInfo)
			<< L"variable: " << e.first.c_str() << L" = " << e.second
			;
	}
	CX_RUNTIME_LOG(cxLInfo) << L" ";


	cx::rpn::rpn_string_t expression1 = CX_RPN_TEXT("min(3,a)");
	rpn2_calc(expression1, context);

	cx::rpn::rpn_string_t expression2 = CX_RPN_TEXT("max(1+3*(3+1),a)");
	rpn2_calc(expression2, context);

	cx::rpn::rpn_string_t expression3 = CX_RPN_TEXT("max(min(1+2*5,4), min(3,5))");
	rpn2_calc(expression3, context);

	cx::rpn::rpn_string_t expression4 = CX_RPN_TEXT("1 + (max(a+b, 3*3) / 2)");
	rpn2_calc(expression4, context);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void rpn(void)
{
	//rpn0();
	//rpn1();
	rpn2();

	CX_RUNTIME_LOG(cxLInfo) << L"_rpn_tokenizer_max_size=" << _rpn_tokenizer_max_size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class application
{
public:
	cx::runtime::memory_debug _memory_debug;

public:
	bool initialize()
	{
		//TEXT("application::initialize()\n");

		//-----------------------------------------------------------------------
		_memory_debug.enable();
		_memory_debug.start_leak_check();


		//-----------------------------------------------------------------------
		bool rv;
		rv = cx::runtime::log_facility_initialize();
		if (false == rv)
		{
			terminate();
			return false;
		}
		CX_RUNTIME_LOG(cxLInfo)
			<< L"----------------------------------------------------------------------------" << std::endl
			<< L"START" << std::endl
			<< L"----------------------------------------------------------------------------"
			;


		return true;
	}

	void terminate()
	{
		//-----------------------------------------------------------------------
		CX_RUNTIME_LOG(cxLInfo)
			<< L"----------------------------------------------------------------------------" << std::endl
			<< L"END" << std::endl
			<< L"----------------------------------------------------------------------------"
			;
		cx::runtime::log_facility_flush();
		cx::runtime::log_facility_cleanup();


		//-----------------------------------------------------------------------
		//_memory_debug.test_leak_check();
		_memory_debug.end_leak_check();
	}

	void run()
	{
		rpn();
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	//-----------------------------------------------------------------------
	bool rv;
	application app;
	rv = app.initialize();
	if (false == rv)
	{
		app.terminate();
		return -1;
	}
	app.run();
	app.terminate();
	return 0;
}
