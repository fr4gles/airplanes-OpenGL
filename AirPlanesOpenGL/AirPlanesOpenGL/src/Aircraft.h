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

		_speed = 0.0f;
		_maxSpeed = 0.10f;
		_minSpeed = 0.0f;
		_move = std::vector<GLfloat>(3,0.0f);
		_HP = 50;

		respawn();
	}

	Aircraft(const std::vector<double> &tmp): RootObject()
	{
		_speed = 0.0f;
		_maxSpeed = 0.10f;
		_minSpeed = 0.0f;
		_move = std::vector<GLfloat>(3,0.0f);
		_HP = 50;
		_blowUp = 2.0f;
		_wingsRot = 0.0f;

		_isAlive = 0;
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

	void setWingsRot(const GLfloat& newRot);
	void setBlowUp(const GLfloat& newBlow);
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
	GLfloat _blowUp, _wingsRot, _rotSpeed;
};

