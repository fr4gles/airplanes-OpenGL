#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "system.h"
#include "my_types.h"



/* Opisuje renderowane face'y w obiekcie */
struct ObjectFaceInfo { 

	ObjectFaceInfo() : v(), vn(0), vt(0){ }
	ObjectFaceInfo(GLint pV, GLint pVn, GLint pVt) : v(pV), vn(pVn), vt(pVt) {}
	GLint v;
	GLint vn;
	GLint vt;


};

struct ObjectFace	{ 
	
	std::vector<ObjectFaceInfo> info;
	std::string materialName;
};

/* Klasa obiektu 3D.
- Zawiera funkcje do ladowania obiektu
- Renderowania obiektu
*/
class Object { 

public:

	Object() : alpha(1.0f) {} 

	static Object* load(const char * pFilename);
	static Object* loadCoins(const char* pFilename);
	void setObject(Object* pObj) { object = pObj; } ;

	void render(GLfloat pAlpha = 1.f);
	void setAlpha(GLfloat a) { alpha = a; }
	std::vector<Vector3f> v; // Vectors
	std::vector<Vector3f> vn; // Normals
	std::vector<Vector3f> vt; // Texture cords
	std::vector<ObjectFace> f; // Faces

	GLfloat alpha;

	Object* object;
};
