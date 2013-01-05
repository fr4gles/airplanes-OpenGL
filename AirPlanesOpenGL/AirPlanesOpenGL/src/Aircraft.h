#pragma once
#include "rootobject.h"
#include "utils.h"
#include <time.h>

class Aircraft :
	public RootObject
{
public:
	Aircraft(): RootObject()
	{
		_speed = 0.0f;
		_maxSpeed = 0.10f;
		_minSpeed = 0.0f;
		_move = std::vector<GLfloat>(3,0.0f);
		_HP = 50;
		_color = Color(0.0f,0.0f,1.0f);

		_existence[0][0] = static_cast<GLfloat>(generateRandomPosition());
		_existence[0][1] = 0.35f;
		_existence[0][2] = static_cast<GLfloat>(generateRandomPosition());
		_existence[1][1] = 0.0f;
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
	inline GLfloat getSpeed() const { return _speed; }

	~Aircraft(){};

private:
	GLfloat _speed, _maxSpeed, _minSpeed;
	std::vector<GLfloat> _move;
	GLint _HP;
	Color _color;
};

