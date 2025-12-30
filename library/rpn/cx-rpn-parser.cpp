/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include "cx-rpn.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::rpn
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool parser::parse(token_vector_t& tokens, token_vector_t& rpn)
{
	if ( shunting_yard (tokens, rpn) )
	{
		return true;
	}

	return false;
}

//===========================================================================
bool parser::is_operand(const token_t& token)
{
	if (is_function(token))
	{
		return false;
	}

	if ( _token_identifier == token.first )
	{
		return true;
	}
	if ( _token_numeric == token.first )
	{
		return true;
	}
	if ( _token_string == token.first )
	{
		return true;
	}

	return false;
}

bool parser::is_operator(const token_t& token)
{
	if (_token_operator == token.first)
	{
		return true;
	}
	return false;
}

bool parser::is_function_argument_separator(const token_t& token)
{
	if (_token_function_argument_separator == token.first)
	{
		return true;
	}
	return false;
}

bool parser::is_function(const token_t& token)
{
	return false;
}


//===========================================================================
operator_precedence_t parser::operator_precedence(const token_t& token)
{
	const rpn_char_t* symbol = token.second;
	operator_precedence_t precedence = 0u;


	if      ( 0==token_strcmp(symbol, CX_RPN_TEXT("~" )) ) { precedence=1000u; } // not
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!" )) ) { precedence=1000u; } // logic_not

	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("*" )) ) { precedence= 900u; } // mul
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("/" )) ) { precedence= 900u; } // div
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("%" )) ) { precedence= 900u; } // mod
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("+" )) ) { precedence= 800u; } // add
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("-" )) ) { precedence= 800u; } // sub
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<<")) ) { precedence= 700u; } // bit_lshift
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">>")) ) { precedence= 700u; } // bit_rshift
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<" )) ) { precedence= 600u; } // logic_lt 
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<=")) ) { precedence= 600u; } // logic_lte
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">" )) ) { precedence= 600u; } // logic_gt 
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">=")) ) { precedence= 600u; } // logic_gte
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("==")) ) { precedence= 500u; } // logic_eq 
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!=")) ) { precedence= 500u; } // logic_neq
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("&" )) ) { precedence= 500u; } // bit_and
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("^" )) ) { precedence= 400u; } // bit_xor
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("|" )) ) { precedence= 300u; } // bit_or 
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("&&")) ) { precedence= 200u; } // logic_and
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("||")) ) { precedence= 100u; } // logic_or 

    return precedence;
}

bool parser::operator_left_associativity(const token_t& token)
{
	const rpn_char_t* symbol = token.second;


	if      ( 0==token_strcmp(symbol, CX_RPN_TEXT("~" )) ) { return false;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!" )) ) { return false;}

	// 제곱승은 오른쪽 결합

	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("*" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("/" )) ) { return true ;} 
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("%" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("+" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("-" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<<")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">>")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("<=")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT(">=")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("==")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("!=")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("&" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("^" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("|" )) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("&&")) ) { return true ;}
	else if ( 0==token_strcmp(symbol, CX_RPN_TEXT("||")) ) { return true ;}

    return false;
}

//===========================================================================
bool parser::shunting_yard (token_vector_t& input, token_vector_t& output)
{
	token_vector_t::iterator i;

	token_t             token;
	token_t             token_onstack;
	std::stack<token_t> token_stack;


	for (i=input.begin(); i!=input.end(); i++)
	{
		token = *i;
		
		if      ( is_operand ( token ) )
		{
			output.push_back ( token );
		}
		else if ( is_function ( token ) )
		{
			token_stack.push ( token );
		}
		else if ( is_function_argument_separator ( token ) )
		{
			bool pe = false;

			while ( !token_stack.empty() )
			{
				token_onstack = token_stack.top();

				if ( token_onstack.first == _token_parenthesis_open )
				{
					pe = true;
					break;
				}
				else
				{
					output.push_back ( token_onstack );

					token_stack.pop();
				}
			}

			if (!pe)
			{
				report_error(CX_RPN_TEXT("function argument separator or parentheses mismatched"));
				return false;
			}
		}
		else if ( is_operator(token) )
		{
			while ( !token_stack.empty() )
			{
				token_onstack = token_stack.top();

				if ( is_operator(token_onstack) && 
				     (
				       ( true ==operator_left_associativity(token) && ( operator_precedence(token)<=operator_precedence(token_onstack) ) )
					   ||
				       ( false==operator_left_associativity(token) && ( operator_precedence(token)< operator_precedence(token_onstack) ) )
				     ) )
				{
					output.push_back ( token_onstack );
					token_stack.pop  ();
				}
				else
				{
					break;
				}
			}
			
			token_stack.push ( token );
		}
		else if ( token.first == _token_parenthesis_open )
		{
			token_stack.push ( token );
		}
		else if ( token.first == _token_parenthesis_close )
		{
			bool pe = false;

			while ( !token_stack.empty() )
			{
				token_onstack = token_stack.top();

				if ( token_onstack.first == _token_parenthesis_open )
				{
					pe = true;
					break;
				}
				else
				{
					output.push_back ( token_onstack );
					token_stack.pop  ();
				}
			}

			if(!pe)  
			{
				report_error(CX_RPN_TEXT("parentheses mismatched"));
				return false;
			}

			token_stack.pop  (); // "(" remove

			if ( !token_stack.empty() )
			{
				token_onstack = token_stack.top();

				if ( is_function ( token_onstack ) )
				{
					output.push_back ( token_onstack );
					token_stack.pop  ();
				}
			}
		}
		else
		{
			report_error(CX_RPN_TEXT("unknown token"), token.second);
			return false;
		}
	}

	while ( !token_stack.empty() )
	{
		token_onstack = token_stack.top();

		if ( ( token_onstack.first == _token_parenthesis_open  ) ||
		     ( token_onstack.first == _token_parenthesis_close )  )
		{
			report_error(CX_RPN_TEXT("parentheses mismatched"));
			return false;
		}
		else
		{
			output.push_back ( token_onstack );
			token_stack.pop  ();
		}
	}
	
	return true;
}

void parser::report_error(const rpn_char_t* message, const rpn_char_t* param)
{
	if (_on_error)
	{
		_on_error(message, param);
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
