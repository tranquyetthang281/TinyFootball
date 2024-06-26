#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D &Vector2D::Add(const Vector2D &vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D &Vector2D::Subtract(const Vector2D &vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D &Vector2D::Multiply(const Vector2D &vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D &Vector2D::Divide(const Vector2D &vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2D &operator+(Vector2D &v1, const Vector2D &v2)
{
	return v1.Add(v2);
}

Vector2D &operator-(Vector2D &v1, const Vector2D &v2)
{
	return v1.Subtract(v2);
}

Vector2D &operator*(Vector2D &v1, const Vector2D &v2)
{
	return v1.Multiply(v2);
}

Vector2D &operator/(Vector2D &v1, const Vector2D &v2)
{
	return v1.Divide(v2);
}

Vector2D &Vector2D::operator+=(const Vector2D &vec)
{
	return this->Add(vec);
}

Vector2D &Vector2D::operator-=(const Vector2D &vec)
{
	return this->Subtract(vec);
}

Vector2D &Vector2D::operator*=(const Vector2D &vec)
{
	return this->Multiply(vec);
}

Vector2D &Vector2D::operator/=(const Vector2D &vec)
{
	return this->Divide(vec);
}

Vector2D &Vector2D::operator*(const int &i)
{
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2D &Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;

	return *this;
}

float Vector2D::Dot(const Vector2D& vec)
{
	return this->x * vec.x + this->y * vec.y;
}


std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}
void Vector2D::Normalize()
{
	double l = Length();
	if (l < 0.0001)
	{
		this->x = 1;
		this->y = 0;
	}
	else
	{
		this->x /= l;
		this->y /= l;
	}
}
double Vector2D::Length()
{
	return sqrt(x * x + y * y);
}
Vector2D &Vector2D::Rotate(double angle)
{
	this->x = cos(angle) * this->x - sin(angle) * this->x;
	this->y = sin(angle) * this->y + cos(angle) * this->y;
	return *this;
}