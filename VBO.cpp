#include "VBO.h"

using namespace std;

GLuint VBO::ms_isBound=0;

VBO::VBO()
{
	supported = glewIsSupported("GL_ARB_vertex_buffer_object");

	if(supported)
	{
		std::cout<<"VBO: supported!"<<std::endl;
	}
	else
	{
	    std::cout<<"VBO: not supported!" << std::endl;
	}

	m_vertCount=0;
	m_useNormals=false;
}



void VBO::AddTriangle(const kmVec3* v1, const kmVec2* t1,const kmVec3* v2, const kmVec2* t2,const kmVec3* v3 ,const kmVec2* t3)
{
	//AddTriangle(v1.x,v1.y,v1.z,t1.s,t1.t,v2.x,v2.y,v2.z,t2.s,t2.t,v3.x,v3.y,v3.z,t3.s,t3.t);
	mc_vertices.push_back(*v1);
	mc_texcoords.push_back(*t1);

	mc_vertices.push_back(*v2);
	mc_texcoords.push_back(*t2);

	mc_vertices.push_back(*v3);
	mc_texcoords.push_back(*t3);
	m_vertCount+=3;
}

void VBO::AddTriangle(const kmVec3* v1,const kmVec3* n1, const kmVec2* t1,const kmVec3* v2, const kmVec3* n2, const kmVec2* t2,const kmVec3* v3 ,const kmVec3* n3, const kmVec2* t3)
{
	mc_vertices.push_back(*v1);
	mc_texcoords.push_back(*t1);
	mc_normals.push_back(*n1);

	mc_vertices.push_back(*v2);
	mc_texcoords.push_back(*t2);
	mc_normals.push_back(*n2);

	mc_vertices.push_back(*v3);
	mc_texcoords.push_back(*t3);
	mc_normals.push_back(*n3);

	m_vertCount+=3;
}

void VBO::AddTriangle(float x1, float y1, float z1, float s1, float t1,float x2, float y2, float z2, float s2, float t2,float x3, float y3, float z3, float s3, float t3)
{
    kmVec3 v1, v2, v3;
    kmVec2 tc1, tc2, tc3;

    kmVec3Fill(&v1, x1,y1,z1);
    kmVec3Fill(&v2, x2,y2,z2);
    kmVec3Fill(&v3, x3,y3,z3);

    kmVec2Fill(&tc1, s1,t1);
    kmVec2Fill(&tc2, s2,t2);
    kmVec2Fill(&tc3, s3,t3);

	AddTriangle(&v1, &tc1, &v2, &tc2, &v3, &tc3);
}

void VBO::Upload()
{
	if(mc_normals.size()==mc_vertices.size())
	{
		m_useNormals=true;
	}

	if(supported)
	{
		std::cout<<"VBO: sending arrays to graphics card"<<std::endl;
		std::cout<<"\tVertices"<<std::endl;
		std::cout<<"\tTexCoords"<<std::endl;
		glGenBuffers(1,&m_VBO_vert);
		glBindBuffer(GL_ARRAY_BUFFER,m_VBO_vert);
		glBufferData(GL_ARRAY_BUFFER,m_vertCount*3*sizeof(float),&mc_vertices[0],GL_STATIC_DRAW);

		glGenBuffers(1,&m_VBO_tex);
		glBindBuffer(GL_ARRAY_BUFFER,m_VBO_tex);
		glBufferData(GL_ARRAY_BUFFER,m_vertCount*2*sizeof(float),&mc_texcoords[0],GL_STATIC_DRAW);

		if(m_useNormals)
		{
			std::cout<<"\tNormals"<<std::endl;
			glGenBuffers(1,&m_VBO_norm);
			glBindBuffer(GL_ARRAY_BUFFER,m_VBO_norm);
			glBufferData(GL_ARRAY_BUFFER,m_vertCount*3*sizeof(float),&mc_normals[0],GL_STATIC_DRAW);
		}
		ms_isBound = m_VBO_vert;
	}
	std::cout << "Done Uploading to VBO: " << m_VBO_vert << std::endl;
}

void VBO::Draw(int start, int end, bool withTexture)
{

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if(m_useNormals)
		glEnableClientState(GL_NORMAL_ARRAY);

	//if(ms_isBound!=m_VBO_vert)//if not already bound.. do so
	//{
	//	ms_isBound=m_VBO_vert;
		if(supported)
		{
			glBindBuffer(GL_ARRAY_BUFFER,m_VBO_vert);
			glVertexPointer(3,GL_FLOAT,0,(char *)NULL);

			glBindBuffer(GL_ARRAY_BUFFER,m_VBO_tex);
			glTexCoordPointer(2,GL_FLOAT,0,(char *)NULL);

			if(m_useNormals)
			{

				glBindBuffer(GL_ARRAY_BUFFER,m_VBO_norm);
				glNormalPointer(GL_FLOAT,0,(char *)NULL);
			}
		}
		else
		{
			glVertexPointer(3,GL_FLOAT,0,&mc_vertices[0]);
			glTexCoordPointer(2,GL_FLOAT,0,&mc_texcoords[0]);
			if(m_useNormals)
				glNormalPointer(GL_FLOAT,0,&mc_normals[0]);
		}
	//}
	//the real drawing process
	if(start > m_vertCount)
        start = m_vertCount;
	if(end > m_vertCount)
        end = m_vertCount;

	glDrawArrays(GL_TRIANGLES,start,end-start);

	switch(glGetError())
	{
	case GL_NO_ERROR: break;
	case GL_INVALID_VALUE: std::cout<<"VBO: Index out of bounds exception!"<<std::endl;break;
	//default: std::cout<<"VBO: error occured!"<<std::endl;break;
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if(m_useNormals)
		glDisableClientState(GL_NORMAL_ARRAY);

}

void VBO::Draw(bool withTexture)
{
    Draw(0, m_vertCount, withTexture);
}


VBO::~VBO()
{
	std::cout<<"VBO: Cleaning up"<<std::endl;
	glDeleteBuffers(1,&m_VBO_vert);
	glDeleteBuffers(1,&m_VBO_tex);
	if(m_useNormals) glDeleteBuffers(1,&m_VBO_norm);
}

