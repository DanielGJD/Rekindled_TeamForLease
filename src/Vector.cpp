/*!
  \file   Vector.cpp
  \author Sam Montanari
  \date   9/16/15
  \brief
    Contains implementations for the Vector classes defined in Vector.h.
  \see Vector.h
*/

#include "Vector.h"
#include <iostream>
#include <cmath>
namespace ForLeaseEngine {

	/*!
	  \brief
	    Default constructor for Hcoord
	*/
	Hcoord::Hcoord() : x(0), y(0), w(0) { }

	/*!
	  \brief
	    Non-default constructor for Hcoord

	  \param X
	    X coordinate

	  \param Y
	    Y coordinate

	  \param W
	    W coordinate

	*/
	Hcoord::Hcoord(float X, float Y, float W) : x(X), y(Y), w(W) { }

	/*!
	  \brief
	    Prints vector/point to the screen
	*/
	void Hcoord::Print() const
	{
		std::cout << "Vector/Point:\n";
		std::cout << x << std::endl << y << std::endl << w << std::endl;
	}

	/*!
	  \brief
	    Accesses a coordinate of the vector/point

	  \param i
	    Coordinate to access

	  \return
	    Coordinate accessed
	*/
	float& Hcoord::operator[](int i)
	{
		return *(&x + i);
	}

	/*!
	  \brief
	    Accesses a coordinate of the vector/point (const version)

	  \param i
	    Coordinate to access

	  \return
	    Coordinate accessed
	*/
	float Hcoord::operator[](int i) const
	{
		return *(&x + i);
	}

	/*!
	  \brief
	    Default constructor for Points
	*/
	Point::Point() : Hcoord(0, 0, 1) {}

	/*!
	  \brief
	    Non-default constructor for Points

	  \param x
	    X coordinate

	  \param y
	    Y coordinate

	*/
	Point::Point(float x, float y) : Hcoord(x, y, 1) {}

	/*!
	  \brief
	    Converts to Point from Hcoord

	  \param v
	    Hcoord being converted to a point
	*/
	Point::Point(Hcoord& v) : Hcoord(v) { w = 1; }

	/*!
	  \brief
	    Default constructor for Vectors
	*/
	Vector::Vector() : Hcoord() {}

	/*!
	  \brief
	    Non-default constructor for Vectors

	  \param x
	    X coordinate

	  \param y
	    Y coordinate
	*/
	Vector::Vector(float x, float y) : Hcoord(x, y, 0) {}

	/*!
	  \brief
	    Converts to Vector from Hcoord

	  \param v
	    Hcoord being converted to a vector
	*/
	Vector::Vector(Hcoord& v) : Hcoord(v) { w = 0; }

	/*!
	  \brief
	    Normalizes a vector
	*/
	void Vector::Normalize()
	{
		float mag = DotProduct(*this, *this);
		mag = sqrt(mag);
		x /= mag;
		y /= mag;
	}

	/*!
	  \brief
	    Calculates the the dot product of two vectors

	  \param vec1
	    First vector of the dot product

	  \param vec2
	    Second vector of the dot product

	  \return
	    Result of the dot product
	*/
	float Vector::DotProduct(Vector vec1, Vector vec2)
	{
		return vec1[0] * vec2[0] + vec1[1] * vec2[1];
	}

	/*!
	  \brief
	    Rotates a vector

      \param vec
        Vector to rotate

      \param angle
        Angle of rotation
	*/
	Vector Vector::Rotate(Vector vec, float angle)
	{
	    float sinAngle = sin(angle);
	    float cosAngle = cos(angle);

	    return Vector(vec[0] * cosAngle - vec[1] * sinAngle,
                      vec[0] * sinAngle + vec[1] * cosAngle);
	}

	/*!
	  \brief
	    Rotates a vector

      \param vec
        Vector to rotate

      \param magnitude
        Scale magnitude
	*/
	Vector Vector::Scale(Vector vec, float magnitude) {
        return Vector(vec[0] * magnitude, vec[1] * magnitude);
	}

	/*!
	  \brief
	    Generates an Identity matrix
	*/
	Vector Point::operator-(const Point& rhs) const
	{
		Vector result;
		result[0] = (*this)[0] - rhs[0];
		result[1] = (*this)[1] - rhs[1];
		return result;
	}

    /*!
	  \brief
	    Displaces a point by a vector
	*/
	Point Point::operator+(const Vector& rhs) const
	{
	    return Point(x + rhs[0], y + rhs[1]);
	}
}
