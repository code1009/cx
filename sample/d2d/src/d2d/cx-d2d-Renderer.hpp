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
private:
	Context* _Context{ nullptr };
	std::uint32_t _Width{ 0 };
	std::uint32_t _Height{ 0 };

public:
	explicit Renderer(Context* context);

public:
	virtual ~Renderer();

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

public:
	void resize(std::uint32_t width, std::uint32_t height);

public:
	virtual void render(void);

protected:
	virtual bool createDeviceIndependentResources(void);
	virtual void destroyDeviceIndependentResources(void);

protected:
	virtual bool createDeviceResources(void);
	virtual void destroyDeviceResources(void);

protected:
	virtual void draw(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




