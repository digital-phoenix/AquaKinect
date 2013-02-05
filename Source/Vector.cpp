#include "Vector.h"
#include <math.h>
#include <iostream>

Vector::Vector(){
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vector::Vector(GLfloat x, GLfloat y, GLfloat z, GLfloat w){
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector::Vector(const Vector &other){
	x = other.getX();
	y = other.getY();
	z = other.getZ();
}

Vector::Vector(GLfloat v[4]){
	x = v[0];
	y = v[1];
	z = v[2];
	w = v[3];
}

void Vector::setValues(GLfloat v[4]){
	x = v[0];
	y = v[1];
	z = v[2];
	w = v[3];
}


Vector & Vector::operator=(const Vector &other){
	if( this == &other){
		return *this;
	}

	x = other.getX();
	y = other.getY();
	z = other.getZ();	
	w = other.getW();

	return *this;
}

Vector & Vector::operator+=(const Vector &other){
	x += other.getX();
	y += other.getY();
	z += other.getZ();

	return *this;
}

Vector & Vector::operator*=(const Vector &other){
	GLfloat tempX, tempY, tempZ;

	tempX = y * other.getZ() - z * other.getY();
	tempY = x * other.getZ() - z * other.getX();
	tempZ = x * other.getY() - y * other.getX();

	x = tempX;
	y = tempY;
	z = tempZ;

	return *this;
}

Vector & Vector::operator-=(const Vector &other){
	x-= other.getX();
	y-= other.getY();
	z-= other.getZ();
	return *this;
}

Vector & Vector::operator*=(const GLfloat scalar){
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

const Vector Vector::operator*(const GLfloat scalar)const{
	Vector result(*this);
	result *= scalar;

	return result;
}

const Vector Vector::operator+(const Vector &other)const{
	Vector result(*this);
	result += other;
	return result;
}

const Vector Vector::operator-(const Vector &other)const{
	Vector result(*this);
	result -= other;
	return result;
}

const Vector Vector::operator*(const Vector &other)const{
	Vector result(*this);
	result *= other;
	return result;

}

GLfloat Vector::dot(Vector & other){
	return x * other.getX() + y * other.getY() + z * other.getZ();
}

GLfloat Vector::magnitudeSquared(){
	return dot(*this);
}

GLfloat Vector::magnitude(){
	return sqrt(magnitudeSquared());
}

Vector & Vector::normalize(){
	GLfloat mag = magnitude();
	if( mag < 0.00001){
		mag += 0.00001;
	}
	return (*this) *= 1/mag;
}

GLfloat Vector::getX()const{
	return x;
}

GLfloat Vector::getY()const{
	return y;
}

GLfloat Vector::getZ()const{
	return z;
}

GLfloat Vector::getW()const{
	return w;
}

void Vector::setX(GLfloat n){
	x = n;
}

void Vector::setY(GLfloat n){
	y = n;
}

void Vector::setZ(GLfloat n){
	z = n;
}

void Vector::setW(GLfloat n){
	w = n;
}

void Vector::print(){
	std::cout<<"\n vector:\n( " << x << " " << y << " " << z << ")\n";
}

Vector & Vector::operator=(const XnPoint3D &other){
	x = other.X;
	y = other.Y;
	z = other.Z;
	w = 1.0;
	return *this;
}
