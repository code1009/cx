#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::rpn
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using operator_precedence_t = std::uint32_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class parser
{
public:
	using on_error_t = std::function<void(const rpn_char_t* message, const rpn_char_t* param)>;
	on_error_t _on_error;

public:
	virtual ~parser() = default;

public:
	virtual bool parse (token_vector_t& tokens, token_vector_t& rpn);

public:
	virtual bool is_operand(const token_t& token);
	virtual bool is_operator(const token_t& token);
	virtual bool is_function_argument_separator(const token_t& token);
	virtual bool is_function(const token_t& token);

public:
	virtual bool operator_left_associativity(const token_t& token);
	virtual operator_precedence_t operator_precedence(const token_t& token);

public:
	virtual bool shunting_yard(token_vector_t& input, token_vector_t& output);

public:
	virtual void report_error(const rpn_char_t* message, const rpn_char_t* param = nullptr);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
