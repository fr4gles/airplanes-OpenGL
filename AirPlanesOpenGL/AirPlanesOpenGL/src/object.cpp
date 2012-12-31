#include "object.h"
#include "material.h"

extern std::map<std::string, GLuint> textures;
extern std::map<std::string, Material*> materials;
Object* Object::load(const char* pFilename) {

	using namespace std; 

	

	fstream file;
	file.open(pFilename, std::fstream::in);

	if(!file.is_open() ) { 

		cout << "File \"" << pFilename << "\" not found!\n";
		system("pause");
	}

	Object* result = new Object();
	Object* actual = result;

	char lineFromFile[512];
	std::string actualMaterial = ""; // Aktualny material dla Face'a


	while (file.getline(lineFromFile,sizeof(lineFromFile))) {
      
	   string line(lineFromFile);
       
	   vector<std::string>split = System::splitString(lineFromFile);

	   // MTL 
	   if(line.find("mtllib ") == 0) { 

		  // Material::load(split[1].c_str());
	   }
	   else  
	   // Vector
	   if(line.find("v ") == 0) { 

		   Vector3f vec(0,0,0);
		   vec.x = atof(split[1].c_str());
		   vec.y = atof(split[2].c_str());
		   vec.z = atof(split[3].c_str());

		  

		   actual->v.push_back(vec);

	   }
	   else  
	   // Normals
	   if(line.find("vn ") == 0) {
		    
		   Vector3f vec(0,0,0);
		   vec.x = atof(split[1].c_str());
		   vec.y = atof(split[2].c_str());
		   vec.z = atof(split[3].c_str());
		   
		 
		   actual->vn.push_back(vec);
	   }
	   else  
	   // Texture cords
	   if(line.find("vt ") == 0) {

		   Vector3f vec(0,0,0);
		   vec.x = atof(split[1].c_str());
		   vec.y = atof(split[2].c_str());
		   vec.z = atof(split[3].c_str());
		   

		   actual->vt.push_back(vec);
	   }
	   else  
	   // Faces
	   if(line.find("f ") == 0) {

		   ObjectFace objFace;
		   objFace.materialName = actualMaterial;
		   for(GLint i = 1; i < split.size(); i++) { 

			   vector<std::string> splitTokens = System::splitString(split[i], "/");
			   ObjectFaceInfo info;
			   info.v =  atoi(splitTokens[0].c_str());
			  
			   info.vt =  atoi(splitTokens[1].c_str());	
			   info.vn =  atoi(splitTokens[2].c_str());

			   objFace.info.push_back(info);
			  
		   }

		 
		   actual->f.push_back(objFace);
	   }
	   else  
	   // Material
	   if(line.find("usemtl ") == 0) {

		   actualMaterial = split[1];
	   }

    }

	file.close();

	return result;
}

void Object::render(GLfloat pAlpha) {
	 
	

	GLint numFaces = f.size();
	for(GLint i = 0 ; i < numFaces; i++) {

			GLint numVec = f[i].info.size();
		
			glBindTexture(GL_TEXTURE_2D, textures[f[i].materialName]);
			glBegin(GL_TRIANGLES);
			

			Material* mat = materials[f[i].materialName];
			ObjectFace * face = &f[i];

			for(GLint k = 0; k < 3 ; k++) {

				   ObjectFaceInfo * info = &face->info[k];
					GLint vID = info->v - 1;
				    GLint nID = info->vn - 1;
					GLint tID = info->vt - 1;

					

					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat->diffuse_vector);
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat->ambient_vector);
					GLfloat spec[4] = {1,1,1,1};
					GLfloat em[4] = {0,0,0,1};
					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
					glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, em);
					glColor4f(mat->diffuse.x,mat->diffuse.y,mat->diffuse.z, pAlpha);
				
					glTexCoord2f(vt[tID].x, -vt[tID].y);
					glNormal3f(vn[nID].x, vn[nID].y, vn[nID].z);
					glVertex3f(v[vID].x, v[vID].y, v[vID].z);


					
			}
			
			glEnd();
		

	
	}

}