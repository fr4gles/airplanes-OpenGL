#include "RootObject.h"


RootObject::RootObject(void)
{
	for(int i=0;i<2;++i)
		_existence.push_back(new GLfloat[3]);

	for(int i=0;i<_existence.size();++i)
		for(int j=0;j<3;++j)
			_existence[i][j] = 0.0f;
}

RootObject::RootObject(GLuint listID)
{
	for(int i=0;i<2;++i)
		_existence.push_back(new GLfloat[3]);

	for(int i=0;i<_existence.size();++i)
		for(int j=0;j<3;++j)
			_existence[i][j] = 0.0f;

	setList(listID);
}

void RootObject::setPosition(GLfloat x,GLfloat y,GLfloat z) 
{ 
	_existence[0][0] = x;
	_existence[0][1] = y;
	_existence[0][2] = z;
}

void RootObject::setPosition(GLfloat pos[3]) 
{ 
	for(int i=0;i<3;++i)
		_existence[0][i] = pos[i];
}

void RootObject::addRotate(GLfloat x, GLfloat y, GLfloat z) 
{
	_existence[1][0] += x;
	_existence[1][1] += y;
	_existence[1][2] += z;
}

void RootObject::addRotate(GLfloat rot[3]) 
{
	_existence[1][0] = rot[0];
	_existence[1][1] = rot[1];
	_existence[1][2] = rot[2];
}

RootObject::~RootObject(void)
{
	for ( int i=0; i < _existence.size(); ++i )
		delete [] _existence[i];
}
