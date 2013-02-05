#ifndef __MATRIX__
#define __MATRIX__

#include<iostream>
#include<vector>
#include "Vector.h"
#include<gl/glew.h>

class Matrix{

	public:
		Matrix();
		Matrix(int rows, int cols);
		Matrix(const Matrix &m);
		Matrix(int rows, int cols, std::vector<GLfloat> m);

		Matrix & operator=(const Matrix &other);
		Matrix & operator+=(const Matrix &other);
		Matrix & operator*=(const Matrix &other);
		Matrix & operator*=(const Vector &vec);
		Matrix & operator-=(const Matrix &other);
		const Matrix operator+(const Matrix &other)const;
		const Matrix operator-(const Matrix &other)const;
		const Matrix operator*(const Matrix &other)const;
		const Matrix operator*(const Vector &vec)const;
		
		int getRows()const;
		int getCols()const;

		void setElement(int row, int col, GLfloat val);
		GLfloat getElement(int row, int col)const;

		void identity();
		void print();

	protected:
		int rows;
		int cols;
		std::vector<GLfloat> m;
};

#endif