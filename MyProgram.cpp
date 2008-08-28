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
    m_refractionFbo = NULL;
    m_depthFbo = NULL;
    m_time=0;
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
	m_refractionFbo = new FBO(GL_RGBA, GL_RGBA, 1024, 1024, true);
	//m_depthFbo = new FBO(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, 1024, 1024);

	m_waterHeight = 20.0;
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


	//position model
	kmMat4 projection;
	kmMat4PerspectiveProjection(&projection, 45.0f, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 1.0f, 5000.0f);
	//glLoadMatrixf(&projection.m_Mat[0]);


    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

// *******************************************
//Draw scene to FBO
// d*******************************************
    m_refractionFbo->bindFBO();

	kmMat4 lookAt = m_view.getLookAtMatrix();

    kmMat4 scale;
    kmMat4Scaling(&scale, 5.0, 7.0, 5.0);

	kmMat4 terrain_trans;
	kmMat4Assign(&terrain_trans, &lookAt);
	kmMat4Multiply(&terrain_trans, &terrain_trans, &scale);

    m_renderShader->BindShader();
	m_renderShader->sendUniform("mat_modelview",terrain_trans);
	m_renderShader->sendUniform("mat_projection",projection);

    m_terrain->DrawModel(true, m_renderShader);

    kmMat4Scaling(&scale, 500.0, 200.0, 500.0);
    kmMat4 sky_trans;
    kmMat4Multiply(&sky_trans, &terrain_trans, &scale);
    kmMat4 rotation;
    kmMat4RotationY(&rotation, kmDegreesToRadians(105.0));
    kmMat4Multiply(&sky_trans, &sky_trans, &rotation);

	m_renderShader->sendUniform("mat_modelview",sky_trans);
    m_skydome->DrawModel(true, m_renderShader);
    FBO::disable();
// *******************************************
//Draw reflection to FBO
// *******************************************
    m_reflectionFbo->bindFBO();
    glFrontFace(GL_CW);

    kmMat4 flip;
    kmMat4 flip_trans;
    kmMat4Scaling(&flip, 1.0, -1.0, 1.0);

    kmMat4 translation;
    kmMat4Translation(&translation, 0.0, -2*m_waterHeight, 0.0);


    kmMat4Multiply(&flip_trans, &terrain_trans, &flip);
    kmMat4Multiply(&flip_trans, &flip_trans, &translation);

    m_reflectionShader->BindShader();
    m_reflectionShader->sendUniform("mat_projection", projection);
    m_reflectionShader->sendUniform("water_height", m_waterHeight-0.05f);

    m_reflectionShader->sendUniform("mat_modelview", flip_trans);

    m_terrain->DrawModel(true, m_reflectionShader);


    kmMat4Multiply(&flip_trans, &flip_trans, &scale);
    kmMat4Multiply(&flip_trans, &flip_trans, &rotation);

    //no clipping for the sky
    m_renderShader->BindShader();
	m_renderShader->sendUniform("mat_projection",projection);
    m_renderShader->sendUniform("mat_modelview", flip_trans);
    m_skydome->DrawModel(true, m_reflectionShader);

    FBO::disable();
    glFrontFace(GL_CCW);


// *******************************************
// Draw scene
// *******************************************
    m_renderShader->BindShader();
    m_renderShader->sendUniform("mat_projection",projection);
	m_renderShader->sendUniform("mat_modelview",terrain_trans);
    m_terrain->DrawModel(true, m_renderShader);

	m_renderShader->sendUniform("mat_modelview",sky_trans);
    m_skydome->DrawModel(true, m_renderShader);

// *******************************************
// Draw water using DUDV map
// *******************************************
    glDisable(GL_CULL_FACE);
    m_waterShader->BindShader();
    m_waterShader->sendUniform("mat_projection", projection);
    kmMat4 water_trans;
    kmMat4Translation(&water_trans, 0.0f, m_waterHeight, 0.0f);
    kmMat4Multiply(&water_trans, &terrain_trans, &water_trans);
    m_waterShader->sendUniform("mat_modelview", water_trans);

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_water->getMaterial("dudvmap")->texID);
    m_waterShader->sendUniform("dudvmap", 1);

    glActiveTexture(GL_TEXTURE2);
    m_reflectionFbo->useTextureImage();
    m_waterShader->sendUniform("water_reflection",2);

    glActiveTexture(GL_TEXTURE3);
    m_refractionFbo->useTextureImage();
    m_waterShader->sendUniform("water_refraction",3);

    glActiveTexture(GL_TEXTURE4);
    m_refractionFbo->useDepthTexture();
    m_waterShader->sendUniform("water_depth",4);

    m_waterShader->sendUniform("lightpos",20.0f,100.0f,200.0f,1.0f);
    kmVec3 pos = m_view.getPosition();
    m_waterShader->sendUniform("viewpos",pos.x, pos.y, pos.z, 1.0f);
    m_waterShader->sendUniform("waterColor", 0.3f,0.8f,0.8f,0.0f);

    m_waterShader->sendUniform("time",m_time);
    m_waterShader->sendUniform("time2",-m_time);

    m_water->DrawModel(true, m_waterShader);

    Shader::UnbindShader();
    checkForErrors();
    glColor3f(1.0,1.0,1.0);
    glActiveTexture(GL_TEXTURE0);
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
	m_time += deltaTime*0.07;
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
    if(m_refractionFbo)
        delete m_refractionFbo;
    if(m_depthFbo)
        delete m_depthFbo;
}

sfmlFramework::ViewController* MyProgram::getController()
{
	return &m_controller;
}
