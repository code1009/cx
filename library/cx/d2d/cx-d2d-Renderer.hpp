/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Renderer
{
public:
	std::function<void(DrawingSession*)> drawingHandler;

private:
	Context* _Context{ nullptr };
	std::uint32_t _cx{ 0 };
	std::uint32_t _cy{ 0 };

public:
	explicit Renderer(Context* context);
	explicit Renderer(Context* context, std::uint32_t cx, std::uint32_t cy);

public:
	virtual ~Renderer();

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

protected:
	virtual bool createDeviceIndependentResources(void);
	virtual void destroyDeviceIndependentResources(void);

protected:
	virtual bool createDeviceResources(void);
	virtual void destroyDeviceResources(void);

public:
	void resize(std::uint32_t cx, std::uint32_t cy);

public:
	virtual void render(DrawingSession* ds);

protected:
	virtual void draw(DrawingSession* ds);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




