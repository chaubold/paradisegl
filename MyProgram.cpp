#include "MyProgram.h"

MyProgram::MyProgram() : MyApp("OpenGL 3.0 Island", 800, 600, 32, false, 4)
{
    m_terrain = NULL;
    m_skydome = NULL;
    m_water = NULL;
    m_renderShader = NULL;
    m_reflectionShader = NULL;
    m_waterShader = NULL;
    m_reflectionFbo = NULL;
    //m_refractionFbo = NULL;
    //m_depthFbo = NULL;
}

bool MyProgram::init()
{
    kmVec3 position;
    kmVec3Fill(&position, 0.0,250.0,0.0);
    m_view=sfmlFramework::ViewSimpleArcBall( 700.0f, position );
    m_view.pitch(15.0);

    m_controller=sfmlFramework::ViewController(m_view);

	//OpenGL Setup
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Load other stuff like models/shaders/images
	m_terrain = new ObjModel("data/terrain.obj");
	m_skydome = new ObjModel("data/skydome.obj");
	m_water = new ObjModel("data/water_plane.obj");
	if(m_water->getMaterial("dudvmap") == NULL)
	{
	    std::cout << "Water model has no DUDV map" << std::endl;
	    return false;
	}

	m_renderShader = new Shader("data/shader/default.vert", "data/shader/default.frag");
	m_reflectionShader = new Shader("data/shader/reflection.vert", "data/shader/reflection.frag");
	m_waterShader = new Shader("data/shader/water.vert", "data/shader/water.frag");

	m_reflectionFbo = new FBO(GL_RGBA, GL_RGBA, 1024, 1024);

	m_waterHeight = 10.0;
	return true;
}

void MyProgram::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	//setup perspective matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    m_renderShader->BindShader();
	//position model
	kmMat4 projection;
	kmMat4PerspectiveProjection(&projection, 45.0f, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 1.0f, 5000.0f);
	m_renderShader->sendUniform("mat_projection",projection);
	//glLoadMatrixf(&projection.m_Mat[0]);


    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//*******************************************
//Draw scene
//*******************************************

	kmMat4 lookAt = m_view.getLookAtMatrix();

    kmMat4 scale;
    kmMat4Scaling(&scale, 3.0, 7.0, 3.0);

	kmMat4 terrain_trans;
	kmMat4Assign(&terrain_trans, &lookAt);
	kmMat4Multiply(&terrain_trans, &terrain_trans, &scale);

	m_renderShader->sendUniform("mat_modelview",terrain_trans);

    m_terrain->DrawModel(true, m_renderShader);

    kmMat4Scaling(&scale, 700.0, 300.0, 700.0);
    kmMat4 sky_trans;
    kmMat4Multiply(&sky_trans, &terrain_trans, &scale);
    kmMat4 rotation;
    kmMat4RotationY(&rotation, kmDegreesToRadians(105.0));
    kmMat4Multiply(&sky_trans, &sky_trans, &rotation);
	//glLoadMatrixf(&result.m_Mat[0]);
	m_renderShader->sendUniform("mat_modelview",sky_trans);
    m_skydome->DrawModel(true, m_renderShader);

//*******************************************
//Draw reflection to FBO
//*******************************************
    m_reflectionFbo->bindFBO();
    glViewport(0,0,1024, 1024);
    glFrontFace(GL_CW);
    kmMat4 flip;
    kmMat4Scaling(&flip, 1.0, -1.0, 1.0);
    kmMat4 translation;
    kmMat4Translation(&translation, 0.0, -m_waterHeight, 0.0);

    kmMat4 flip_trans;
    kmMat4Multiply(&flip_trans, &terrain_trans, &flip);
    kmMat4Multiply(&flip_trans, &flip_trans, &translation);

    m_reflectionShader->BindShader();
    m_reflectionShader->sendUniform("mat_projection", projection);
    m_reflectionShader->sendUniform("mat_modelview", flip_trans);
    m_reflectionShader->sendUniform("water_height", m_waterHeight);

    m_terrain->DrawModel(true, m_reflectionShader);

    kmMat4Multiply(&flip_trans, &sky_trans, &flip);
    kmMat4Multiply(&flip_trans, &flip_trans, &translation);

    m_reflectionShader->sendUniform("mat_modelview", flip_trans);
    m_skydome->DrawModel(true, m_reflectionShader);
    FBO::disable();
    glFrontFace(GL_CCW);
//*******************************************
//Draw water using DUDV map
//*******************************************
    glViewport(0, 0, m_windowWidth, m_windowHeight);

    m_waterShader->BindShader();
    m_waterShader->sendUniform("mat_projection", projection);
    m_waterShader->sendUniform("mat_modelview", terrain_trans);

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_water->getMaterial("dudvmap")->texID);
    m_waterShader->sendUniform("dudvmap", 1);

    m_water->DrawModel(true, m_waterShader);

    Shader::UnbindShader();
    checkForErrors();
}

void MyProgram::checkForErrors()
{
    int err=glGetError();
    while(err != GL_NO_ERROR){
        std::cout<<"glGetError: "<<err<<std::endl;
        err=glGetError();
    }
}

void MyProgram::update(float deltaTime)
{
	m_rotY += deltaTime;
	if (m_rotY > 36.0) m_rotY = 0.0;
}

void MyProgram::resize(int x, int y)
{
	m_windowHeight = y;
	m_windowWidth = x;
	std::cout << "Resizing window to " << x << "x" << y << std::endl;

	glViewport(0, 0, x, y);// Reset The Current Viewport
}

MyProgram::~MyProgram()
{
    if(m_terrain)
        delete m_terrain;
    if(m_skydome)
        delete m_skydome;
    if(m_water)
        delete m_water;
    if(m_renderShader)
        delete m_renderShader;
    if(m_reflectionShader)
        delete m_reflectionShader;
    if(m_waterShader)
        delete m_waterShader;
    if(m_reflectionFbo)
        delete m_reflectionFbo;
}

sfmlFramework::ViewController* MyProgram::getController()
{
	return &m_controller;
}
