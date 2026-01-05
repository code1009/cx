#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
struct Point
{
    float X { 0.0f };
    float Y { 0.0f };

    Point() noexcept = default;

    constexpr Point(float X, float Y) noexcept :
        X(X),
        Y(Y)
    {
    }
};

//===========================================================================
constexpr bool operator==(Point const& left, Point const& right) noexcept
{
    return left.X == right.X && left.Y == right.Y;
}

constexpr bool operator!=(Point const& left, Point const& right) noexcept
{
    return !(left == right);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
struct Size
{
    float Width { 0.0f };
    float Height { 0.0f };

    Size() noexcept = default;

    constexpr Size(float width, float height) noexcept :
        Width (width), 
        Height(height)
    {
    }
};

//===========================================================================
constexpr bool operator==(Size const& left, Size const& right) noexcept
{
    return left.Width == right.Width && left.Height == right.Height;
}

constexpr bool operator!=(Size const& left, Size const& right) noexcept
{
    return !(left == right);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
struct Rect
{
    float X { 0.0f };
    float Y { 0.0f };
    float Width { 0.0f };
    float Height { 0.0f };

    Rect() noexcept = default;

    constexpr Rect(float X, float Y, float Width, float Height) noexcept :
        X(X), Y(Y), Width(Width), Height(Height)
    {
    }

    constexpr Rect(Point const& point, Size const& size)  noexcept :
        X(point.X), Y(point.Y), Width(size.Width), Height(size.Height)
    {
    }
};

//===========================================================================
constexpr bool operator==(Rect const& left, Rect const& right) noexcept
{
    return left.X == right.X && left.Y == right.Y && left.Width == right.Width && left.Height == right.Height;
}

constexpr bool operator!=(Rect const& left, Rect const& right) noexcept
{
    return !(left == right);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
