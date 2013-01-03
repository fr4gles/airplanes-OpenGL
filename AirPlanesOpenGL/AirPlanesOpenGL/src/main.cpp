#define _CRT_SECURE_NO_WARNINGS

#include "chat_client.hpp"

#include "glew.h"
#include "glfw.h"

#include "vsync.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\GL.h>
#include <string>

#include "freeglut/freeglut.h"

#include "World.h"
#include "Camera.h"
#include "Aircraft.h"

bool keys[256], specialkeys[256];

int figura=1;
float scale_scene = 0.0f;

float	camera_distance=10.0f,
		camera_theta=0.2f,
		camera_fi=1.2f; // ustawienia kamery

float pos_x=0.0,pos_y=0.0,pos_z=0.0; // pozycja xyz
float scale_x=1.0,scale_y=1.0,scale_z=1.0; // skala xyz
float rot_x=0.0,rot_y=0.0,rot_z=0.0; // rotacja xyz

int mouse_button,mouse_x,mouse_y; // ruchy mysza

// textury
GLuint texture[10],tex_num;

const GLfloat accel = 0.05f;

// zmienne klas
World* world;
Camera *camera;
static Aircraft *aircraft;

typedef std::vector<std::pair<std::string,std::vector<double>>> Player;
Player players;

//void handleKeys()
//{	
//	//if (keys['1']) figura=1;
//	//if (keys['2']) figura=2;
//	//if (keys['3']) figura=3;
//	//if (keys['4']) figura=4;
//	//if (keys['5']) figura=5;
//	//if (keys['6']) figura=6;	
//	//if (keys['7']) figura=7;
//	
//	if (keys['9']) scale_scene+=0.02f;
//	if (keys['0']) scale_scene-=0.02f;
//
//	if(keys[27]) exit(0);
//
//	if (keys['a']) camera_fi-=0.01f;				// lewo
//	if (keys['d']) camera_fi+=0.01f;		// prawo
//	if (keys['s']) camera_theta+=0.01f;		// gora
//	if (keys['w']) camera_theta-=0.01f;		// dol
//
//
//	if (specialkeys[100]) camera_fi-=0.01f;        //lewo 
//	if (specialkeys[102]) camera_fi+=0.01f;        //prawo
//	if (specialkeys[101]) camera_theta+=0.01f;     //gora
//	if (specialkeys[103]) camera_theta-=0.01f;     //dol
//
//	//if (keys[113]) pos_x-=0.01f; if (keys[119]) pos_x+=0.01f;
//	//if (keys[97] ) pos_y-=0.01f; if (keys[115]) pos_y+=0.01f;
//	//if (keys[122]) pos_z-=0.01f; if (keys[120]) pos_z+=0.01f;
//
//	//if (keys[101]) scale_x-=0.02f; if (keys[114]) scale_x+=0.02f;
//	//if (keys[100]) scale_y-=0.02f; if (keys[102]) scale_y+=0.02f;
//	//if (keys[99] ) scale_z-=0.02f; if (keys[118]) scale_z+=0.02f;
//
//	//if (keys[116]) rot_x-=0.5f; if (keys[121]) rot_x+=0.5f;
//	//if (keys[103]) rot_y-=0.5f; if (keys[104]) rot_y+=0.5f;
//	//if (keys[98] ) rot_z-=0.5f; if (keys[110]) rot_z+=0.5f;
//
//}

void handleKeys(){
	if(keys[27]) 
		exit(0);
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

	////////////CAR

	if(keys['a']||keys['A'])  {
		aircraft->addRotate(0,0.8f,0);
		//aircraft->setTurn(11.0f);
		//camera->setExtraRotation(0.0f,-12.0f,0.0f);
	}
	if(keys['d']||keys['D']) {
		aircraft->addRotate(0,-0.8f,0);
		//aircraft->setTurn(-11.0f);
		//camera->setExtraRotation(0.0f,12.0f,0.0f);
	}
	if(keys['w']||keys['W']) {
		aircraft->addRotate(-0.5f,0,0);
		//aircraft->setTurn(-11.0f);
		//camera->setExtraRotation(0.0f,12.0f,0.0f);
	}
	if(keys['s']||keys['S']) {
		aircraft->addRotate(0.5f,0,0);
		//aircraft->setTurn(-11.0f);
		//camera->setExtraRotation(0.0f,12.0f,0.0f);
	}
	/*else{
		//aircraft->setTurn(0.0f);
		//camera->setExtraRotation(0.0f,0.0f,0.0f);
	}*/

	if(keys['r']||keys['R'])
		aircraft->speedUp(accel);
	if(keys['f']||keys['F'])
		aircraft->speedDown(accel);

	/////////////EXTRA VIEW

	if(keys['1']||keys['!']) {
		camera->setExtraRotation(0.0f,-90.f,0.0f);
	}
	if(keys['2']||keys['@']) { 
		camera->setExtraRotation(0.0f,180.f,0.0f);
	}
	if(keys['3']||keys['#']) {
		camera->setExtraRotation(0.0f,90.f,0.0f);
	}
	/*if(keys['4']||keys['$']) {
		if(fogDensity<1.0)
			fogDensity+=0.005f;
	}
	if(keys['5']||keys['%']) {
		if(fogDensity>0.0)
			fogDensity-=0.005f;
	}*/
}

void keyUp(unsigned char key, int x, int y)
{
	keys[key]=false;	
}

void keyDown(unsigned char key, int x, int y)
{
	keys[key]=true;

	printf("%c\n",key);
}

void specialKeyDown(int key, int x, int y)
{
	specialkeys[key]=true;
}

void specialKeyUp(int key, int x, int y)
{
	specialkeys[key]=false;
}

void mouseButton (int button,int state,int x,int y)
{
	mouse_button=-1;
	if (button==GLUT_LEFT_BUTTON)
	{
		mouse_button=button;
		if (state==GLUT_DOWN)
		{
			mouse_x=x;
			mouse_y=y;
		}
	}
}

void mouseMotion (int x,int y)
{
	if (mouse_button==GLUT_LEFT_BUTTON)
	{
		camera_fi += float(2.0/glutGet(GLUT_WINDOW_WIDTH)*(x-mouse_x));
		camera_theta -= float(2.0/glutGet(GLUT_WINDOW_HEIGHT)*(mouse_y-y));
		mouse_x=x;
		mouse_y=y;
		glutPostRedisplay ();
	}
}

void mouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		// Zoom in
		camera_distance -= 0.25;
	}
	else
	{
		// Zoom out
		camera_distance += 0.25;
	}

	return;
}

void reshapeSceen(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // !!
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h, 0.1f, 600.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // !! inaczej nie ma resize
}

void initOpenGL()
{
	glClearColor(1.0f,1.0f,1.0f,0.0f);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_LIGHTING);
	//glLightModelf(G, GL_TRUE);
	glEnable(GL_LIGHT0);
}

void drawScene()
{
	//handleKeys();
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	/* GLboolean isLighting;
	 glGetBooleanv(GL_LIGHTING,&isLighting);
	 */
	GLfloat *p = camera->getPosition();

	gluLookAt(
		p[0], p[1], p[2],
		camera->getLookAt()[0], camera->getLookAt()[1],  camera->getLookAt()[2],
		0,1,0);
	/*gluLookAt(
		camera_distance*cos(camera_theta)*cos(camera_fi),
		camera_distance*sin(camera_theta),
		camera_distance*cos(camera_theta)*sin(camera_fi),
		0.0,0.0,0.0,
		camera_distance*cos(camera_theta+0.1)*cos(camera_fi),
		camera_distance*sin(camera_theta+0.1),
		camera_distance*cos(camera_theta+0.1)*sin(camera_fi));
	*/
	//if(scale_scene)
	//{
	//	glScalef(scale_scene, scale_scene, scale_scene);
	//	if(scale_scene>5.0f || scale_scene <-2.5f)
	//		scale_scene = 0.0;
	//}
	/*float lightPos[4] = {0, 1000, 1000, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	*/
	//glPushMatrix();
	world->render();
	aircraft->render();
	//glPopMatrix();
	glDisable(GL_LIGHTING);

	glTranslatef(pos_x,pos_y,pos_z);
	glRotatef(rot_x,1,0,0);
	glRotatef(rot_y,0,1,0);
	glRotatef(rot_z,0,0,1);
	glScalef(scale_x,scale_y,scale_z);

	/* uklad wspolrzednych */
	glLineWidth(4);
	glBegin(GL_LINES);
		glColor3f(0.0,1.0,0.0);glVertex3f( 0.0, 0.0, 0.0);glVertex3f( 1.0, 0.0, 0.0);
		glColor3f(1.0,0.0,0.0);glVertex3f( 0.0, 0.0, 0.0);glVertex3f( 0.0, 1.0, 0.0);
		glColor3f(0.0,0.0,1.0);glVertex3f( 0.0, 0.0, 0.0);glVertex3f( 0.0, 0.0, 1.0);
	glEnd ();

	/* linie ukaldu przedluzajace */
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
		glColor3f(0.0,0.0,0.0);glVertex3f( 0.0, 0.0, 0.0);glVertex3f( -1.0, 0.0, 0.0);
		glColor3f(0.0,0.0,0.0);glVertex3f( 0.0, 0.0, 0.0);glVertex3f( 0.0, 0.0, -1.0);
		glColor3f(0.0,0.0,0.0);glVertex3f( 0.0, 0.0, 0.0);glVertex3f( 0.0, -1.0, 0.0);

	glEnd ();
	glDisable(GL_LINE_STIPPLE);


	glDisable(GL_DEPTH_TEST);
	/*if(isLighting) {
		glEnable(GL_LIGHTING);
	}*/
	//glFlush ();
	glutSwapBuffers ();
	glutPostRedisplay();

	/*myPos[0]+=0.0001;
	myPos[1]+=0.0005;
	myPos[2]+=0.05;
	myPos[3]+=0.2;*/
}

void sendAndRecv(int v)
{
	for(int i=0;i<3;++i)
		tmp_Me[i] = aircraft->getPosition()[i];

	for(int i=3,j=0;i<6;++i,++j)
		tmp_Me[i] = aircraft->getRotation()[j];

	Connetion::getInstance().Start();

	glutTimerFunc(50, sendAndRecv, 0);
}

void timer(int v)
{
	handleKeys();

	aircraft->doSth();
	camera->doSth();
	world->doSth();

	glutTimerFunc(10, timer, 0);

}

void initGame()
{
	aircraft = new Aircraft();
	world = new World();
	world->initLoad();

	camera = new Camera();
	camera->setFollow(aircraft);
	world->setFollow(aircraft);
}



int main(int argc, char **argv)
{	
	//std::string ip, port, name;
	//std::cout << " Podaj NAZWE: " << std::endl;
	//std::cin >> name;
	//std::cout << " Podaj IP: " << std::endl;
	//std::cin >> ip;
	//std::cout << " Podaj PORT: " << std::endl;
	//std::cin >> port;

	//Connetion::getInstance().Init(name,ip , port);

			  //  try
		//{
		//        monkey = new CModel3DS("obj/Airplane HARR/Airplane HARR.3ds");
		//}
		//catch(std::string error_str)
		//{
		//        std::cerr << "Error: " << error_str << std::endl;
		//        exit(1);
		//}

			

	Connetion::getInstance().Init("Mihal","192.168.1.105" , "1234");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH );
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Air DESTROYYYYYER");


	initOpenGL();
	InitVSync();

	SetVSyncState(1);

	glewInit();

	initGame();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshapeSceen);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);


	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);


	// aktualizowanie pozycji gracza
	glutTimerFunc(0, sendAndRecv, 0);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();

	Connetion::getInstance().Stop();

	return 0;
}