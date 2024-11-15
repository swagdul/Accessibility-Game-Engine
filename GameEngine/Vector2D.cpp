#include "Vector2D.h"

Vector2D::Vector2D()
{
	m_x = 0.0f;
	m_y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->m_x = x;
	this->m_y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vector)
{
	this->m_x += vector.m_x;
	this->m_y += vector.m_y;

	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vector)
{
	this->m_x -= vector.m_x;
	this->m_y -= vector.m_y;

	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vector)
{
	this->m_x *= vector.m_x;
	this->m_y *= vector.m_y;

	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vector)
{
	this->m_x /= vector.m_x;
	this->m_y /= vector.m_y;

	return *this;
}

Vector2D& operator+(Vector2D& vector1, const Vector2D& vector2)
{
	return vector1.Add(vector2);
}

Vector2D& operator-(Vector2D& vector1, const Vector2D& vector2)
{
	return vector1.Subtract(vector2);
}

Vector2D& operator*(Vector2D& vector1, const Vector2D& vector2)
{
	return vector1.Multiply(vector2);
}

Vector2D& operator/(Vector2D& vector1, const Vector2D& vector2)
{
	return vector1.Divide(vector2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vector)
{
	return this->Add(vector);
}

Vector2D& Vector2D::operator-=(const Vector2D& vector)
{
	return this->Subtract(vector);
}

Vector2D& Vector2D::operator*=(const Vector2D& vector)
{
	return this->Multiply(vector);
}

Vector2D& Vector2D::operator/=(const Vector2D& vector)
{
	return this->Divide(vector);
}

Vector2D& Vector2D::operator*(const int& i)
{
	this->m_x *= i;
	this->m_y *= i;

	return *this;
}

Vector2D& Vector2D::Zero()
{
	this->m_x = 0;
	this->m_y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vector)
{
	stream << "(" << vector.m_x << "," << vector.m_y << ")";
	return stream;
}
