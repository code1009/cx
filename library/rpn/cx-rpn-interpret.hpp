#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::rpn
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using value_t = std::uint32_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1==CX_RPN_UNICODE
static value_t to_value(const rpn_char_t* s)
{
	return static_cast<value_t>(_wtoi(s));
}
#else
static value_t to_value(const rpn_char_t* s)
{
	return static_cast<value_t>(atoi(s));
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class expression;

//===========================================================================
using expression_stack_t  = std::stack<expression*>;
using expression_vector_t = std::vector<expression*>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class context
{
public:
	virtual ~context() = default;

public:
	virtual bool set_value(const rpn_char_t* s, const value_t& v);
	virtual bool get_value(const rpn_char_t* s, value_t&       v);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class expression
{
public:
	virtual ~expression() = default;

public:
	virtual value_t interpret (context* c);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class unary_operator : public expression
{
protected:
	expression* _operand0{ nullptr };

public:
	explicit unary_operator(expression* opd0);
};

//===========================================================================
class binary_operator : public expression
{
protected:
	expression* _operand0{ nullptr };
	expression* _operand1{ nullptr };

protected:
	binary_operator(expression* opd0, expression* opd1);
};

//===========================================================================
class ternary_operator : public expression
{
protected:
	expression* _operand0{ nullptr };
	expression* _operand1{ nullptr };
	expression* _operand2{ nullptr };

protected:
	ternary_operator(expression* opd0, expression* opd1, expression* opd2);
};

//===========================================================================
template<typename T>
class unary_function_operator : public unary_operator
{
public:
	explicit unary_function_operator(expression* opd0)
		: unary_operator(opd0)
	{
	}

public:
	virtual value_t interpret(context* c)
	{ 
		T function;
		return function( _operand0->interpret(c) );
	}
};

//===========================================================================
template<typename T>
class binary_function_operator : public binary_operator
{
public:
	binary_function_operator(expression* opd0, expression* opd1) 
		: binary_operator(opd0,opd1)
	{
	}

public:
	virtual value_t interpret(context* c)
	{ 
		T function;
		return function(_operand0->interpret(c), _operand1->interpret(c));
	}
};

//===========================================================================
template<typename T>
class ternary_function_operator : public ternary_operator
{
public:
	explicit ternary_function_operator(expression* opd0, expression* opd1, expression* opd2)
		: ternary_operator(opd0,opd1,opd2)
	{
	}

public:
	virtual value_t interpret(context* c)
	{ 
		T function;
		return function(_operand0->interpret(c), _operand1->interpret(c), _operand2->interpret(c));
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class constant : public expression
{
private:
	const value_t _value;

public:
	explicit constant(const value_t v);

public:
	virtual value_t interpret(context* c);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class variable : public expression
{
private:
	rpn_string_t _name;

public:
	explicit variable(const rpn_char_t* s);

public:
	virtual value_t interpret(context* c);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class evaluator : public expression
{
private:
	expression*         _syntax_tree;
	expression_vector_t _expressions;

public:	
	evaluator();
	virtual ~evaluator();

public:     
	bool create(const token_vector_t& tokens);
	void destroy(void);

public:
	virtual value_t interpret(context* c);

public:
	virtual bool is_function(const token_t& token);
	virtual bool is_operator(const token_t& token);
	virtual bool is_constant(const token_t& token);
	virtual bool is_variable(const token_t& token);

	virtual expression* create_function(const token_t& token, expression_stack_t& stack);
	virtual expression* create_operator(const token_t& token, expression_stack_t& stack);
	virtual expression* create_constant(const token_t& token, expression_stack_t& stack);
	virtual expression* create_variable(const token_t& token, expression_stack_t& stack);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
