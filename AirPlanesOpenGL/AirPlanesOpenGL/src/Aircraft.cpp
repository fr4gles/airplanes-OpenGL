#include "Aircraft.h"
#include "freeglut\glut.h"

void Aircraft::render()
{
	glPushMatrix();

	glTranslatef(_existence[0][0],_existence[0][1],_existence[0][2]);	
		glRotatef(-_existence[1][0], 1,0,0 );
		glRotatef(_existence[1][1], 0, cos(_existence[1][0] * 3.1415/180.0),sin(_existence[1][0] * 3.1415/180.0) );
		glRotatef(_existence[1][2],0,0,1);
	glutSolidCone(0.1f,1.0f, 4, 5);

	glPopMatrix();
}

void Aircraft::doSth()
{
	_move[0] = sin(_existence[1][1]*3.14f/180.f);
	_move[1] = sin(_existence[1][0]*3.14f/180.f);
	_move[2] = cos(_existence[1][1]*3.14f/180.f);

	_existence[0][0] -= _move[0]*_speed;
	_existence[0][1] -= _move[1]*_speed;
	_existence[0][2] -= _move[2]*_speed;
}

void Aircraft::speedUp(GLfloat newSpeed)
{
	_speed += newSpeed;
	if(_speed >= _maxSpeed)
		_speed = _maxSpeed;
}

void Aircraft::speedDown(GLfloat newSpeed)
{
	_speed -= newSpeed;
	if(_speed <= _minSpeed)
		_speed = _minSpeed;
}

void Aircraft::addRotate(GLfloat x, GLfloat y, GLfloat z)
{
	RootObject::addRotate(x, y, z);
}