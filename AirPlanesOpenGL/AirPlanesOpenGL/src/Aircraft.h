#pragma once
#include "rootobject.h"
#include "utils.h"
#include <time.h>

class Aircraft :
	public RootObject
{
public:
	GLuint AIRCRAFT_MODEL;

	Aircraft(): RootObject()
	{
		respawn();
	}

	Aircraft(std::vector<double> &tmp): RootObject()
	{
		Aircraft();
		_color = Color(1.0f,0.0f,0.0f);
		_existence[0][0] = tmp[0];
		_existence[0][1] = tmp[1];
		_existence[0][2] = tmp[2];
		_existence[1][0] = tmp[3];
		_existence[1][1] = tmp[4];
		_existence[1][2] = tmp[5];	
	}

	void doSth();
	void render();
	GLint generateRandomPosition(); 
	void addRotate(GLfloat x, GLfloat y, GLfloat z);

	void speedUp(GLfloat newSpeed);
	void speedDown(GLfloat newSpeed);
	void attacked();
	void dead();
	void respawn();

	GLint getHP() const;
	void setHP(GLint newHP);
	inline GLfloat getSpeed() const { return _speed; }

	~Aircraft(){};

private:
	GLfloat _speed, _maxSpeed, _minSpeed;
	std::vector<GLfloat> _move;
	GLint _HP;
	Color _color;
};

