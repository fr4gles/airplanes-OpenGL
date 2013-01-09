#include "freeglut\freeglut.h"
#include "Camera.h"
#include "Aircraft.h"

extern bool keys[256], specialkeys[256];
extern bool exitProgram;
extern Camera *camera;
extern Aircraft *aircraft;
extern int licznikStrzal;
extern bool strzelaj;
extern int nrWystrzelonejKuli;
extern int iloscKul;
extern const GLfloat accel;
extern GLfloat fogDensity;


void handleKeys(){
	if(keys[27]) 
	{
		exitProgram = true;
	}

	////////////CAMERA
	if(specialkeys[GLUT_KEY_LEFT]){
		camera->addDistance(0.0f,-0.1f,0.0f);
	}
	else if(specialkeys[GLUT_KEY_RIGHT]){
		camera->addDistance(0.0f,0.1f,0.0f);
	}
	if(specialkeys[GLUT_KEY_UP])
		camera->addDistance(0.1f,0.0f,0.1f);
	else if(specialkeys[GLUT_KEY_DOWN])
		camera->addDistance(-0.1f,0.0f,-0.1f);

	////////////AIRCRAFT
	if(aircraft->getHP() > 0)
	{
		if(keys[32])
		{
			licznikStrzal++;

			nrWystrzelonejKuli = licznikStrzal;
			strzelaj = true;

			if(licznikStrzal > iloscKul-2)
				licznikStrzal = -1;
		}

		if(keys['a']||keys['A'])  {
			aircraft->addRotate(0,0.8f,0);
			aircraft->setWingsRot(1.0f);
		}
		if(keys['d']||keys['D']) {
			aircraft->addRotate(0,-0.8f,0);
			aircraft->setWingsRot(-1.0f);
		}
		if(keys['s']||keys['S']) {
			aircraft->addRotate(-0.5f,0,0);
		}
		if(keys['w']||keys['W']) {
			aircraft->addRotate(0.5f,0,0);
		}

		if(keys['r']||keys['R'])
			aircraft->speedUp(accel);
		if(keys['f']||keys['F'])
			aircraft->speedDown(accel);
	}
	/////////////EXTRA VIEW
	if(keys['.']) {
		camera->setExtraRotation(0.0f,-90.f,0.0f);
	}
	else if(keys['b']||keys['B']) { 
		camera->setExtraRotation(0.0f,180.f,0.0f);
	}
	else if(keys[',']) {
		camera->setExtraRotation(0.0f,90.f,0.0f);
	}
	else {
		camera->setExtraRotation(0.0f,0.0f,0.0f);
	}
	if(keys['4']) {
		if(fogDensity<1.0)
			fogDensity+=0.005f;
	}
	if(keys['5']) {
		if(fogDensity>0.0)
			fogDensity-=0.005f;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	keys[key]=false;	
}

void keyDown(unsigned char key, int x, int y)
{
	keys[key]=true;
}

void specialKeyDown(int key, int x, int y)
{
	specialkeys[key]=true;
}

void specialKeyUp(int key, int x, int y)
{
	specialkeys[key]=false;
}
