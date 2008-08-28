#ifndef _My_PROGRAM
#define _My_PROGRAM

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <iomanip>

#include <kazmath/mat4.h>

#include "MyApp.h"
#include "ViewSimpleArcBall.h"
#include "ViewController.h"
#include "ObjModel.h"
#include "Shader.h"
#include "FBO.h"

class MyProgram : public sfmlFramework::MyApp
{
	private:
        void checkForErrors();


		sfmlFramework::ViewSimpleArcBall m_view;
		sfmlFramework::ViewController m_controller;

		///rotation of the model
		float m_rotY;
		float m_waterHeight;
		float m_time;

		///Model to load and display
		ObjModel* m_terrain;
		ObjModel* m_skydome;
		ObjModel* m_water;

        Shader* m_renderShader;
        Shader* m_reflectionShader;
        Shader* m_waterShader;

        FBO* m_reflectionFbo;
        FBO* m_refractionFbo;
        FBO* m_depthFbo;
	public:
		MyProgram();
		~MyProgram();

		virtual bool init();
		virtual void draw();
		virtual void update(float deltaTime);
		virtual void resize(int x, int y);
		virtual sfmlFramework::ViewController* getController();
};

#endif
