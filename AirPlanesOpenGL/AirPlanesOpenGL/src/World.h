#pragma once
#include "RootObject.h"
#include "freeglut\glut.h"

class World :
	public RootObject
{
public:
	World(int groundSize = 100);

	// ladowanie swiata - textur
	void initLoad();

	// ladowanie tekstur
	void load_texture(char *fn, GLuint &texture);

	void setFollow(RootObject *root);

	inline void doSth() { setPosition( _follow->getPosition()); };
	void render();

	void renderGround();
	void renderSkybox();

	~World(void);
private:
	GLuint textureGround, textureSky[5];
	RootObject *_follow;
	int _groundSize;
};


