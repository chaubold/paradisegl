#ifndef _MO_SHADER
#define _MO_SHADER

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <kazmath/vec3.h>
#include <kazmath/mat4.h>
#include <GL/glew.h>

class Shader{
	private:
		GLenum m_shader;
		//static GLenum ms_isBound;
		GLenum vert,frag;
		bool m_compilationError;
		Shader();
	public:
		Shader(std::string VertexShader,std::string FragmentShader);
		~Shader();
		void BindShader();
		static void UnbindShader()
		{
			glUseProgramObjectARB(0);
			//ms_isBound=0;
		}


		GLenum getShader();

		int getUL(const std::string& uniformName);
		int getAL(const std::string& attributeName);

		void sendUniform(const std::string& uniformName,float x);
		void sendUniform(const std::string& uniformName,float x,float y);
		void sendUniform(const std::string& uniformName,float x,float y, float z);
		void sendUniform(const std::string& uniformName,float x,float y, float z, float w);
		void sendUniform(const std::string& uniformName,int x);
		void sendUniform(const std::string& uniformName,int x,int y);
		void sendUniform(const std::string& uniformName,int x,int y, int z);
		void sendUniform(const std::string& uniformName,int x,int y, int z, int w);
		void sendUniform(const std::string& uniformName,const kmVec3& v);
		void sendUniform(const std::string& uniformName,const kmMat4& mat);
};


#endif
