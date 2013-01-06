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
	//void load_texture(int w, int h, GLenum format, GLenum type, GLvoid* data, GLuint &texture);

	void setFollow(RootObject *root);

	inline void doSth() { setPosition( _follow->getPosition()); };
	void render();

	void renderGround();
	void expandGround(int &x, int &z);
	void renderSkybox();

	~World(void);
private:
	//GLuint textureGround, textureSky[5];
	RootObject *_follow;
	int _groundSize;
};


