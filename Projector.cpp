#include "Projector.h"

Projector::Projector()
{
    m_position=CVector();
    m_lookAt=CVector();
    m_upVec=CVector();
    m_farClipPlane=1000.0;
}

Projector::Projector(const CVector& pos,const CVector& lookat,const CVector& up,float farClipPlane)
{
    m_position=pos;
    m_lookAt=lookat;
    m_upVec=up;
    m_farClipPlane=farClipPlane;
}

void Projector::setPosition(const CVector& pos)
{
    m_position=pos;
}

void Projector::setPosition(const CVector& pos,const CVector& lookat,const CVector& up)
{
    m_position=pos;
    m_lookAt=lookat;
    m_upVec=up;
}

void Projector::setFarClipPlane(float distance)
{
    m_farClipPlane=distance;
}

void Projector::setupTexGen()
{
    mat4 m_result;
    glTexGenfv(GL_S, GL_EYE_PLANE, (GLfloat*)&m_result.mat[0]);
    glTexGenfv(GL_T, GL_EYE_PLANE, (GLfloat*)&m_result.mat[4]);
    glTexGenfv(GL_R, GL_EYE_PLANE, (GLfloat*)&m_result.mat[8]);
    glTexGenfv(GL_Q, GL_EYE_PLANE, (GLfloat*)&m_result.mat[12]);

    glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_R,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}


void Projector::setupProjector()
{
    setupTexGen();

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.5,0.5,0.0);
    glScalef(0.5,0.5,1.0);

    gluPerspective(60, 1, 1, m_farClipPlane);

    gluLookAt(m_position.x,m_position.y,m_position.z,   //Pos
              m_lookAt.x,m_lookAt.y,m_lookAt.z,    //LookAt
              m_upVec.x,m_upVec.y,m_upVec.z     //Up
             );

    glMatrixMode(GL_MODELVIEW);

    CVector normal=(-1)*m_lookAt;
    double d=normal.x*m_position.x+normal.y*m_position.y+normal.z*m_position.z;
    double eqn[]={normal.x,normal.y,normal.z,-d};
    glClipPlane(GL_CLIP_PLANE0,&eqn[0]);
    glEnable(GL_CLIP_PLANE0);
}

void Projector::reset()
{
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    glDisable(GL_CLIP_PLANE0);
}

void Projector::setupProjectorsView()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(60, 1, 1, m_farClipPlane);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    gluLookAt(m_position.x,m_position.y,m_position.z,   //Pos
              m_lookAt.x,m_lookAt.y,m_lookAt.z,    //LookAt
              m_upVec.x,m_upVec.y,m_upVec.z     //Up
             );
}

void Projector::resetProjectorsView()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
