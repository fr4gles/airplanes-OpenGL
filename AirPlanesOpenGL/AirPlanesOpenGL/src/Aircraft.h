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
		_color = Color(0.0f,0.0f,1.0f);

		_isAlive = 0;

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

	inline const GLint& getHP() const { return _HP; }
	void setHP(const GLint &newHP);
	inline const GLfloat &getSpeed() const { return _speed; }

	inline void addToIsAlive(const GLint &isAlive) { _isAlive += isAlive; }
	inline const GLint& getIsAlive() const { return _isAlive; }

	~Aircraft(){};

private:
	GLfloat _speed, _maxSpeed, _minSpeed;
	std::vector<GLfloat> _move;
	GLint _HP;
	Color _color;
	GLint _isAlive;
};

