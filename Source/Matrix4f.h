#ifndef __Matrix4F__
#define __Matrix4F__

#include "Matrix.h"
#include<gl/glew.h>

class Matrix4f : public Matrix{
	
	public:
		Matrix4f():Matrix(4,4){};
		Matrix4f(int rows, int cols):Matrix(4,4){}
		Matrix4f(const Matrix &m);
		Matrix4f(int rows, int cols, std::vector<GLfloat> m):Matrix(4,4,m){}

		Matrix4f & operator=(const Matrix &other);

		const Vector operator*(const Vector &vec)const;
		const Matrix4f operator*(const Matrix4f &other)const{return Matrix::operator*(other);}


		void Ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal);
		void translate(GLfloat x, GLfloat y, GLfloat z);

		void uniformMatrix( GLuint position);
};

#endif