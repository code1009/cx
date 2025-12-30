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
static inline bool is_alphabet_character(const rpn_char_t c)
{
	if ('A' <= c && c <= 'Z') return true;
	if ('a' <= c && c <= 'z') return true;

	return false;
}

static inline bool is_number_character(const rpn_char_t c)
{
	if ('0' <= c && c <= '9') return true;

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_event::tokenizer_event() : 
	new_char(0)
{
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state::tokenizer_state (tokenizer_state_id_t id) : _id(id)
{
}

//===========================================================================
tokenizer_state_id_t tokenizer_state::get_id (void)
{
	return _id;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_finish:: tokenizer_state_finish() : 
	tokenizer_state(tokenizer_state_id_finish)
{
}

//===========================================================================
void tokenizer_state_finish::on_entry(fsm::machine* m, fsm::state* s){}
void tokenizer_state_finish::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_finish::on_event(fsm::machine* m, fsm::event* e){}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_error:: tokenizer_state_error() :
	tokenizer_state(tokenizer_state_id_error)
{
}

//===========================================================================
void tokenizer_state_error::on_entry(fsm::machine* m, fsm::state* s){}
void tokenizer_state_error::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_error::on_event(fsm::machine* m, fsm::event* e){}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_scan:: tokenizer_state_scan() :
	tokenizer_state(tokenizer_state_id_scan)
{
}

//===========================================================================
void tokenizer_state_scan::on_entry(fsm::machine* m, fsm::state* s){}
void tokenizer_state_scan::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_scan::on_event(fsm::machine* m, fsm::event* e)
{
	tokenizer*       tm = static_cast< tokenizer*       > ( m );
	tokenizer_event* te = static_cast< tokenizer_event* > ( e );
	if (nullptr == tm)
	{
		return;
	}
	if (nullptr == te)
	{
		return;
	}


	switch ( te->new_char )
	{
	case '\0':
		{
			transition (tm, tm->get_state_instance(tokenizer_state_id_finish) );
			return;
		}
		break;

	case '\r':
	case '\n':
	case '\t':
	case ' ' :
		{
			return;
		}
		break;

	case '(':
		{
			tm->add_character ( te->new_char );
			tm->end_character ( _token_parenthesis_open );
			return;
		}
		break;

	case ')':
		{
			tm->add_character ( te->new_char );
			tm->end_character ( _token_parenthesis_close );
			return;
		}
		break;

	case ',':
		{
			tm->add_character ( te->new_char );
			tm->end_character ( _token_function_argument_separator );
			return;
		}
		break;
		
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '~': 
	case '^': 
	case '&': // &&     &
	case '|': // ||     |
	case '<': // <=     <<     <
	case '>': // >=     >>     >
	case '!': // !=     !
	case '=': // ==     
		{
			transition (tm, tm->get_state_instance(tokenizer_state_id_operator) );
			tm->notify (e);
			return;
		}
		break;

	case '\"':
		{
			transition (tm, tm->get_state_instance(tokenizer_state_id_string) );
			/*tm->notify (e);*/
			return;
		}
		break;
	}

	if ( is_number_character (te->new_char) )
	{
		transition (tm, tm->get_state_instance(tokenizer_state_id_numeric) );
		tm->notify (e);
		return;
	}
	
	if ( is_alphabet_character (te->new_char) )
	{
		transition (tm, tm->get_state_instance(tokenizer_state_id_identifier) );
		tm->notify (e);
		return;
	}

	transition (tm, tm->get_state_instance(tokenizer_state_id_error) );
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_operator:: tokenizer_state_operator() :
	tokenizer_state(tokenizer_state_id_operator)
{
}

//===========================================================================
void tokenizer_state_operator::on_entry(fsm::machine* m, fsm::state* s){}
void tokenizer_state_operator::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_operator::on_event(fsm::machine* m, fsm::event* e) 
{
	tokenizer*       tm = static_cast< tokenizer*       > ( m );
	tokenizer_event* te = static_cast< tokenizer_event* > ( e );
	if (nullptr == tm)
	{
		return;
	}
	if (nullptr == te)
	{
		return;
	}


	/*
	"~"  not      
	"!"  logic_not
	"*"  mul      
	"/"  div      
	"%"  mod      
	"+"  add      
	"-"  sub      
	"<<" bit_lshift      
	">>" bit_rshift      
	"<"  logic_lt 
	"<=" logic_lte
	">"  logic_gt 
	">=" logic_gte
	"==" logic_eq 
	"!=" logic_neq
	"&"  bit_and      
	"^"  bit_xor      
	"|"  bit_or       
	"&&" logic_and
	"||" logic_or 
	*/

	if ( 0u==tm->get_current_token_symbol_size() )
	{
		switch ( te->new_char )
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '~': 
		case '^': 
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
			break;
		}
	}

	if ( 0u==tm->get_current_token_symbol_size() )
	{
		switch ( te->new_char )
		{
		case '&': // &&     &
		case '|': // ||     |
		case '<': // <=     <<     <
		case '>': // >=     >>     >
		case '!': // !=     !
		case '=': // ==   
			{
				tm->add_character ( te->new_char );
				return;
			}
			break;
		}
	}

	if ( 1u==tm->get_current_token_symbol_size() )
	{
		rpn_char_t ch = *tm->get_current_token_symbol();

		if ('&'==ch)
		{
			if ( '&'==te->new_char )
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
			else
			{
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				tm->notify (e);
				return;
			}
		}

		if ('|'==ch)
		{
			if ( '|'==te->new_char )
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
			else
			{
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				tm->notify (e);
				return;
			}
		}
		if ('<'==ch)
		{
			if ( ( '<'==te->new_char ) || ( '='==te->new_char ) )
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
			else
			{
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				tm->notify (e);
				return;
			}
		}
		if ('>'==ch)
		{
			if ( ( '>'==te->new_char ) || ( '='==te->new_char ) )
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
			else
			{
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				tm->notify (e);
				return;
			}
		}
		if ('!'==ch)
		{
			if ( '='==te->new_char )
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
			else
			{
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				tm->notify (e);
				return;
			}
		}
		if ('='==ch)
		{
			if ( '='==te->new_char )
			{
				tm->add_character ( te->new_char );
				tm->end_character ( _token_operator );
				transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
				return;
			}
		}
	}

	transition (tm, tm->get_state_instance(tokenizer_state_id_error) );
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_numeric:: tokenizer_state_numeric(): 
	tokenizer_state(tokenizer_state_id_numeric),
	_has_dot(false)
{
}

//===========================================================================
void tokenizer_state_numeric::on_entry(fsm::machine* m, fsm::state* s)
{
	_has_dot = false;
}
void tokenizer_state_numeric::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_numeric::on_event(fsm::machine* m, fsm::event* e)
{
	tokenizer*       tm = static_cast< tokenizer*       > ( m );
	tokenizer_event* te = static_cast< tokenizer_event* > ( e );
	if (nullptr == tm)
	{
		return;
	}
	if (nullptr == te)
	{
		return;
	}


	if ( '.' == te->new_char )
	{
		if (false==_has_dot)
		{
			tm->add_character ( te->new_char );
		}
		else
		{
			transition (tm, tm->get_state_instance(tokenizer_state_id_error) );
		}
		return;
	}

	if ( is_number_character ( te->new_char ) )
	{
		tm->add_character ( te->new_char );
		return;
	}

	if ( is_alphabet_character ( te->new_char ) )
	{
		transition (tm, tm->get_state_instance(tokenizer_state_id_error) );
	}
	else
	{
		tm->end_character ( _token_numeric );

		transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
		tm->notify (e);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_string:: tokenizer_state_string() :
	tokenizer_state(tokenizer_state_id_string)
{
}

//===========================================================================
void tokenizer_state_string::on_entry(fsm::machine* m, fsm::state* s){}
void tokenizer_state_string::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_string::on_event(fsm::machine* m, fsm::event* e)
{
	tokenizer*       tm = static_cast< tokenizer*       > ( m );
	tokenizer_event* te = static_cast< tokenizer_event* > ( e );
	if (nullptr == tm)
	{
		return;
	}
	if (nullptr == te)
	{
		return;
	}


	if ( '\"' == te->new_char )
	{
		tm->end_character ( _token_string );

		transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
		/*tm->notify (e);*/
		return;
	}

	tm->add_character ( te->new_char );
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer_state_identifier:: tokenizer_state_identifier() :
	tokenizer_state(tokenizer_state_id_identifier)
{
}

//===========================================================================
void tokenizer_state_identifier::on_entry(fsm::machine* m, fsm::state* s){}
void tokenizer_state_identifier::on_exit (fsm::machine* m, fsm::state* s){}
void tokenizer_state_identifier::on_event(fsm::machine* m, fsm::event* e)
{
	tokenizer*       tm = static_cast< tokenizer*       > ( m );
	tokenizer_event* te = static_cast< tokenizer_event* > ( e );
	if (nullptr == tm)
	{
		return;
	}
	if (nullptr == te)
	{
		return;
	}


	if ( ( '_' ==                 te->new_char  ) || 
	     ( is_number_character   (te->new_char) ) || 
		 ( is_alphabet_character (te->new_char) )  )
	{
		tm->add_character ( te->new_char );
	}
	else
	{
		tm->end_character ( _token_identifier );

		transition (tm, tm->get_state_instance(tokenizer_state_id_scan) );
		tm->notify (e);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
tokenizer::tokenizer() :
	_buffer(nullptr),
	_buffer_max_size(0u),
	_buffer_offset(0u),
	_current_token_symbol_pointer(nullptr),
	_current_token_symbol_size(0u),
	_current_token_symbol_offset(0u)
{
}

//===========================================================================
fsm::state* tokenizer::get_state_instance(tokenizer_state_id_t id)
{
	switch (id)
	{
	case tokenizer_state_id_scan      : return &_state_scan;
	case tokenizer_state_id_finish    : return &_state_finish;
	case tokenizer_state_id_error     : return &_state_error;
	case tokenizer_state_id_operator  : return &_state_operator;
	case tokenizer_state_id_numeric   : return &_state_numeric;
	case tokenizer_state_id_string    : return &_state_string;
	case tokenizer_state_id_identifier: return &_state_identifier;
	}

	return nullptr;
}

//===========================================================================
void tokenizer::reset(void)
{
	//-----------------------------------------------------------------------
	if (nullptr == _buffer || 0u == _buffer_max_size)
	{
		assert(0);
	}


	//-----------------------------------------------------------------------
	if (_buffer)
	{
		memset(_buffer, 0, _buffer_max_size);
	}
	_buffer_offset = 0u;


	//-----------------------------------------------------------------------
	_current_token_symbol_pointer = _buffer;
	_current_token_symbol_size = 0u;


	//-----------------------------------------------------------------------
	_tokens.clear();


	//-----------------------------------------------------------------------
	transition(get_state_instance(tokenizer_state_id_scan));
}

//===========================================================================
bool tokenizer::tokenize(const rpn_char_t* text, std::size_t length)
{
	//-----------------------------------------------------------------------
	reset();


	//-----------------------------------------------------------------------
	tokenizer_event e;
	std::size_t count;
	std::size_t index;
	count = length;
	for (index = 0u; index < count; index++)
	{
		e.new_char = text[index];
		notify(&e);
	}
	if (length > 0u)
	{
		e.new_char = '\0';
		notify(&e);
	}


	//-----------------------------------------------------------------------
	tokenizer_state* s;
	s = static_cast<tokenizer_state*>(get_state());
	if (s)
	{
		if (tokenizer_state_id_finish == s->get_id())
		{
			return true;
		}
	}

	return false;
}

//===========================================================================
const rpn_char_t* tokenizer::get_current_token_symbol(void)
{
	return _current_token_symbol_pointer;
}

std::size_t tokenizer::get_current_token_symbol_size(void)
{
	return _current_token_symbol_size;
}

//===========================================================================
void tokenizer::add_character(rpn_char_t c)
{
	_buffer_offset++;

	if (_buffer_offset < _buffer_max_size)
	{
		_current_token_symbol_pointer[_current_token_symbol_size++] = c;
	}
	else
	{
		assert(0);
	}
}

void tokenizer::end_character(token_type_t type)
{
	add_character('\0');


	token_t token(type, _current_token_symbol_pointer);
	_tokens.push_back(token);


	_current_token_symbol_pointer = &_current_token_symbol_pointer[_current_token_symbol_size];
	_current_token_symbol_size = 0u;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
