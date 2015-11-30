/*!
  \file   Vector.h
  \author Sam Montanari
  \date   9/16/15
  \brief
    Defines the vector and point classes
  \see Vector.cpp

  \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include <iostream>

#ifndef VECTOR_H
#define VECTOR_H

namespace ForLeaseEngine {

    const float Epsilon = 0.001f;
    bool Near(float f1, float f2);

	/*!
	  \class Hcoord

	  \brief
	    Base class that vectors and points inherit from
	*/
	class Hcoord
	{
	protected:
		//! X, Y, and W coordinates
		float x, y, w;

	public:
		Hcoord();
		Hcoord(float X, float Y, float W);
		void Print() const;
		float& operator[](int i);
		float operator[](int i) const;
        Hcoord& operator=(const Hcoord& rhs);
        friend std::ostream& operator<<(std::ostream& os, const Hcoord& hcoord);
	};

	/*!
	  \class Vector

	  \brief
	    2D homogeneous vector
	*/
	class Vector : public Hcoord
	{
	public:
		Vector();
		Vector(float x, float y);
		Vector(const Hcoord& v);
		void Normalize();
		float Magnitude();
        float MagnitudeSquared();
		static float DotProduct(const Vector& vec1, const Vector& vec2);
        static Vector Scale(Vector const& vec, float magnitude);
        static Vector Rotate(Vector vec, float angle);
        static float ToAngle(Vector const& vec);
        static float AngleBetween(Vector const& vec1, Vector const& vec2);
        Vector& operator=(const Vector& rhs);
        Vector& operator+=(const Vector& rhs);
        Vector operator*(float rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const Vector& vector);
	};

	/*!
	  \class Point

	  \brief
	    2D homogeneous point
	*/
	class Point : public Hcoord
	{
	public:
		Point();
		Point(float x, float y);
		Point(const Hcoord& v);
		Vector operator-(const Point& rhs) const;
		Point operator+(const Vector& rhs) const;
		Point& operator+=(const Vector& rhs);
		Point& operator-=(const Vector& rhs);
		static float Distance(const Point& p1, const Point& p2);
		static float DistanceSquared(const Point& p1, const Point& p2);
		Point operator-();
        static bool InBetween(const Point& p1, const Point& p2, const Point& q);
        bool InBetween(const Point& p1, const Point& p2) const;
        friend std::ostream& operator<<(std::ostream& os, const Point& point);
	};
}

#endif
