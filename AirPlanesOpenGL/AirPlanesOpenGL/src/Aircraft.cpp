#include "Aircraft.h"
#include "freeglut\glut.h"

void Aircraft::render()
{
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);

	glTranslatef(_existence[0][0],_existence[0][1],_existence[0][2]);	
	glRotatef(-_existence[1][0], 1,0,0 );
	glRotatef(_existence[1][1], 0, cos(_existence[1][0] * 3.1415/180.0),sin(_existence[1][0] * 3.1415/180.0) );
	glRotatef(_existence[1][2],0,0,1);
	glColor3f(_color.red,_color.green,_color.blue);
	
	glutSolidCone(0.1f,1.0f, 4, 5);

	glColor4f(1.0,1.0,1.0,1.0);
	glDisable(GL_COLOR_MATERIAL);
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

	if(_existence[0][1] < 0.2f)
		_existence[1][0] = -_existence[1][0];
	else if(_existence[0][1] > 50.0f)
		_existence[1][0] = -_existence[1][0];
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

GLint Aircraft::generateRandomPosition()
{ 
	srand(time(NULL));
	return rand()%21-10;
}