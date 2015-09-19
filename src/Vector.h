/*!
  \file   Vector.h
  \author Sam Montanari
  \date   9/16/15
  \brief
    Defines the vector and point classes
  \see Vector.cpp
*/

#ifndef VECTOR_H
#define VECTOR_H

namespace ForLeaseEngine {
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
		Vector(Hcoord& v);
		void Normalize();
		static float DotProduct(Vector vec1, Vector vec2);
    static Vector Scale(Vector vec, float magnitude);
    static Vector Rotate(Vector vec, float angle);
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
		Point(Hcoord& v);
		Vector operator-(const Point& rhs) const;
		Point operator+(const Vector& rhs) const;
	};
}

#endif
