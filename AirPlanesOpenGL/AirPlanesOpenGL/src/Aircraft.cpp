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
	if(_HP>0)
	{
		// tekstura samolotu
		glColor3f(_color.red,_color.green,_color.blue);
	
		glutSolidCone(0.1f,1.0f, 4, 5);
		//glCallList(AIRCRAFT_MODEL);		//problem z includami, jak siê uda skonsolidowaæ powinno dzia³aæ
	}
	else
	{
		//zmiana tekstury na wybuch
		glColor3f(0.9f,0.7f,0.0f);

		glutSolidSphere(2.0f,10,10);
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

	if(_HP < 1 && _existence[0][1] > 0.2f)
	{
		// zestrzelony kulami
		// po co to jest ?
		//this->addRotate(-0.3f,0,0);
		//speedUp(0.001);
			_isAlive += 1;
	}
	if(_existence[0][1] < 0.2f)
	{	// spadl
		//_existence[1][0] = 0.2f;
		dead();				//wybuch bo wpad³ w ziemiê
		speedDown(0.01);

		_isAlive += 1;
	}
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

	static bool init = false;

	if (!init) 
	{
		srand(time(NULL));
		init = true;
	}

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

GLint Aircraft::getHP() const
{
	return _HP;
}

void Aircraft::setHP(GLint newHP)
{
	_HP = newHP;
}

void Aircraft::respawn()
{
	_speed = 0.0f;
	_maxSpeed = 0.10f;
	_minSpeed = 0.0f;
	_move = std::vector<GLfloat>(3,0.0f);
	_HP = 50;
	

	_existence[0][0] = static_cast<GLfloat>(generateRandomPosition());
	_existence[0][1] = 0.35f;
	_existence[0][2] = static_cast<GLfloat>(generateRandomPosition());
	
	_existence[1][0] = 0.0f;
	_existence[1][1] = 0.0f;
	_existence[1][2] = 0.0f;

	_isAlive = 0;
}