#pragma once
#include "rootobject.h"
#include "utils.h"
class Bullet :
	public RootObject
{
public:
	Bullet(): RootObject()
	{
		_speed = 0.25f;
		_move = std::vector<GLfloat>(3,0.0f);
		_color = Color(0.0f,0.0f,1.0f);
		//pos
		_existence[0][0] = -1000.0f;
		_existence[0][1] = -1000.0f;
		_existence[0][2] = -1000.0f;
		//rot
		_existence[1][0] =  0.0f;
		_existence[1][1] =  0.0f;
		_existence[1][2] =  0.0f;
	}

	Bullet(const Color &color): RootObject()
	{
		_speed = 0.25f;
		_move = std::vector<GLfloat>(3,0.0f);
		_color = color;
		//pos
		_existence[0][0] = -1000.0f;
		_existence[0][1] = -1000.0f;
		_existence[0][2] = -1000.0f;
		//rot
		_existence[1][0] =  0.0f;
		_existence[1][1] =  0.0f;
		_existence[1][2] =  0.0f;
	}

	void doSth();
	void render();
	void setColor(const GLfloat &red,const GLfloat &green,const GLfloat &blue);

	inline const GLfloat& getSpeed() const { return _speed; }
	

	~Bullet(){};

private:
	GLfloat _speed;
	std::vector<GLfloat> _move;
	Color _color;
};



