#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Application : public cx::runtime::WindowApplication
{
public:
	Application() = default;

public:
	virtual ~Application() = default;

	//-----------------------------------------------------------------------
	// cx::runtime::WindowApplication
public:
	virtual bool initialize(void) override;
	virtual void terminate(void) override;

	//-----------------------------------------------------------------------
	// Application
public:
	void run(void);
	void launch(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Application* getApplication(void);

