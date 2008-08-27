#ifndef _VBO
#define _VBO

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <kazmath/vec2.h>
#include <kazmath/vec3.h>

using namespace std;

class VBO{
        public:
            VBO();
            ~VBO();
            void AddTriangle(const kmVec3* v1, const kmVec2* t1,const kmVec3* v2, const kmVec2* t2,const kmVec3* v3 ,const kmVec2* t3);
            void AddTriangle(float x1, float y1, float z1, float s1, float t1,float x2, float y2, float z2, float s2, float t2,float x3, float y3, float z3, float s3, float t3);
            void AddTriangle(const kmVec3* v1,const kmVec3* n1, const kmVec2* t1,const kmVec3* v2, const kmVec3* n2, const kmVec2* t2,const kmVec3* v3 ,const kmVec3* n3, const kmVec2* t3);
            void Upload();
            void Draw(int start, int end, bool withTexture);
            void Draw(bool withTexture);

            bool supported;
        private:
            static GLuint ms_isBound;
            GLuint m_VBO_vert;
            GLuint m_VBO_tex;
            GLuint m_VBO_norm;

            bool m_useNormals;

            std::vector<kmVec3> mc_vertices;
            std::vector<kmVec2> mc_texcoords;
            std::vector<kmVec3> mc_normals;
            int m_vertCount;
};



#endif
