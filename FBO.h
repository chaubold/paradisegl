#ifndef _3DCASTE_FBO
#define _3DCASTE_FBO

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

///A FramebufferObject, also stores the Texture which is used as render target
class FBO
{
private:
    GLuint m_fbo, m_texID;
    bool m_supported;
    GLenum m_internalFormat;
    int m_width, m_height;

    GLuint m_depthBuffer;

public:
    /**
        Setup the FBO, create a texture render target with the given properties
    */
    FBO(GLenum internalFormat,GLenum textureFormat,int width, int height);

    ///Destructor
    ~FBO();

    ///method to initialize the FBO, called from constructor
    void initialize(GLenum internalFormat,GLenum textureFormat,int width, int height);


    ///is FBO supported by this card?
    inline static bool isSupported()
    {
        return glewIsSupported("GL_EXT_framebuffer_object");
    }

    /**
     * Disable use of FBO, it restores the viewport and sets the active framebuffer to the OpenGL context.
     * If you want to switch to another Framebuffer afterwards, just do the glPopAttribs call yourself and then bind the next one
     * this is faster than first disabling FBO and then binding a new one
     */
    inline static void disable()
    {
        if (isSupported())
        {
            glPopAttrib();
            glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
        }
    }

    ///Return the internal format of the FBO and its texture, GL_DEPTH_COMPONENT or GL_RGBA usually
    GLenum getInternalFormat();

    ///bind the FBO so it is the new render target
    void bindFBO();

    ///after having rendered to the FBO, we can use the texture directly with this method (binds to current texture unit)
    void useTextureImage(bool mipmap=false);
};


#endif
