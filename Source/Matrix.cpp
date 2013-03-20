#include"Matrix.h"
#include<iostream>

/*void testMatrices( Matrix4f test, Matrix4f otherTest){
	Matrix4f result;


	std::cout<<"test:\n";
	test.print();
 	std::cout<<"other test:\n";
	otherTest.print();
	result = test * otherTest;
	std::cout<<"test * other test =\n";
	result.print();

	result.identity();
	std::cout<<"identity\n";
	result.print();

	result = test + otherTest;
	std::cout<<"test + otherTest\n";
	result.print();

	result.Ortho(0, 10, 0, 10, -1, 1);
	std::cout<<"ortho(0,10,0,10,-1,1)\n";
	result.print();

	Vector r = result * Vector(10,0,0);
	std::cout<<"orthor * (10,0,0)\n";
	r.print();

	r = result * Vector(0,10,0);
	std::cout<<"orthor * (0, 10,0)\n";
	r.print();

	r = result * Vector(0,0,10);
	std::cout<<"orthor * (1,0,0)\n";
	r.print();

	result.translate(10, 20, 30);
	std::cout<<"translate(10,20,30)\n";
	result.print();
}*/

Matrix::Matrix(int rows, int cols){
	this->rows = rows;
	this->cols = cols;
	m.resize(rows * cols);
	identity();
}

Matrix::Matrix(){
	rows = 0;
	cols = 0;
}

Matrix::Matrix(const Matrix &m){
	*this = m;
}

Matrix::Matrix( int rows, int cols, std::vector<GLfloat> m){
	this->rows = rows;
	this->cols = cols;
	
	for( unsigned int i = 0; i < m.size(); i++){
		this->m[i] = m[i];
	}
}

Matrix & Matrix::operator=(const Matrix &other){
	if(this == &other){
		return *this;
	}

	rows = other.getRows();
	cols = other.getCols();
		
	m.resize(rows * cols);
	for( int i = 0; i < rows; i++){
		for( int j = 0; j < cols; j++){
			setElement( i, j, other.getElement(i, j));
		}
	}

	return *this;
}

Matrix & Matrix::operator+=(const Matrix &other){
	if( rows != other.getRows() || cols != other.getCols()){
		return *this;
	}

	for( int i = 0; i < rows; i++){
		for( int j = 0; j < cols; j++){
			setElement( i, j, getElement(i, j) + other.getElement(i, j));
		}
	}

	return *this;
}

Matrix & Matrix::operator-=(const Matrix &other){
	if( rows != other.getRows() || cols != other.getCols()){
		return *this;
	}

	for( int i = 0; i < rows; i++){
		for( int j = 0; j < cols; j++){
			setElement( i, j, getElement(i, j) - other.getElement(i, j));
		}
	}

	return *this;
}

Matrix & Matrix::operator*=(const Matrix &other){
	if( cols != other.getRows()){
		return *this;
	}

	Matrix result(rows, other.getCols());

	for( int i = 0; i < rows; i++){
		for( int j = 0; j < other.getCols(); j++){
			GLfloat val = 0;
			for( int k=0; k<cols; k++){
				val += getElement(i, k) * other.getElement(k, j);
			}
			result.setElement(i, j, val);
		}
	}

	*this = result;

	return *this;
}

const Matrix Matrix::operator+(const Matrix &other)const{
	Matrix result = *this;
	result += other;
	return result;
}

const Matrix Matrix::operator*(const Matrix &other)const{
	Matrix result = *this;
	result *= other;
	return result;
}

const Matrix Matrix::operator-(const Matrix &other)const{
	Matrix result = *this;
	result -= other;
	return result;
}

int Matrix::getRows()const{
	return rows;
}

int Matrix::getCols()const{
	return cols;
}

void Matrix::setElement(int row, int col, GLfloat val){
	if( row > rows || col > cols){
		return;
	}
	m[col * cols + row] = val;
}

GLfloat Matrix::getElement(int row, int col)const{
	return m.at(col * cols + row);
}

Matrix & Matrix::operator*=(const Vector &vec){
	(*this) = (*this) * vec;
	return *this;
}

const Matrix Matrix::operator*(const Vector &vec)const{
	if( cols != 4){
		return *this;
	}
	
	Matrix result(rows, 1);
	for( int i =0; i < rows; i++){
		result.setElement(i, 0, getElement(i, 0) * vec.getX() +
							getElement(i, 1) * vec.getY() +
							getElement(i, 2) * vec.getZ() +
							getElement(i, 3) * vec.getW());
	}

	return result;
}

void Matrix::identity(){
	for( int i = 0; i < rows; i++){
		for( int j = 0; j < cols; j++){
			if( i == j)
				setElement(i, j, 1);
			else
				setElement(i, j, 0);
		}
	}
}

void Matrix::print(){
	for(int row=0; row<4; row++){
		for(int col = 0; col<4; col++){
			std::cout<< getElement(row, col)  << " ";
		}
		std::cout<<"\n";
	}
}
