#include "MyApp.h"

namespace sfmlFramework
{
    std::string MyApp::getTitle()
    {
        return m_windowTitle;
    }

    int MyApp::getWindowWidth()
    {
        return m_windowWidth;
    }

    int MyApp::getWindowHeight()
    {
        return m_windowHeight;
    }

    int MyApp::getWindowBpp()
    {
        return m_windowBpp;
    }

    int MyApp::getFSAA()
    {
        return m_fsaaLevel;
    }

    bool MyApp::getWindowFullscreen()
    {
        return m_windowFullscreen;
    }

    Controller* MyApp::getController()
    {
        return &m_controller;
    }

    void MyApp::setupPerspectiveView(float viewAngle,float farClipPlane)
    {
        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glLoadIdentity();							// Reset The Projection Matrix

        gluPerspective( viewAngle, (GLdouble)m_windowWidth / m_windowHeight, 0.1, farClipPlane );

        glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
        glLoadIdentity();							// Reset The Modelview Matrix
    }

    void MyApp::setupOrthoView(int width, int height)
    {
        glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
        glLoadIdentity();							// Reset The Projection Matrix

        glOrtho(0.0f,width,height,0.0f,-1.0f,1.0f);

        glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
        glLoadIdentity();							// Reset The Modelview Matrix
    }

} //namespace sfmlFramework
