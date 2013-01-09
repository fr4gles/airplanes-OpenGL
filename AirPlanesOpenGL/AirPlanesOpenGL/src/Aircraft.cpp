#include "Aircraft.h"
#include "freeglut\glut.h"

extern GLuint AIRCRAFT_MODEL;

void Aircraft::render()
{
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);

	glTranslatef(_existence[0][0],_existence[0][1],_existence[0][2]);	
	glRotatef(-_existence[1][0], 1,0,0 );
	glRotatef(_existence[1][1], 0, cos(_existence[1][0] * 3.1415/180.0),sin(_existence[1][0] * 3.1415/180.0) );
	glRotatef(_existence[1][2],0,0,1);
	if(_HP > static_cast<GLint>(0))
	{
		glDisable(GL_TEXTURE_2D);
		// tekstura samolotu
		glColor3f(_color.red,_color.green,_color.blue);
	
		//glutSolidCone(0.1f,1.0f, 4, 5); KIJAAAANKA
		glScalef(0.1f,0.1f,0.1f);
		glRotatef(-90.0f,1,0,0);
		glCallList(AIRCRAFT_MODEL);
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		//zmiana tekstury na wybuch
		glColor3f(0.9f,0.7f,0.0f);

		glutSolidSphere(_blowUp,10,10);
		if(_blowUp < 2.0f)
			_blowUp += 0.01f;
	}
	
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

	_rotSpeed = 1.0f;

	if(fabs(_wingsRot) > 0.f)
	{
		_existence[1][2] += _wingsRot*_rotSpeed;

		if(_existence[1][2] > 20.f) 
			_existence[1][2] = 20.f;
		
		if(_existence[1][2] < -20.f) 
			_existence[1][2] = -20.f;		
	}

	if(_HP < 1 && _existence[0][1] > 0.2f)
	{
		_isAlive +=1;
	}
	if(_existence[0][1] < 0.3f)
	{
		_existence[1][0] = 0.2f;
		dead();				//wybuch bo wpad³ w ziemiê
		speedDown(0.01);

		_isAlive += 1;
	}
	else if(_existence[0][1] > 50.0f)
		_existence[1][0] = -_existence[1][0];

	if(_existence[1][2] != 0.0f && _wingsRot == 0.0f) 
		_existence[1][2]+= -_rotSpeed*(_existence[1][2]/fabs(_existence[1][2]));

	_wingsRot = 0.0f;
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

void Aircraft::attacked()
{
	_HP -= 1;
}

void Aircraft::dead()
{
	_HP = 0;
}

void Aircraft::setHP(const GLint& newHP)
{
	_HP = newHP;
}

void Aircraft::respawn()
{
	_speed = 1.0f/5.0f * _maxSpeed;
	_existence[0][0] = static_cast<GLfloat>(generateRandomPosition());
	_existence[0][1] = 0.35f;
	_existence[0][2] = static_cast<GLfloat>(generateRandomPosition());
	
	_existence[1][0] = 0.0f;
	_existence[1][1] = 0.0f;
	_existence[1][2] = 0.0f;

	_blowUp = 0.45f;
	_HP = 50;
	_isAlive = 0;
	_wingsRot = 0.0f;
}

void Aircraft::setBlowUp(const GLfloat& newBlow)
{
	//_blowUp = 0.45f;
	_blowUp += newBlow;
}

void Aircraft::setWingsRot(const GLfloat& newRot)
{
	_wingsRot = newRot;
}