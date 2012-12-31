#pragma once

#include <string>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\GL.h>

#include "freeglut/freeglut.h"
#include "lib3ds/file.h"
#include "lib3ds/mesh.h"
#include "lib3ds/material.h"
//#include "lib3ds\

// Our 3DS model class
class CModel3DS
{
        public:
                CModel3DS(std:: string filename);
                //virtual void Draw() const;
                //virtual void CreateVBO();
                //virtual ~CModel3DS();
        protected:
                void GetFaces();
                unsigned int m_TotalFaces;
				Lib3dsFile * m_model;
                GLuint m_VertexVBO, m_NormalVBO;
};

// Load 3DS model
CModel3DS::CModel3DS(std:: string filename)
{
        m_TotalFaces = 0;
       

        m_model = lib3ds_file_load(filename.c_str());
		// If loading the model failed, we throw an exception
        if(!m_model)
        {
                throw strcat("Unable to load ", filename.c_str());
        }
}