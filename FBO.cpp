#include "FBO.h"
#include <iostream>

FBO::FBO(GLenum internalFormat,GLenum textureFormat,int width, int height)
{
    if(FBO::isSupported())
	{
	    m_supported=true;
	    initialize(internalFormat,textureFormat,width,height);
	}
	else m_supported=false;
}

void FBO::initialize(GLenum internalFormat,GLenum textureFormat,int width, int height)
{
    glGenFramebuffersEXT(1,&m_fbo);

	    glGenTextures (1, &m_texID);
        glBindTexture (GL_TEXTURE_2D, m_texID);

        glTexImage2D (GL_TEXTURE_2D, 0, textureFormat, width,height, 0, internalFormat, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        m_internalFormat=internalFormat;

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
        switch(m_internalFormat)
	    {
	        case GL_DEPTH_COMPONENT:
	        case GL_LUMINANCE:
	        case GL_INTENSITY:
                std::cout<<"FBO: Creating an fbo which returns a depth texture"<<std::endl;
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_texID, 0);

                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
	        break;

	        default:
                glGenRenderbuffersEXT(1, &m_depthBuffer);
                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthBuffer);
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width,height);
                glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texID, 0 );
                glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depthBuffer);
	        break;
	    }
	    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        m_width=width;
        m_height=height;
}


FBO::~FBO()
{
	if(m_supported)
	{
	    glDeleteFramebuffersEXT(1, &m_fbo);
	    glDeleteTextures(1, &m_texID);
	    if(m_internalFormat!=GL_DEPTH_COMPONENT)
            glDeleteRenderbuffersEXT(1, &m_depthBuffer);
	}
}

GLenum FBO::getInternalFormat()
{
	return m_internalFormat;
}

void FBO::bindFBO()
{
	if(m_supported)
	{
	    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	    glPushAttrib(GL_VIEWPORT_BIT);
        glViewport( 0, 0, m_width, m_height);

        switch(m_internalFormat)
	    {
	        case GL_DEPTH_COMPONENT:
	        case GL_LUMINANCE:
	        case GL_INTENSITY:
                glClear( GL_DEPTH_BUFFER_BIT );
	        break;

	        default:
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	        break;
	    }


        if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)==GL_FRAMEBUFFER_UNSUPPORTED_EXT)
        {
            std::cout<<"FBO: Wrong combination of buffers.."<<std::endl;
        }
        else if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
        {
            std::cout<<"FBO: Error when setting up Framebuffer!"<<std::endl;
        }
	}
}

void FBO::useTextureImage(bool mipmap)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    if(mipmap)
        glGenerateMipmapEXT(GL_TEXTURE_2D);
}

