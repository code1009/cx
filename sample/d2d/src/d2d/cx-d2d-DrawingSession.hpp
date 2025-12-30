#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DrawingSession
{
public:
	Context* _Context{ nullptr };

public:
	explicit DrawingSession(Context* context);
	virtual ~DrawingSession() = default;

public:
	DrawingSession(const DrawingSession&) = delete;
	DrawingSession& operator=(const DrawingSession&) = delete;
	DrawingSession(DrawingSession&&) = delete;
	DrawingSession& operator=(DrawingSession&&) = delete;

public:
	Context* getContext(void) const noexcept
	{
		return _Context;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
