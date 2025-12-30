/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include "cx-rpn.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
# step
1. infix notation input
2. visitor patterned fsm-tokenizing
3. apply the shunting-yard algorithm
4. make a reverse polish notation (rpn)
5. create a abstract syntax tree (ast)
6. execute a ast for interpret pattern
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::rpn
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef value_t (*trinary_operator_function_t)(value_t, value_t, value_t);
typedef value_t (*binary_operator_function_t )(value_t, value_t);
typedef value_t (*unary_operator_function_t  )(value_t );

//===========================================================================
struct _not
{ 
	value_t operator() (value_t operand0) const
	{ 
		return ~operand0; 
	} 
};

struct _logic_not { value_t operator()(value_t operand0) const { return !operand0; } };

//===========================================================================
struct _mul
{ 
	value_t operator()(value_t operand0, value_t operand1) const 
	{ 
		return operand0 *  operand1; 
	} 
};

struct _div
{
	value_t operator()(value_t operand0, value_t operand1) const
	{
		if (0u!=operand1)
		{
			return operand0 /  operand1; 
		}

		return 0u;
	} 
};

struct _mod
{
	value_t operator()(value_t operand0, value_t operand1) const 
	{
		if (0u!=operand1)
		{
			return operand0 %  operand1; 
		}

		return 0u;
	} 
};

struct _add       { value_t operator()(value_t operand0, value_t operand1) const { return operand0 +  operand1; } };
struct _sub       { value_t operator()(value_t operand0, value_t operand1) const { return operand0 -  operand1; } };
struct _shl       { value_t operator()(value_t operand0, value_t operand1) const { return operand0 << operand1; } };
struct _shr       { value_t operator()(value_t operand0, value_t operand1) const { return operand0 >> operand1; } };
struct _logic_lt  { value_t operator()(value_t operand0, value_t operand1) const { return operand0 <  operand1; } };
struct _logic_lte { value_t operator()(value_t operand0, value_t operand1) const { return operand0 <= operand1; } };
struct _logic_gt  { value_t operator()(value_t operand0, value_t operand1) const { return operand0 >  operand1; } };
struct _logic_gte { value_t operator()(value_t operand0, value_t operand1) const { return operand0 >= operand1; } };
struct _logic_eq  { value_t operator()(value_t operand0, value_t operand1) const { return operand0 == operand1; } };
struct _logic_neq { value_t operator()(value_t operand0, value_t operand1) const { return operand0 != operand1; } };
struct _and       { value_t operator()(value_t operand0, value_t operand1) const { return operand0 &  operand1; } };
struct _xor       { value_t operator()(value_t operand0, value_t operand1) const { return operand0 ^  operand1; } };
struct _or        { value_t operator()(value_t operand0, value_t operand1) const { return operand0 |  operand1; } };
struct _logic_and { value_t operator()(value_t operand0, value_t operand1) const { return operand0 && operand1; } };
struct _logic_or  { value_t operator()(value_t operand0, value_t operand1) const { return operand0 || operand1; } };





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool context::set_value(const rpn_char_t* s, const value_t& v)
{
	return true;
}

bool context::get_value(const rpn_char_t* s, value_t& v)
{
	v = 0u;
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
value_t expression::interpret (context* c)
{
	return 0u;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
unary_operator::unary_operator(expression* opd0) :
	_operand0(opd0)
{
}

//===========================================================================
binary_operator::binary_operator(expression* opd0, expression* opd1) :
	_operand0(opd0),
	_operand1(opd1)
{
}

//===========================================================================
ternary_operator::ternary_operator(expression* opd0, expression* opd1, expression* opd2) :
	_operand0(opd0),
	_operand1(opd1),
	_operand2(opd2)
{
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constant::constant(const value_t v) :
	_value (v)
{
}

value_t constant::interpret(context* c)
{ 
	return _value; 
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
variable::variable(const rpn_char_t* s) :
	_name (s)
{
}

//===========================================================================
value_t variable::interpret(context* c)
{ 
	value_t value = 0u;
	c->get_value (_name.c_str(), value);
	return value; 
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
evaluator::evaluator() :
	_syntax_tree (nullptr)
{
}

evaluator::~evaluator()
{
	destroy ();
}

//===========================================================================
bool evaluator::create(const token_vector_t& tokens)
{
	token_vector_t::const_iterator i;
	expression*                    e;
	expression_stack_t             stack;


	for ( i=tokens.begin(); i!=tokens.end(); i++ )
	{
		e = nullptr;

		if ( is_function((*i)) )
		{
			e = create_function((*i), stack);
		}

		else if ( is_operator ( (*i) ) )
		{
			e = create_operator ( (*i), stack );
		}

		else if ( is_constant ( (*i) ) )
		{
			e = create_constant ( (*i), stack );
		}

		else if ( is_variable ( (*i) ) )
		{
			e = create_variable ( (*i), stack );
		}

		if ( e )
		{
			stack.push             ( e );
			_expressions.push_back ( e );
		}
		else
		{
			return false;
		}
	}

	if ( !stack.empty() )
	{
		_syntax_tree = stack.top();
		stack.pop();
	}
	else
	{
		_syntax_tree = nullptr;
	}

	if ( 0u==stack.size() )
	{
		return true;
	}

	return false;
}

//===========================================================================
template <typename TYPE>
static void delete_pointer(TYPE p)
{
	delete p;
}

void evaluator::destroy(void)
{
	std::for_each(_expressions.begin(), _expressions.end(), delete_pointer<expression*>);
	_expressions.clear();

	_syntax_tree = nullptr;
}

//===========================================================================
value_t evaluator::interpret(context* c)
{
	if (nullptr == _syntax_tree)
	{
		return 0u;
	}

	return _syntax_tree->interpret(c);
}

//===========================================================================
bool evaluator::is_function(const token_t& s)
{
	return false;
}

expression* evaluator::create_function(const token_t& token, expression_stack_t& stack)
{
	return nullptr;
}

//===========================================================================
bool evaluator::is_operator(const token_t& s)
{
	if (s.first==_token_operator)
	{
		return true;
	}
	/*
	const rpn_char_t* symbol = s.second;

	// c language operator
	if      ( 0==token_strcmp(symbol, CX_RPN_TEXT("~" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!" )) ) return true;
	
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("*" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("/" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("%" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("+" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("-" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<<")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">>")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<=")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">=")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("==")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!=")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("&" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("^" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("|" )) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("&&")) ) return true;
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("||")) ) return true;
	*/
	return false;
}

expression* evaluator::create_operator(const token_t& token, expression_stack_t& stack)
{
	const rpn_char_t* symbol = token.second;

	expression* operand0=nullptr;
	expression* operand1=nullptr;


	// unary_function_operator
	if (1u <= stack.size())
	{
		if      ( 0==token_strcmp(symbol, CX_RPN_TEXT("~" )) ){ operand0 = stack.top(); stack.pop(); return new unary_function_operator< _not       >(operand0); }
		else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!" )) ){ operand0 = stack.top(); stack.pop(); return new unary_function_operator< _logic_not >(operand0); }
	}

	// binary_function_operator
	if (2u <= stack.size())
	{
		if      ( 0 == token_strcmp(symbol, CX_RPN_TEXT("+" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _add       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("-" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _sub       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("*" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _mul       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("/" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _div       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("%" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _mod       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("&" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _and       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("^" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _xor       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("|" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _or        >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("<<")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _shl       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT(">>")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _shr       >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("<" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_lt  >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("<=")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_lte >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT(">" )) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_gt  >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT(">=")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_gte >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("==")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_eq  >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("!=")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_neq >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("&&")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_and >(operand0, operand1); }
		else if ( 0 == token_strcmp(symbol, CX_RPN_TEXT("||")) ) { operand1 = stack.top(); stack.pop(); operand0 = stack.top(); stack.pop(); return new binary_function_operator< _logic_or  >(operand0, operand1); }
	}

	return nullptr;
}

//===========================================================================
bool evaluator::is_constant(const token_t& token)
{
	if (token.first == _token_numeric)
	{
		return true;
	}

	return false;
}

expression* evaluator::create_constant(const token_t& token, expression_stack_t& stack)
{
	if (token.first == _token_numeric)
	{
		value_t value;

		value = to_value(token.second);
		return new constant(value);
	}

	return nullptr;
}

//===========================================================================
bool evaluator::is_variable(const token_t& token)
{
	if (token.first==_token_identifier)
	{
		return true;
	}

	return false;
}

expression* evaluator::create_variable(const token_t& token, expression_stack_t& stack)
{
	if (token.first==_token_identifier)
	{
		return new variable ( token.second );
	}

	return nullptr;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
