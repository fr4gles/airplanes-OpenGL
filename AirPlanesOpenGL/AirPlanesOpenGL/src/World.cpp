#include "World.h"
#include "targa.h"
#include <iostream>

World::World(int groundSize) : _groundSize(groundSize)
{
	//
	/*textureGround = 0;
	for(int i=0;i<5;++i)
		textureSky[0] = 0;*/
}

void World::initLoad()
{
	// tu bedzie ladowanie
	load_texture("textures/ground.tga",textureGround);
	
	int w,h;
	GLenum format, type;
	GLvoid *data;

	//load_targa("textures/ground.tga", w,h, format, type, data);
	//glBindTexture( GL_TEXTURE_2D, textureGround );
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	//gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, format, type, data );

	//delete data;

	//std::cout << " = = = " << textureGround << std::endl;

	for(int i=0;i<5;++i)
		std::cout << " = = = " << textureSky[i] << std::endl;

	load_texture("textures/front.tga",textureSky[0]);
	/*load_targa("textures/front.tga", w,h, format, type, data);
	glBindTexture( GL_TEXTURE_2D, textureSky[0] );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, format, type, data );

	delete data;
*/

	load_texture("textures/right.tga",textureSky[1]);
	/*load_targa("textures/right.tga", w,h, format, type, data);
	glBindTexture( GL_TEXTURE_2D, textureSky[1] );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, format, type, data );

	delete data;*/



	load_texture("textures/back.tga",textureSky[2]);
	load_texture("textures/left.tga",textureSky[3]);

	load_texture("textures/top.tga",textureSky[4]);
	/*	load_targa("textures/top.tga", w,h, format, type, data);
	glBindTexture( GL_TEXTURE_2D, textureSky[4] );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, format, type, data );

	delete data;*/




	//std::cout << " = = " << textureGround << std::endl;

	for(int i=0;i<5;++i)
		std::cout << " = = " << textureSky[i] << std::endl;
}

void World::setFollow(RootObject *root)
{
	_follow = root;
	doSth();
}

void World::render()
{
	renderSkybox();
	renderGround();
}

void World::renderGround()
{
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureGround );
		glPushMatrix();
			glTranslated(floor(_existence[0][0]),/*-_groundSize/2*/0,floor(_existence[0][2]));
			for(int i=-_groundSize/2;i<_groundSize/2;i+=5.0f)
				for(int j=-_groundSize/2;j<_groundSize/2;j+=5.0f)
				{
					glBegin( GL_QUADS );
						glTexCoord2d(0.0,0.0); glVertex3f(i		,0.0,j);
						glTexCoord2d(1.0,0.0); glVertex3f(i+5.0	,0.0,j);
						glTexCoord2d(1.0,1.0); glVertex3f(i+5.0	,0.0,j+5.0);
						glTexCoord2d(0.0,1.0); glVertex3f(i		,0.0,j+5.0);
					glEnd();
				}
		glPopMatrix();
	glDisable( GL_TEXTURE_2D );
}

void World::renderSkybox()
{
	glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		glTranslated(_existence[0][0],_existence[0][1],_existence[0][2]);
		glEnable( GL_TEXTURE_2D );
		for(int i=0;i<5;i++)
		{
			
			glBindTexture( GL_TEXTURE_2D, textureSky[i] );

			float x = 1.0; // z dziwnego powodu jest przesuniecie:/;/ ktore musze likwidowac na czuja
			float scale=(float)_groundSize;
			float crossing = 0.5;
			glPushMatrix();
			if(i!=4)
				glRotatef(-i*-90.0,0,1,0);
			else
				glRotatef(90.0,1,0,0);
				glTranslated(-(x*scale)-scale/2.0,-scale/2.0,-scale/2.0+crossing);
				glBegin( GL_QUADS );
					glTexCoord2d(x,0); glVertex2d(x*scale,0.0);
					glTexCoord2d(1.0,0.0); glVertex2d(scale,0.0);
					glTexCoord2d(1.0,1.0); glVertex2d(scale,scale);
					glTexCoord2d(x,1.0); glVertex2d(x*scale,scale);
				glEnd();
				glTranslated(scale,0.0,0.0);
				glBegin( GL_QUADS );
					glTexCoord2d(0,0); glVertex2d(0.0,0.0);
					glTexCoord2d(x,0.0); glVertex2d(x*scale,0.0);
					glTexCoord2d(x,1.0); glVertex2d(x*scale,scale);
					glTexCoord2d(0,1.0); glVertex2d(0,scale);
					glEnd();
			glPopMatrix();
		}
		glDisable( GL_TEXTURE_2D );
		glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void World::load_texture(char *fn, GLuint &texture)
{
	int w,h;
	GLenum format, type;
	GLvoid *data;

	load_targa(fn, w,h, format, type, data);
	glGenTextures(1,&texture);
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h, format, type, data );

	delete data;
}

World::~World(void)
{
	//for ( int i=0; i < _existence.size(); ++i )
	//	delete [] _existence[i];
}
