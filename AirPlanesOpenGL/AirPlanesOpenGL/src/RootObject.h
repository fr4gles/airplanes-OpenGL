#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <vector>

class RootObject
{
public:
	// konstruktory
	RootObject(void);
	RootObject(GLuint listID);

	// lista - ID obiektu
	inline void setList(GLuint listID) { _listID = listID; };
	inline GLuint getList() { return _listID; }

	// pozycja
	void setPosition(GLfloat position[3]);
	void setPosition(GLfloat x,GLfloat y,GLfloat z);
	inline GLfloat* getPosition() { return _existence[0];};	

	// obrót obiektu
	virtual void addRotate(GLfloat x, GLfloat y, GLfloat z);
	inline GLfloat* getRotate() { return _existence[1] ; }

	// przesuniêcie
	void move(GLfloat x, GLfloat y, GLfloat z) 
	{ 
		_existence[0][0] += x;
		_existence[0][1] += y;
		_existence[0][2] += z;
	}
	
	virtual void doSth() = 0;
	virtual void render() = 0;

	virtual ~RootObject(void);

protected:
	GLuint	_listID;

	//1 elem pos , 2 elem rot 
	std::vector<GLfloat*> _existence; 
};

