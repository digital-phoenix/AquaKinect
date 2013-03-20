#ifndef __PARTICLE__
#define __PARTICLE__

#include "Vector.h"
#include "Matrix4f.h"

class Particle{
	
	public:
		Particle();
		Particle(const Particle &p);
		Particle( Vector &position);

		void draw(GLuint vPos, GLuint matLoc, Matrix4f projectionMatrix);
		void update();

		Particle & operator=(const Particle &other);

		void setVelocity(Vector &v);
		void setAcceleration(Vector &a);
		void setPosition(Vector &p);

		Vector getVelocity()const;
		Vector getPosition()const;
		Vector getAcceleration()const;
		GLuint getBuffer()const;
		GLuint getVao()const;

	private:
		void init();
		GLuint buffer;
		GLuint vao;
		Vector position;
		Vector velocity;
		Vector acceleration;
};

#endif