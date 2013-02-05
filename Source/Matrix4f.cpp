#include"Matrix4f.h"


Matrix4f::Matrix4f(const Matrix &m):Matrix(4, 4){
	if( m.getCols() == 4 && m.getRows() == 4){
		*this = m;
	}
}

void Matrix4f::Ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal){
	identity();
	setElement(0,0, 2 / (right - left));
	setElement( 1, 1, 2 / ( top - bottom));
	setElement( 2, 2,  -2 / ( farVal - nearVal));
	setElement( 3, 3, 1);
	setElement( 0, 3, -( right + left) / ( right - left));
	setElement( 1, 3,  -( top + bottom) / ( top - bottom));
	setElement( 2, 3,  -( farVal + nearVal) / ( farVal - nearVal));
}

Matrix4f & Matrix4f::operator=(const Matrix &other){
	if( this == &other || other.getCols() != 4 || other.getRows() != 4){
		return *this;
	}
	Matrix::operator=(other);
	return *this;
}

void Matrix4f::translate(GLfloat x, GLfloat y, GLfloat z){
	identity();
	setElement(0, 3, x);
	setElement(1, 3, y);
	setElement(2, 3, z);
}

void Matrix4f::uniformMatrix( GLuint position){
	glUniformMatrix4fv(position, 1, false, &m[0]);
}

const Vector Matrix4f::operator*(const Vector &vec)const{
	GLfloat v[4];
	for( int i =0; i < rows; i++){
		v[i] = getElement(i, 0) * vec.getX() +
				getElement(i, 1) * vec.getY() +
				getElement(i, 2) * vec.getZ() +
				getElement(i, 3) * vec.getW();
	}

	return Vector(v);
}
