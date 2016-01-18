/*!
  \file   Vector.cpp
  \author Sam Montanari
  \date   9/16/15
  \brief
    Contains implementations for the Vector classes defined in Vector.h.
  \see Vector.h

  \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Vector.h"
#include <iostream>
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846
#endif // PI

namespace ForLeaseEngine {

    bool Near(float f1, float f2) {
        return (-Epsilon <= f1 - f2) && (f1 - f2 <= Epsilon);
    }

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

    Hcoord& Hcoord::operator=(const Hcoord& rhs) {
        x = rhs.x;
        y = rhs.y;
        w = rhs.w;
        return *this;
    }

    std::ostream& operator<<(std::ostream& os, const Hcoord& hcoord) {
        os << "[" << hcoord.x << " " << hcoord.y << " " << hcoord.w << "]";
        return os;
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
	Point::Point(const Hcoord& v) : Hcoord(v) { w = 1; }

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
	Vector::Vector(const Hcoord& v) : Hcoord(v) { w = 0; }

	/*!
	  \brief
	    Normalizes a vector
	*/
	void Vector::Normalize()
	{
		float mag = Magnitude();
		x /= mag;
		y /= mag;
	}

	float Vector::Magnitude()
	{
        return sqrt(MagnitudeSquared());
	}

	float Vector::MagnitudeSquared()
	{
	    return x * x + y * y;
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
	float Vector::DotProduct(const Vector& vec1, const Vector& vec2)
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

      \return
        The rotated vector
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
	    Scales a vector

      \param vec
        Vector to scale

      \param magnitude
        Scale magnitude

      \return
        The scaled vector
	*/
	Vector Vector::Scale(Vector const& vec, float magnitude) {
        return Vector(vec[0] * magnitude, vec[1] * magnitude);
	}

	/*!
        \brief
            Finds the angle between the +x axis and the vector, Range [0, 2*pi]

        \param vec
            Vector to calculate the angle of

        \return
            Angle of the vector
    */
    float Vector::ToAngle(Vector const& vec) {
        float angle = atan2(vec[1], vec[0]) - atan2(0, 1);
        return angle;
    }

    /*!
        \brief
            Finds the angle between two vectors

        \param vec1
            First vector

        \param vec2
            Second vector

        \return
            Angle between the vectors
    */
    float Vector::AngleBetween(Vector const& vec1, Vector const& vec2) {
        float angle = atan2(vec2[1], vec2[0]) - atan2(vec1[1], vec1[0]);
        return angle;
    }

    Vector& Vector::operator=(const Vector& rhs) {
        x = rhs.x;
        y = rhs.y;

        return *this;
    }

	/*!
		Adds another vector to this vector.

		\param rhs
			Vector to add to this vector.

		\return
			A reference to this vector.
	*/
	Vector& Vector::operator+=(const Vector& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	/*!
		Scales the vector with an operator overload.

		\param rhs
			Float to scale this by.

		\return
			The scaled vector.
	*/
	Vector Vector::operator*(float rhs) const { return Scale(*this, rhs); }

    /*!
        \brief
            Subtracts two vectors

        \param rhs
            Vector to subtract from this vector

        \return
            Difference of the vectors
    */
	Vector Vector::operator-(const Vector& rhs) {
        return Vector(x - rhs.x, y - rhs.y);
	}

	/*!
        \brief
            Adds two vectors

        \param rhs
            Vector to add to this vector

        \return
            Sum of the vectors
    */
    Vector Vector::operator+(Vector const& rhs) {
        return Vector(x + rhs.x, y + rhs.y);
    }

	/*!
	  \brief
	    Subtracts two points, making a vector

      \param rhs
        Point to subtract from this point

      \return
        Vector representing the displacement from a point to this point
	*/
	Vector Point::operator-(const Point& rhs) const
	{
		Vector result;
		result[0] = (*this)[0] - rhs[0];
		result[1] = (*this)[1] - rhs[1];
		return result;
	}

    std::ostream& operator<<(std::ostream& os, const Vector& vector) {
        os << "<" << vector.x << "," << vector.y << ">";
        return os;
    }

    /*!
	  \brief
	    Displaces a point by a vector
	*/
	Point Point::operator+(const Vector& rhs) const {
	    return Point(x + rhs[0], y + rhs[1]);
	}

	/*!
        \brief
          "Scales" a point
    */
    Point Point::operator*(float rhs) const {
        return Point(rhs * x, rhs * y);
    }

	Point& Point::operator+=(const Vector& rhs) {
		x += rhs[0];
		y += rhs[1];

		return *this;
	}

	Point& Point::operator-=(const Vector& rhs) {
		x -= rhs[0];
		y -= rhs[1];

		return *this;
	}

	Point Point::operator-() {
        return Point(-x, -y);
	}

	float Point::Distance(const Point& p1, const Point& p2) {
        return sqrt(DistanceSquared(p1, p2));
	}

	float Point::DistanceSquared(const Point& p1, const Point& p2) {
        return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
	}

    bool Point::InBetween(const Point& p1, const Point& p2, const Point& q) {
        float distOfTri = Point::Distance(p1, q) + Point::Distance(q, p2);
        float distOfLine = Point::Distance(p1, p2);
        float distDiff = distOfTri - distOfLine;

        return -Epsilon < distDiff && distDiff < Epsilon;
    }

    bool Point::InBetween(const Point& p1, const Point& p2) const {
        return Point::InBetween(p1, p2, *this);
    }

    std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << "(" << point.x << "," << point.y << ")";
        return os;
    }
}
