#define _CRT_SECURE_NO_WARNINGS
//
//#include <cstdlib>
//#include <iostream>
//#include <string>

#include "chat_client.hpp"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\GL.h>

#include "freeglut/freeglut.h"

#include "targa.h"
#include "obj.h"

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

bool loaded = false; // 


bool start_MP = false; // czy gra MP wystartowa³a?

void handleKeys()
{	
	if (keys['1']) figura=1;
	if (keys['2']) figura=2;
	if (keys['3']) figura=3;
	if (keys['4']) figura=4;
	if (keys['5']) figura=5;
	if (keys['6']) figura=6;	
	if (keys['7']) figura=7;
	
	if (keys['9']) scale_scene+=0.02f;
	if (keys['0']) scale_scene-=0.02f;

	if(keys[27]) exit(0);

	if (specialkeys[100]) camera_fi-=0.01f;        //lewo 
	if (specialkeys[102]) camera_fi+=0.01f;        //prawo
	if (specialkeys[101]) camera_theta+=0.01f;     //gora
	if (specialkeys[103]) camera_theta-=0.01f;     //dol

	if (keys[113]) pos_x-=0.01f; if (keys[119]) pos_x+=0.01f;
	if (keys[97] ) pos_y-=0.01f; if (keys[115]) pos_y+=0.01f;
	if (keys[122]) pos_z-=0.01f; if (keys[120]) pos_z+=0.01f;

	if (keys[101]) scale_x-=0.02f; if (keys[114]) scale_x+=0.02f;
	if (keys[100]) scale_y-=0.02f; if (keys[102]) scale_y+=0.02f;
	if (keys[99] ) scale_z-=0.02f; if (keys[118]) scale_z+=0.02f;

	if (keys[116]) rot_x-=0.5f; if (keys[121]) rot_x+=0.5f;
	if (keys[103]) rot_y-=0.5f; if (keys[104]) rot_y+=0.5f;
	if (keys[98] ) rot_z-=0.5f; if (keys[110]) rot_z+=0.5f;

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
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	gluPerspective( 35.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void initOpenGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_NORMALIZE|GL_LIGHTING);
	//glEnable(GL_LIGHT0);

}

void drawScene()
{
	handleKeys();
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glLoadIdentity();
	//gluLookAt(3.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	gluLookAt(camera_distance*cos(camera_theta)*cos(camera_fi),
		camera_distance*sin(camera_theta),
		camera_distance*cos(camera_theta)*sin(camera_fi),
		0.0,0.0,0.0,
		camera_distance*cos(camera_theta+0.1)*cos(camera_fi),
		camera_distance*sin(camera_theta+0.1),
		camera_distance*cos(camera_theta+0.1)*sin(camera_fi));

	if(scale_scene)
	{
		glScalef(scale_scene, scale_scene, scale_scene);
		if(scale_scene>5.0f || scale_scene <-2.5f)
			scale_scene = 0.0;
	}

	glTranslatef(pos_x,pos_y,pos_z);
	glRotatef(rot_x,1,0,0);
	glRotatef(rot_y,0,1,0);
	glRotatef(rot_z,0,0,1);
	glScalef(scale_x,scale_y,scale_z);

	float i=0.0, j=0.0;

	/* uklad wspolrzednych */
	glLineWidth(4);
	glBegin(GL_LINES);
		glColor3f(0.0,1.0,0.0);
		glVertex3f( 0.0, 0.0, 0.0);
		glVertex3f( 1.0, 0.0, 0.0);
		glColor3f(1.0,0.0,0.0);
		glVertex3f( 0.0, 0.0, 0.0);
		glVertex3f( 0.0, 1.0, 0.0);
		glColor3f(0.0,0.0,1.0);
		glVertex3f( 0.0, 0.0, 0.0);
		glVertex3f( 0.0, 0.0, 1.0);
	glEnd ();

	/* linie ukaldu przedluzajace */
	glLineStipple(1, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,0.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( -1.0, 0.0, 0.0);
	glColor3f(0.0,0.0,0.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0, -1.0);
	glColor3f(0.0,0.0,0.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, -1.0, 0.0);

	glEnd ();
	glDisable(GL_LINE_STIPPLE);
	
	if(figura==1)
	{
		//glEnable(GL_LIGHTING);
		/* szescian */
		glColor3f(0.0,0.0,0.0);
		glPushMatrix();
		glutWireCube(0.5);
		glPopMatrix();

		/* trojkat */
		glPushMatrix();
		glBegin(GL_TRIANGLES);
			glColor3f(1.0,0.0,0.0); glVertex3f(-0.25f, 0.25f, 0.25f);
			glColor3f(0.0,1.0,0.0); glVertex3f(0.25f, -0.25f, 0.25f);
			glColor3f(0.0,0.0,1.0); glVertex3f(0.25f, 0.25f, -0.25f);
		
		glEnd();
		glPopMatrix();
	}

	if(figura==2)
	{
		//glDisable(GL_LIGHTING);
		/* obwazanek */
		glPushMatrix();
		glColor3f(0.3,0.5,0.2);
		glutSolidTorus(0.1f, 0.5f,100,50);  
		glPopMatrix();
		//glEnable(GL_LIGHTING);
	}

	if(figura==3)
	{
		glEnable(GL_LIGHTING);
		for(i=-7;i<7;i++)
		{
			for(j=-7;j<7;j++)
			{
				glRectf(i*0.1,j*0.1,i*0.1+0.1,j*0.1+0.1);
				j+=0.1f;
			}
			i+=0.1f;
		}
	}


	glFlush ();
	glutSwapBuffers ();
	glutPostRedisplay();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	myPos[0]+=0.0001;
	myPos[1]+=0.0005;
	myPos[2]+=0.05;
	myPos[3]+=0.2;

	//if(myPos[3] == 0.2)
	//	start_MP = true;

	//if(start_MP)
	//{
		Connetion::getInstance().Start();
	//	start_MP = false;
	//}
}

int main(int argc, char **argv)
{	
	std::string ip, port, name;
	std::cout << " Podaj NAZWE: " << std::endl;
	std::cin >> name;
	//std::cout << " Podaj IP: " << std::endl;
	//std::cin >> ip;
	//std::cout << " Podaj PORT: " << std::endl;
	//std::cin >> port;

	Connetion::getInstance().Init(name);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH );
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OpenGL first");

	initOpenGL();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(reshapeSceen);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);

	glutMainLoop();

	Connetion::getInstance().Stop();

	return 0;
}