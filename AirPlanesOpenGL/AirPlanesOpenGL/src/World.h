#pragma once
#include "RootObject.h"
#include "targa.h"
#include "freeglut\glut.h"

class World :
	public RootObject
{
public:
	World(void);

	// ladowanie swiata - textur
	void initLoad();

	// ladowanie tekstur
	void load_texture(char *fn, GLuint &texture);

	void setFollow(RootObject *root);

	inline void doSth() { setPosition( _follow->getPosition()); };
	void render() {};

	~World(void);
private:
	GLuint textureFloor, textureSky[5];
	RootObject *_follow;
};

