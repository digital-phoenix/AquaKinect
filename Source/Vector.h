#ifndef __VECTOR__
#define __VECTOR__

#include <XnCppWrapper.h>
#include <XnVPointControl.h>
#include<gl/glew.h>

class Vector{
	public:
		Vector();
		Vector(GLfloat x, GLfloat y, GLfloat z, GLfloat w = 0);
		Vector(const Vector &other);
		Vector(GLfloat v[4]);

		Vector & operator=(const Vector &other);
		Vector & operator=(const XnPoint3D &other);
		Vector & operator+=(const Vector &other);
		Vector & operator*=(const Vector &other);
		Vector & operator*=(const GLfloat scalar);
		Vector & operator-=(const Vector &other);

		const Vector operator+(const Vector &other)const;
		const Vector operator-(const Vector &other)const;
		const Vector operator*(const Vector &other)const;
		const Vector operator*(const GLfloat scalar)const;

		GLfloat dot( Vector & other);
		Vector & normalize();
		GLfloat magnitude();
		GLfloat magnitudeSquared();

		void print();

		GLfloat getX()const;
		GLfloat getY()const;
		GLfloat getZ()const;
		GLfloat getW()const;

		void setX(GLfloat n);
		void setY(GLfloat n);
		void setZ(GLfloat n);
		void setW(GLfloat n);
		void setValues(GLfloat v[4]);

	private:	
		GLfloat x,y,z,w;
};

#endif