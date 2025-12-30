#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::rpn
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1==CX_RPN_UNICODE
using rpn_char_t                    = wchar_t;
using rpn_string_t                  = std::wstring;
using rpn_strcmp_t                  = int(*)(const rpn_char_t*, const rpn_char_t*);
constexpr rpn_strcmp_t token_strcmp = wcscmp;
#define CX_RPN_TEXT(x) (L##x)
#else
using rpn_char_t                    = char;
using rpn_string_t                  = std::string;
using rpn_strcmp_t                  = int(*)(const rpn_char_t*, const rpn_char_t*);
constexpr rpn_strcmp_t token_strcmp = strcmp;
#define CX_RPN_TEXT(x) (x)
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using token_type_t = std::uint8_t;

constexpr token_type_t _token_parenthesis_open            = 1;
constexpr token_type_t _token_parenthesis_close           = 2;
constexpr token_type_t _token_function_argument_separator = 3;
constexpr token_type_t _token_operator                    = 4;
constexpr token_type_t _token_numeric                     = 5;
constexpr token_type_t _token_string                      = 6;
constexpr token_type_t _token_identifier                  = 7;

//===========================================================================
typedef std::pair  <token_type_t, const rpn_char_t*> token_t;
typedef std::vector<token_t>                         token_vector_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using tokenizer_state_id_t = std::uint8_t;

constexpr tokenizer_state_id_t tokenizer_state_id_scan       = 1;
constexpr tokenizer_state_id_t tokenizer_state_id_finish     = 2;
constexpr tokenizer_state_id_t tokenizer_state_id_error      = 3;
constexpr tokenizer_state_id_t tokenizer_state_id_operator   = 4;
constexpr tokenizer_state_id_t tokenizer_state_id_numeric    = 5;
constexpr tokenizer_state_id_t tokenizer_state_id_string     = 6;
constexpr tokenizer_state_id_t tokenizer_state_id_identifier = 7;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_event : public fsm::event
{
public:
	rpn_char_t new_char{ 0 };

public:
	tokenizer_event();

public:
	virtual ~tokenizer_event() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state : public fsm::state
{
private:
	tokenizer_state_id_t _id;

public:
	explicit tokenizer_state(tokenizer_state_id_t id);

public:
	virtual ~tokenizer_state() = default;

public:
	virtual tokenizer_state_id_t get_id (void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_finish : public tokenizer_state
{
public:
	tokenizer_state_finish();

public:
	virtual ~tokenizer_state_finish() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_error : public tokenizer_state
{
public:
	tokenizer_state_error();

public:
	virtual ~tokenizer_state_error() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_scan : public tokenizer_state
{
public:
	tokenizer_state_scan();

public:
	virtual ~tokenizer_state_scan() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_operator : public tokenizer_state
{
public:
	tokenizer_state_operator();

public:
	virtual ~tokenizer_state_operator() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_numeric : public tokenizer_state
{
private:
	bool _has_dot;

public:
	tokenizer_state_numeric();

public:
	virtual ~tokenizer_state_numeric() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_string : public tokenizer_state
{
public:
	tokenizer_state_string();

public:
	virtual ~tokenizer_state_string() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer_state_identifier : public tokenizer_state
{
public:
	tokenizer_state_identifier();

public:
	virtual ~tokenizer_state_identifier() = default;

public:
	virtual void on_entry(fsm::machine* m, fsm::state* s);
	virtual void on_exit (fsm::machine* m, fsm::state* s);
	virtual void on_event(fsm::machine* m, fsm::event* e);
};






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class tokenizer : public fsm::machine
{
public:
	rpn_char_t* _buffer;
	std::size_t _buffer_max_size;
	std::size_t _buffer_offset;

private:
	rpn_char_t* _current_token_symbol_pointer;
	std::size_t _current_token_symbol_size;
	std::size_t _current_token_symbol_offset;

private:
	tokenizer_state_scan       _state_scan;
	tokenizer_state_finish     _state_finish;
	tokenizer_state_error      _state_error;
	tokenizer_state_operator   _state_operator;
	tokenizer_state_numeric    _state_numeric;
	tokenizer_state_string     _state_string;
	tokenizer_state_identifier _state_identifier;

public:
	token_vector_t _tokens;

public:
	tokenizer();

public:
	virtual ~tokenizer() = default;

public:
	virtual fsm::state* get_state_instance(tokenizer_state_id_t id);

public:
	void reset(void);

public:
	bool tokenize(const rpn_char_t* s, std::size_t length);

public:
	const rpn_char_t* get_current_token_symbol(void);
	std::size_t       get_current_token_symbol_size(void);

public:
	void add_character(rpn_char_t c);
	void end_character(token_type_t type);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




