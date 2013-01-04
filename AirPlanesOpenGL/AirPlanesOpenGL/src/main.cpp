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
#include "Bullet.h"

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

const GLfloat accel = 0.005f;

// zmienne klas
World* world;
Camera *camera;
Aircraft *aircraft;
std::vector<Bullet*> bullets;

std::vector<std::pair<std::string,RootObject*>> przeciwnicy;

typedef std::vector<std::pair<std::string,std::vector<double>>> Player;
Player players;

bool exitProgram = false;
bool strzelaj = false;
int iloscKul = 100;

// usuwanie elementu z wektora
template <typename T>
void remove(std::vector<T>& vec, size_t pos)
{
	std::vector<T>::iterator it = vec.begin();
	std::advance(it, pos);
	vec.erase(it);
}

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

	if(keys[32])
	{
		licznikStrzal++;

		nrWystrzelonejKuli = licznikStrzal;
		strzelaj = true;

		if(licznikStrzal > iloscKul-2)
			licznikStrzal = -1;
	}

	////////////AIRCRAFT

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

	//if(keys[32])
	//	boost::this_thread::sleep(boost::posix_time::milliseconds(100));

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

	for(int i=0;i<bullets.size();++i)
		bullets[i]->render();

	aircraft->render();


	//dodawanie przeciwników
	if(przeciwnicy.size() != 0 )
	{
		for(int i=0;i<przeciwnicy.size();++i)
		{
			przeciwnicy[i].second->render();
		}
	}

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


	//glDisable(GL_DEPTH_TEST);
	/*if(isLighting) {
		glEnable(GL_LIGHTING);
	}*/
	//glFlush ();
	glutSwapBuffers ();
	glutPostRedisplay();
}

void push_backToPrzeciwnicy(std::string tmp1, RootObject* tmp2)
{
	przeciwnicy.push_back(std::make_pair(tmp1, tmp2));

	for(int i=0;i<iloscKul;++i)
		bullets.push_back(new Bullet());
}

void aktualizujPozycjeGracza()
{
	if(exitProgram)
	{
		for(int i=0;i<3;++i)
			tmp_Me[i] = aircraft->getPosition()[i]+5000.0f;

		for(int i=3,j=0;i<6;++i,++j)
			tmp_Me[i] = aircraft->getRotation()[j]+5000.0f;

		for(int i=0;i<10;++i)
			Connection::getInstance().Start();
		
		Connection::getInstance().Stop();
		glutLeaveMainLoop();
	}
	else
	{
		for(int i=0;i<3;++i)
			tmp_Me[i] = aircraft->getPosition()[i];

		for(int i=3,j=0;i<6;++i,++j)
			tmp_Me[i] = aircraft->getRotation()[j];
	}


	Connection::getInstance().Start();

	// dodawanie przeciwników
	if(players.size() != 0 && przeciwnicy.size() == 0)
		push_backToPrzeciwnicy(players[0].first,new Aircraft(players[0].second)); //przeciwnicy.push_back(std::make_pair(players[0].first, new Aircraft(players[0].second)));
	else if (players.size() != 0 /*&& przeciwnicy.size() != 0*/)
	{
		bool isNot = true;
		for(int i=0;i<players.size();++i)
		{
			for(int j=0;j<przeciwnicy.size();++j)
			{			
				if(przeciwnicy[j].first == players[i].first)
					isNot = false;
			}
		}

		if(isNot)
			push_backToPrzeciwnicy(players[players.size()-1].first,new Aircraft(players[players.size()-1].second));
			//przeciwnicy.push_back(std::make_pair(players[players.size()-1].first, new Aircraft(players[players.size()-1].second)));


		for(int i=0;i<players.size();++i)
		{
			for(int j=0;j<przeciwnicy.size();++j)
			{
				if(players[i].first == przeciwnicy[j].first)
				{
					przeciwnicy[j] = std::make_pair(players[i].first, new Aircraft(players[i].second));
					
					int tmp = /*static_cast<int>*/(players[i].second[players[i].second.size()-1]);
					if(tmp > -1)
					{
						bullets[(iloscKul-1)*(j+1)+tmp-1]->setPosition(przeciwnicy[j].second->getPosition());
						bullets[(iloscKul-1)*(j+1)+tmp-1]->addRotate(przeciwnicy[j].second->getRotation());
					}
				}
			}
		}
	}
}


void sendAndRecv(int v)
{
	aktualizujPozycjeGracza();
	glutTimerFunc(50, sendAndRecv, 0);
}

void bulletTime(int v)
{
	if(licznikStrzal > -1 && strzelaj == true)
	{
		bullets[licznikStrzal]->setPosition(aircraft->getPosition());
		bullets[licznikStrzal]->addRotate(aircraft->getRotation());

		strzelaj = false;

		//aktualizujPozycjeGracza();
	}

	glutTimerFunc(50, bulletTime, 0);
}


void timer(int v)
{
	handleKeys();

	aircraft->doSth();
	camera->doSth();
	world->doSth();

	for(int i=0;i<bullets.size();++i)
		bullets[i]->doSth();

	glutTimerFunc(10, timer, 0);

}

void initGame()
{
	for(int i=0;i<(iloscKul);++i)
		bullets.push_back(new Bullet());

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

	//Connection::getInstance().Init(name,ip , port);

			  //  try
		//{
		//        monkey = new CModel3DS("obj/Airplane HARR/Airplane HARR.3ds");
		//}
		//catch(std::string error_str)
		//{
		//        std::cerr << "Error: " << error_str << std::endl;
		//        exit(1);
		//}

			

	Connection::getInstance().Init("Mi","192.168.1.105" , "1234");

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
	glutTimerFunc(0, bulletTime, 0);

	glutMainLoop();

	Connection::getInstance().Stop();

	return 0;
}