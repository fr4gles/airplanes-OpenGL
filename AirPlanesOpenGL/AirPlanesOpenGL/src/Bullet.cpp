#include "Bullet.h"
#include "freeglut\glut.h"

void Bullet::render()
{
	glPushMatrix();

	glTranslatef(_existence[0][0],_existence[0][1],_existence[0][2]);	
		glutSolidSphere(0.05f,10,10);

	glPopMatrix();
}

void Bullet::doSth()
{
	_move[0] = sin(_existence[1][1]*3.14f/180.f);
	_move[1] = sin(_existence[1][0]*3.14f/180.f);
	_move[2] = cos(_existence[1][1]*3.14f/180.f);

	_existence[0][0] -= _move[0]*_speed;
	_existence[0][1] -= _move[1]*_speed;
	_existence[0][2] -= _move[2]*_speed;

	for(int i=0;i<3;++i)
		if(_existence[0][i] < -1000.0f)
			_existence[0][i] = -1000.0f;
}