#define _CRT_SECURE_NO_WARNINGS

#include "chat_client.hpp"

#include "glew.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\GL.h>

#include "freeglut/freeglut.h"

#include "World.h"
#include "Camera.h"

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

// zmienne klas
World* world;
Camera *camera;

void handleKeys()
{	
	//if (keys['1']) figura=1;
	//if (keys['2']) figura=2;
	//if (keys['3']) figura=3;
	//if (keys['4']) figura=4;
	//if (keys['5']) figura=5;
	//if (keys['6']) figura=6;	
	//if (keys['7']) figura=7;
	
	if (keys['9']) scale_scene+=0.02f;
	if (keys['0']) scale_scene-=0.02f;

	if(keys[27]) exit(0);

	if (keys['a']) camera_fi-=0.01f;				// lewo
	if (keys['d']) camera_fi+=0.01f;		// prawo
	if (keys['s']) camera_theta+=0.01f;		// gora
	if (keys['w']) camera_theta-=0.01f;		// dol


	if (specialkeys[100]) camera_fi-=0.01f;        //lewo 
	if (specialkeys[102]) camera_fi+=0.01f;        //prawo
	if (specialkeys[101]) camera_theta+=0.01f;     //gora
	if (specialkeys[103]) camera_theta-=0.01f;     //dol

	//if (keys[113]) pos_x-=0.01f; if (keys[119]) pos_x+=0.01f;
	//if (keys[97] ) pos_y-=0.01f; if (keys[115]) pos_y+=0.01f;
	//if (keys[122]) pos_z-=0.01f; if (keys[120]) pos_z+=0.01f;

	//if (keys[101]) scale_x-=0.02f; if (keys[114]) scale_x+=0.02f;
	//if (keys[100]) scale_y-=0.02f; if (keys[102]) scale_y+=0.02f;
	//if (keys[99] ) scale_z-=0.02f; if (keys[118]) scale_z+=0.02f;

	//if (keys[116]) rot_x-=0.5f; if (keys[121]) rot_x+=0.5f;
	//if (keys[103]) rot_y-=0.5f; if (keys[104]) rot_y+=0.5f;
	//if (keys[98] ) rot_z-=0.5f; if (keys[110]) rot_z+=0.5f;

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
	glEnable(GL_LIGHT0);
}

void drawScene()
{
	handleKeys();
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	gluLookAt(
		camera_distance*cos(camera_theta)*cos(camera_fi),
		camera_distance*sin(camera_theta),
		camera_distance*cos(camera_theta)*sin(camera_fi),
		0.0,0.0,0.0,
		camera_distance*cos(camera_theta+0.1)*cos(camera_fi),
		camera_distance*sin(camera_theta+0.1),
		camera_distance*cos(camera_theta+0.1)*sin(camera_fi));

	//if(scale_scene)
	//{
	//	glScalef(scale_scene, scale_scene, scale_scene);
	//	if(scale_scene>5.0f || scale_scene <-2.5f)
	//		scale_scene = 0.0;
	//}

	glEnable(GL_LIGHTING);
	glPushMatrix();
		world->render();
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


	glFlush ();
	glutSwapBuffers ();
	glutPostRedisplay();

	myPos[0]+=0.0001;
	myPos[1]+=0.0005;
	myPos[2]+=0.05;
	myPos[3]+=0.2;
}

void sendAndRecv(int v)
{
	Connetion::getInstance().Start();

	glutTimerFunc(50, sendAndRecv, 0);
}

void initGame()
{
	world = new World();
	world->initLoad();

	camera = new Camera();
	camera->setFollow(world);
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

	glutMainLoop();

	Connetion::getInstance().Stop();

	return 0;
}