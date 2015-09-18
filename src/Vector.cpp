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
		mag = sqrtf(mag);
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
	float DotProduct(Vector vec1, Vector vec2)
	{
		return vec1[0] * vec2[0] + vec1[1] * vec2[1];
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
}
