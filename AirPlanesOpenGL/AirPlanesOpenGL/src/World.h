#pragma once
#include "RootObject.h"
#include "freeglut\glut.h"

class World :
	public RootObject
{
public:
	World(int groundSize = 100);

	void setFollow(RootObject *root);

	inline void doSth() { setPosition( _follow->getPosition()); };
	void render();

	void renderGround();
	void renderSkybox();

	~World(void);
private:
	RootObject *_follow;
	int _groundSize;
};


