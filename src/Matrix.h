/*!
  \file   Matrix.h
  \author Sam Montanari
  \date   9/16/15
  \brief
    Defines the matrix class
  \see Matrix.cpp
*/

#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
namespace ForLeaseEngine {
	/*!
	  \class Matrix

	  \brief
	    2D affine matrix
	*/
	class Matrix
	{
	private:
		//! Columns of the matrix
		Hcoord m[3];

	public:
		Matrix();
		Matrix(const Matrix& rhs);
		Hcoord& operator[](int i);
		const Hcoord& operator[](int i) const;
		Matrix operator*(const Matrix& rhs) const;
		Hcoord operator*(const Hcoord& rhs) const;
		Matrix operator+(const Matrix& rhs) const;
		Matrix operator-(const Matrix& rhs) const;
		void Print() const;
	};

	Matrix RotationRad(float radians);
	Matrix RotationDeg(float degrees);
	Matrix Translation(const Vector& v);
	Matrix Scale(float x, float y);
}

#endif
