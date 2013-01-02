#include "World.h"


World::World(void)
{
}

void World::initLoad()
{
	// tu bedzie ladowanie
}

void World::setFollow(RootObject *root)
{
	_follow = root;
	doSth();
}

void World::load_texture(char *fn, GLuint &texture)
{
	int w,h;
	GLenum format, type;
	GLvoid *data;

	load_targa(fn, w,h, format, type, data);
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, format, type, data );

	delete data;
}

World::~World(void)
{
}
