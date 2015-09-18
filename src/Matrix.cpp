/*!
  \file   Matrix.cpp
  \author Sam Montanari
  \date   9/16/15
  \brief
    Contains implementations for the Matrix class defined in Matrix.h.
  \see Matrix.h
*/

#include "Matrix.h"
#include <iostream> /* printf cout */
#include <cmath>    /* sin, cos    */

namespace ForLeaseEngine {

	/*!
	  \brief
	    Generates an Identity matrix
	*/
	Matrix::Matrix()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				m[i][j] = i == j ? 1.0f : 0.0f;
		}
	}

	/*!
	  \brief
	    Matrix copy constructor
	  
	  \param rhs
	    Matrix being copied from
	*/
	Matrix::Matrix(const Matrix& rhs)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				m[i][j] = rhs[i][j];
		}
	}

	/*!
	  \brief
	    Accesses a row of the matrix

	  \param i
	    row to access

	  \return
	    the row accessed
	*/
	Hcoord& Matrix::operator[](int i)
	{
		return m[i];
	}

	/*!
	  \brief
	    Accesses a row of the matrix (const version)

	  \param i
	    row to access

	  \return
	    the row accessed
	*/
	const Hcoord& Matrix::operator[](int i) const
	{
		return m[i];
	}

	/*!
	  \brief
	    Multiplies two matrices together

	  \param rhs
	    Matrix on the right hand side of the operation

	  \return
	    The result of the multiplication
	*/
	Matrix Matrix::operator*(const Matrix& rhs) const
	{
		Matrix result;
		result[0][0] = m[0][0] * rhs[0][0] + m[0][1] * rhs[1][0] + m[0][2] * rhs[2][0];
		result[0][1] = m[0][0] * rhs[0][1] + m[0][1] * rhs[1][1] + m[0][2] * rhs[2][1];
		result[0][2] = m[0][0] * rhs[0][2] + m[0][1] * rhs[1][2] + m[0][2] * rhs[2][2];
		result[1][0] = m[1][0] * rhs[0][0] + m[1][1] * rhs[1][0] + m[1][2] * rhs[2][0];
		result[1][1] = m[1][0] * rhs[0][1] + m[1][1] * rhs[1][1] + m[1][2] * rhs[2][1];
		result[1][2] = m[1][0] * rhs[0][2] + m[1][1] * rhs[1][2] + m[1][2] * rhs[2][2];
		result[2][0] = m[2][0] * rhs[0][0] + m[2][1] * rhs[1][0] + m[2][2] * rhs[2][0];
		result[2][1] = m[2][0] * rhs[0][1] + m[2][1] * rhs[1][1] + m[2][2] * rhs[2][1];
		result[2][2] = m[2][0] * rhs[0][2] + m[2][1] * rhs[1][2] + m[2][2] * rhs[2][2];
		return result;
	}

	/*!
	  \brief
	    Multiplies a matrix and a vector/point

	  \param rhs
	    Vector/point on the right hand side of the operation

	  \return
	    Resulting vector/point
	*/
	Hcoord Matrix::operator*(const Hcoord& rhs) const
	{
		Hcoord result;
		result[0] = m[0][0] * rhs[0] + m[0][1] * rhs[1] + m[0][2] * rhs[2];
		result[1] = m[1][0] * rhs[0] + m[1][1] * rhs[1] + m[1][2] * rhs[2];
		return result;
	}

	/*!
	  \brief
	    Adds two matrices

	  \param rhs
	    Matrix on the right hand side of the operation

	  \return
	    Resulting matrix
	*/
	Matrix Matrix::operator+(const Matrix& rhs) const
	{
		Matrix result(*this);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				result[i][j] += rhs[i][j];
		}

		return result;
	}

	/*!
	  \brief
	    Subtracts two matrices

	  \param rhs
	    Matrix on the right hand side of the operation

	  \return
	    Resulting matrix
	*/
	Matrix Matrix::operator-(const Matrix& rhs) const
	{
		Matrix result(*this);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				result[i][j] -= rhs[i][j];
		}

		return result;
	}

	/*!
	  \brief
	    Generates a rotation matrix from radians

	  \param radians
	    Angle of rotation

	  \return
	    Resulting matrix
	*/
	Matrix RotationRad(float radians)
	{
		Matrix result;
		result[0][0] = cosf(radians);
		result[0][1] = -sinf(radians);
		result[1][0] = sinf(radians);
		result[1][1] = cosf(radians);
		return result;
	}

	/*!
	  \brief
	    Generates a rotation matrix from degrees

	  \param degrees
	    Angle of rotation

	  \return
	    Resulting matrix
	*/
	Matrix RotationDeg(float degrees)
	{
		Matrix result;
		float radians = degrees * 22.0f / 7 / 180;
		result[0][0] = cosf(radians);
		result[0][1] = -sinf(radians);
		result[1][0] = sinf(radians);
		result[1][1] = cosf(radians);
		return result;
	}

	/*!
	  \brief
	    Generates a translation matrix from a vector

	  \param v
	    Translation vector

	  \return
	    Resulting matrix
	*/
	Matrix Translation(const Vector& v)
	{
		Matrix result;
		result[0][2] = v[0];
		result[1][2] = v[1];
		return result;
	}

	/*!
	  \brief
	    Generates a scaling matrix

	  \param x
	    Scale in the x direction

	  \param y
	    Scale in the y direction

	  \return
	    Resulting matrix
	*/
	Matrix Scale(float x, float y)
	{
		Matrix result;
		result[0][0] = x;
		result[1][1] = y;
		return result;
	}

	/*!
	  \brief
	    Prints the matrix to the screen
	*/
	void Matrix::Print() const
	{
		std::cout << "Matrix:\n";
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				printf("%4.3f ", m[i][j]);

			std::cout << std::endl;
		}
	}

}
