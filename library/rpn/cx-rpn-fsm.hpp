#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::rpn::fsm
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class event;
class state;
class machine;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class machine
{
private:
	state* _state{ nullptr };
	
public:
	machine() = default;
	virtual ~machine() = default;

public:
	virtual void   transition (state*);
	virtual void   notify     (event*);
	virtual state* get_state  (void  ) const;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class event
{
public:
	event() = default;
	virtual ~event() = default;

public:
	virtual void notify (machine*, state*);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class state
{
public:
	state() = default;
	virtual ~state() = default;
	
public:
	virtual void on_entry (machine*, state*);
	virtual void on_exit  (machine*, state*);
	virtual void on_event (machine*, event*);

public:
	virtual void transition (machine*, state*);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




