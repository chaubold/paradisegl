#ifndef _3DCASTE_PROJECTOR
#define _3DCASTE_PROJECTOR

#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../util/Vector.h"

/**
    This class represents a projector and handles everything to set up projective Texturing
*/
class Projector{
    private:
        /**Params for gluLookAt when setting up the projector*/
        CVector m_position, m_lookAt, m_upVec;
        /**farClippingPlane*/
        float m_farClipPlane;

        /**
             Enable Eye-linear texture coordinate generation
             WRAP_S and WRAP_T are set to GL_CLAMP, you need to manually reset these after calling reset()
        */
        void setupTexGen();

    public:
        /**Constructor setting everything to default values (pos & lookat= vec3(0.0,0.0,0.0), farClipPlane=1000)*/
        Projector();

        /**
            Constructor to set up the projector
        */
        Projector(const CVector& pos,const CVector& lookat,const CVector& up,float farClipPlane);

        /**set the Projector to a new position, let it look somewhere else,...*/
        void setPosition(const CVector& pos,const CVector& lookat,const CVector& up);

        /**just change the position of the projector*/
        void setPosition(const CVector& pos);

        /**set the farClipPlane to a new Value*/
        void setFarClipPlane(float distance);

        /**
            sets up a the projector, anything which will be drawn now is affected by the projected texture
            ATTENTION: GL_CLIP_PLANE_0 is used here and will be disabled after calling reset! Texture matrix is modified
                WRAP_S and WRAP_T are set to GL_CLAMP, you need to manually reset these after calling reset()
        */
        void setupProjector();

        /**
            sets up the Perspective and ModelviewMatrix in the same way as the projector does.
            So now we're seeing what the projector sees
            ATTENTION: you need to pop both matrices from the stack again afterwards!
        */
        void setupProjectorsView();

        void resetProjectorsView();

        /**reset TextureCoordinateGeneration and the Texture matrix*/
        void reset();

};

#endif //_3DCASTE_PROJECTOR
