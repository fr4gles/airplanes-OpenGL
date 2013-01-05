#include "Bullet.h"
#include "freeglut\glut.h"

void Bullet::render()
{
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);

	glTranslatef(_existence[0][0],_existence[0][1],_existence[0][2]);	
	glColor3f(_color.red,_color.green,_color.blue);
	glutSolidSphere(0.05f,10,10);

	glColor4f(1.0,1.0,1.0,1.0);
	glDisable(GL_COLOR_MATERIAL);
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
}

void Bullet::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
	_color.red = red;
	_color.green = green;
	_color.blue = blue;
}