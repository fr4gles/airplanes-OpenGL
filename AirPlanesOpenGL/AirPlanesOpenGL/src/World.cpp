#include "World.h"
#include <iostream>
extern GLuint textureGround, textureSky[5];

World::World(int groundSize) : _groundSize(groundSize)
{
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
			glTranslated(floor(_existence[0][0]),0,floor(_existence[0][2]));
			for(int i=-_groundSize/2;i<_groundSize/2;i+=1.0f)
				for(int j=-_groundSize/2;j<_groundSize/2;j+=1.0f)
				{
					glBegin( GL_QUADS );
						glTexCoord2d(0.0,0.0); glVertex3f(i		,0.0,j);
						glTexCoord2d(1.0,0.0); glVertex3f(i+1.0	,0.0,j);
						glTexCoord2d(1.0,1.0); glVertex3f(i+1.0	,0.0,j+1.0);
						glTexCoord2d(0.0,1.0); glVertex3f(i		,0.0,j+1.0);
					glEnd();
				}
		glPopMatrix();
	glDisable( GL_TEXTURE_2D );
}

void World::renderSkybox()
{
	glPushMatrix();
	glTranslated(_existence[0][0],_existence[0][1],_existence[0][2]);
	for(int i=0;i<5;i++)
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, textureSky[i] );
		float scale=(float)_groundSize;
		float crossing = 0.5;
		glPushMatrix();
		if(i!=4)
			glRotatef(-i*-90.0,0,1,0);
		else
			glRotatef(90.0,1,0,0);
			glTranslated(-(scale)-scale/2.0,-scale/2.0,-scale/2.0+crossing);
			glBegin( GL_QUADS );
				glTexCoord2d(1,0); glVertex2d(scale,0.0);
				glTexCoord2d(1.0,0.0); glVertex2d(scale,0.0);
				glTexCoord2d(1.0,1.0); glVertex2d(scale,scale);
				glTexCoord2d(1,1.0); glVertex2d(scale,scale);
			glEnd();
			glTranslated(scale,0.0,0.0);
			glBegin( GL_QUADS );
				glTexCoord2d(0,0); glVertex2d(0.0,0.0);
				glTexCoord2d(1,0.0); glVertex2d(scale,0.0);
				glTexCoord2d(1,1.0); glVertex2d(scale,scale);
				glTexCoord2d(0,1.0); glVertex2d(0,scale);
				glEnd();
		glDisable( GL_TEXTURE_2D );
		glPopMatrix();
	}
	
	glPopMatrix();
}

World::~World(void)
{
}
