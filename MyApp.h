#ifndef _SFML_FRAMEWORK____MY_APP
#define _SFML_FRAMEWORK____MY_APP

#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#include "Controller.h"

namespace sfmlFramework
{
    class MyApp
    {
    protected:
        std::string m_windowTitle;
        int m_windowWidth, m_windowHeight, m_windowBpp;
        bool m_windowFullscreen;
        int m_fsaaLevel;

        Controller m_controller;

        void setupPerspectiveView(float viewAngle,float farClipPlane);
        void setupOrthoView(int width, int height);

    public:
        MyApp(): m_windowTitle("OpenGL"),
                m_windowWidth(800), m_windowHeight(600), m_windowBpp(32),
                m_windowFullscreen(false), m_fsaaLevel(4) {};

        MyApp(std::string title, int width, int height, int bpp, bool fullscreen, int fsaa): m_windowTitle(title),
                m_windowWidth(width), m_windowHeight(height), m_windowBpp(bpp),
                m_windowFullscreen(fullscreen), m_fsaaLevel(fsaa) {};
        virtual ~MyApp(){};

        virtual bool init()=0;
        virtual void draw()=0;
        virtual void update(float deltaTime)=0;
        virtual void resize(int x, int y)=0;

        std::string getTitle();
        int getWindowWidth();
        int getWindowHeight();
        int getWindowBpp();
        bool getWindowFullscreen();
        int getFSAA();
        virtual Controller* getController();
    };
} //namespace sfmlFramework

#endif
