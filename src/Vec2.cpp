#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
	:x(0.0f), y(0.0f)
{

}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{

}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator / (const float val) const
{
	return Vec2(x / val, y / val);
}



void Vec2::operator += (const float rhs)
{
	x += rhs, y += rhs;
}

void Vec2::operator -= (const float rhs)
{
	x -= rhs, y -= rhs;
}

void Vec2::operator *= (const float rhs)
{
	x *= rhs, y *= rhs;
}

void Vec2::operator /= (const float rhs)
{
	x /= rhs, y /= rhs;
}

bool Vec2::operator == (const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return (x != rhs.x && y != rhs.y);
}

void Vec2::operator += (const Vec2& rhs)
{
	x += rhs.x, y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x, y -= rhs.y;
}

void Vec2::operator *= (const Vec2& rhs)
{
	x *= rhs.x, y *= rhs.y;
}

void Vec2::operator /= (const Vec2& rhs)
{
	x /= rhs.x, y /= rhs.y;
}


float Vec2::distsq(const Vec2& rhs) const
{	
	return (((x - rhs.x) * (x - rhs.x)) + ((y - rhs.y) * (y - rhs.y)));
}

float Vec2::magnitude() const
{
	return sqrtf(x * x + y * y);
}

Vec2& Vec2::normalize()
{
	float mag = magnitude();
	if (mag > 0.0f) 
	{ 
		// Safety check
		x = x / mag;
		y = y / mag;
	}
	return *this;
}


