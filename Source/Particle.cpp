#include "Particle.h"


Particle::Particle(): velocity(), acceleration(), position(){
	init();
}

Particle::Particle( const Particle &other){
	*this = other;
}

Particle::Particle( Vector &position): velocity(), acceleration(), position(){
	init();
	this->position = position;
}

void Particle::init(){
	GLfloat vertices[] = { 0.0, 0.0, 0.0, 1.0,
					10.0, 10.0, 0.0, 1.0,
					10.0, 0.0, 0.0, 1.0};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER,	sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Particle::update(){
	position += velocity;
	velocity += acceleration;
}

void Particle::draw(GLuint vPos, GLuint matLoc, Matrix4f projectionMatrix){
	Matrix4f modelViewMatrix;
	modelViewMatrix.translate( position.getX(), position.getY(), position.getZ());

	Matrix4f modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
	modelViewProjectionMatrix.uniformMatrix(matLoc);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(
		vPos,				// attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		0,					// normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);	

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

Particle & Particle::operator=(const Particle &other){
	if( this == &other){
		return *this;
	}

	position = other.getPosition();
	velocity = other.getVelocity();
	acceleration = other.getAcceleration();
	buffer = other.getBuffer();
	vao = other.getVao();

	return *this;
}

void Particle::setVelocity(Vector &v){
	velocity = v;
}

void Particle::setAcceleration(Vector &a){
	acceleration = a;
}

void Particle::setPosition(Vector &p){
	position = p;
}

Vector Particle::getVelocity()const{
	return velocity;
}
Vector Particle::getPosition()const{
	return position;
}
Vector Particle::getAcceleration()const{
	return acceleration;
}

GLuint Particle::getBuffer()const{
	return buffer;
}

GLuint Particle::getVao()const{
	return vao;
}
