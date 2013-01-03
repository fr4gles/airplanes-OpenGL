#pragma once
#include "rootobject.h"
class Aircraft :
	public RootObject
{
public:
	Aircraft(): RootObject()
	{
		_speed = 0.0f;
		_maxSpeed = 10.0f;
		_minSpeed = 0.0f;
		_move = std::vector<GLfloat>(3,0.0f);

		_existence[0][0] = -10.0f;
		_existence[0][1] = 0.35f;
		_existence[0][2] = -10.0f;
		_existence[1][1] = -0.0f;
	}

	void doSth();
	void render();
	void addRotate(GLfloat x, GLfloat y, GLfloat z);

	void speedUp(GLfloat newSpeed);
	void speedDown(GLfloat newSpeed);
	inline GLfloat getSpeed() const { return _speed; }

	~Aircraft(){};

private:
	GLfloat _speed, _maxSpeed, _minSpeed;
	std::vector<GLfloat> _move;
};

