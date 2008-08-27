#include "Shader.h"

//GLenum Shader::ms_isBound=0;

Shader::Shader(){}

Shader::Shader(std::string VertexShader,std::string FragmentShader)
{
        m_compilationError=false;

	    //TODO: Check extension
	    if(glewIsSupported("GL_ARB_shader_objects"))
	    {
	        std::cout << "Shader supported!!" << std::endl;
        }
        else
        {
            std::cout << "Shader not supported!!" << std::endl;
            return;
        }

		std::cout<<"MOShader: Loading shaders: "<<VertexShader<<" and "<<FragmentShader<<std::endl;

		m_shader=glCreateProgramObjectARB();
		vert=glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		frag=glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		std::string vertexSource;
		std::ifstream istr(VertexShader.c_str());
		char c;
		while(istr.get(c))
			vertexSource+=c;
		istr.close();

		const char *tmp1=strdup(vertexSource.c_str());

		glShaderSourceARB(vert, 1, &tmp1, NULL);

		std::string fragmentSource;
		istr.open(FragmentShader.c_str());
		while(istr.get(c))
			fragmentSource+=c;
		istr.close();

		const char *tmp2=strdup(fragmentSource.c_str());

		glShaderSourceARB(frag, 1, &tmp2, NULL);

		glCompileShaderARB(vert);
		GLint vsResult;
		glGetObjectParameterivARB(vert, GL_OBJECT_COMPILE_STATUS_ARB, &vsResult);
        char infoLog[2000];
        int infoLen;
		if(!vsResult)
		{
			std::cout<<"GLSL Shader: Vertex Shader contains errors, please validate this shader!"<<std::endl;

            glGetInfoLogARB(vert, sizeof(infoLog), &infoLen, infoLog);
            std::cout<<infoLog<<std::endl;
		}

        glCompileShaderARB(frag);
		glGetObjectParameterivARB(frag, GL_OBJECT_COMPILE_STATUS_ARB, &vsResult);

		if(!vsResult)
		{
			std::cout<<"GLSL Shader: Fragment Shader contains errors, please validate this shader!"<<std::endl;
			glGetInfoLogARB(frag, sizeof(infoLog), &infoLen, infoLog);
            std::cout<<infoLog<<std::endl;
		}

		// Attach The Shader Objects To The Program Object
		glAttachObjectARB(m_shader, vert);
		glAttachObjectARB(m_shader, frag);

		if(!m_compilationError) std::cout<<"MOShader: Shaders loaded sccessfully"<<std::endl;
		// Link The Program Object
		glLinkProgramARB(m_shader);
}

Shader::~Shader()
{
	glDetachObjectARB(m_shader,vert);
	glDetachObjectARB(m_shader,frag);
	glDeleteObjectARB(m_shader);
	glDeleteObjectARB(frag);
	glDeleteObjectARB(vert);
	std::cout<<"MOShader: Shader deleted"<<std::endl;
}

void Shader::BindShader()
{
	//if(m_shader!=ms_isBound)
	//{
		glUseProgramObjectARB(m_shader);
	//	ms_isBound=m_shader;
	//}
}

GLenum Shader::getShader()
{
	return m_shader;
}

int Shader::getUL(const std::string& uniformName)
{
	return glGetUniformLocation(m_shader,uniformName.c_str());
}

int Shader::getAL(const std::string& attributeName)
{
	return glGetAttribLocation(m_shader,attributeName.c_str());
}

void Shader::sendUniform(const std::string& uniformName,float x)
{
	glUniform1f(getUL(uniformName),x);
}

void Shader::sendUniform(const std::string& uniformName,float x,float y)
{
	glUniform2f(getUL(uniformName),x,y);
}

void Shader::sendUniform(const std::string& uniformName,float x,float y, float z)
{
	glUniform3f(getUL(uniformName),x,y,z);
}

void Shader::sendUniform(const std::string& uniformName,float x,float y, float z, float w)
{
	glUniform4f(getUL(uniformName),x,y,z,w);
}

void Shader::sendUniform(const std::string& uniformName,int x)
{
	glUniform1i(getUL(uniformName),x);
}

void Shader::sendUniform(const std::string& uniformName,int x,int y)
{
	glUniform2i(getUL(uniformName),x,y);
}

void Shader::sendUniform(const std::string& uniformName,int x,int y, int z)
{
	glUniform3i(getUL(uniformName),x,y,z);
}

void Shader::sendUniform(const std::string& uniformName,int x,int y, int z, int w)
{
	glUniform4i(getUL(uniformName),x,y,z,w);
}

void Shader::sendUniform(const std::string& uniformName,const kmVec3& v)
{
	glUniform3f(getUL(uniformName),v.x,v.y,v.z);
}

void Shader::sendUniform(const std::string& uniformName,const kmMat4& mat)
{
    glUniformMatrix4fv(getUL(uniformName),1, false, &mat.m_Mat[0]);
}

