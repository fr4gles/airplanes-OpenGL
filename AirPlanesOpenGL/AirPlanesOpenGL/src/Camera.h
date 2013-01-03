#pragma once
#include "rootobject.h"
#include <vector>

class Camera :
	public RootObject
{
public:
	Camera(void);

	void setLookAt(GLfloat x,GLfloat y,GLfloat z);
	inline std::vector<GLfloat> Camera::getLookAt() { return _lookAt; }

	void doSth();
	void render(){};
	void setFollow(RootObject* follow);

	void addDistance(GLfloat x,GLfloat y,GLfloat z);
	void setExtraRotation(GLfloat x,GLfloat y,GLfloat z);

	~Camera(void);

//private:
	std::vector<GLfloat>	_lookAt,
							_distance,
							_extraRot;

	RootObject *_follow;


};

