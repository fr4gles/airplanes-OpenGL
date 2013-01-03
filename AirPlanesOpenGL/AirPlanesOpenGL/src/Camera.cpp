#include "Camera.h"


Camera::Camera(void)
{
	_lookAt = std::vector<GLfloat>(3,0.0f);
	_distance = std::vector<GLfloat>(3,0.0f);
	_extraRot = std::vector<GLfloat>(3,0.0f);

	setLookAt(0.0f,0.0f,0.0f);

	_distance[0] = 4.0f;
	_distance[1] = 1.0f;
	_distance[2] = 4.0f;
}

void Camera::setLookAt(GLfloat x,GLfloat y,GLfloat z)
{ 
	_lookAt[0] = x;
	_lookAt[1] = y;
	_lookAt[2] = z;
}

void Camera::doSth()
{
	// kamera za obiektem
	
	GLfloat* followPos = _follow->getPosition();
	GLfloat* followRot = _follow->getRotation();
	

	GLfloat x = followPos[0] + _distance[0]*sin((followRot[1]+_extraRot[1])*3.14f/180.f);
	GLfloat y = followPos[1] /*+ _distance[1]/**sin((followRot[0]+_extraRot[1])*3.14f/180.f)*/;
	GLfloat z = followPos[2] + _distance[2]*cos((followRot[1]+_extraRot[1])*3.14f/180.f);


	this->setPosition(x, y , z);
	this->setLookAt(followPos[0],followPos[1],followPos[2]);
}

void Camera::setFollow(RootObject* follow)
{
	//
	_follow = follow;
		
	GLfloat* followPos = _follow->getPosition();
	this->setPosition(followPos[0] + _distance[0], followPos[1] + _distance[1] , followPos[2] + _distance[2]);
}

void Camera::addDistance(GLfloat x,GLfloat y,GLfloat z)
{
	_distance[0]+=x;
	_distance[1]+=y;
	_distance[2]+=z;

	//if(_distance[0] < minDistance)
	//	_distance[0] = minDistance;
	//else if(_distance[0] > maxDistance)
	//	_distance[0] = maxDistance;

	//if(_distance[1] < minDistance)
	//	_distance[1] = minDistance;
	//else if(_distance[1] > maxDistance)
	//	_distance[1] = maxDistance;

	//if(_distance[2] < minDistance)
	//	_distance[2] = minDistance;
	//else if(_distance[2] > maxDistance)
	//	_distance[2] = maxDistance;
}

void Camera::setExtraRotation(GLfloat x,GLfloat y,GLfloat z)
{
	_extraRot[0]=x;
	_extraRot[1]=y;
	_extraRot[2]=z;
}

Camera::~Camera(void)
{
	//
}
