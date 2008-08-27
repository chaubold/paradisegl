#include "SfmlApp.h"

namespace sfmlFramework
{
    SfmlApp::SfmlApp(){}

    SfmlApp::~SfmlApp(){}

    bool SfmlApp::init(MyApp& _app)
    {
        sf::VideoMode m = sf::VideoMode( _app.getWindowWidth(),_app.getWindowHeight(), _app.getWindowBpp() );
        unsigned long style = sf:: Style::Fullscreen;
        if (!_app.getWindowFullscreen())
            style=sf::Style::Resize|sf::Style::Close;

        sf::WindowSettings Settings;
        Settings.DepthBits         = 24; // Request a 24 bits depth buffer
        Settings.StencilBits       = 8;  // Request a 8 bits stencil buffer
        Settings.AntialiasingLevel = _app.getFSAA();  // Request 2 levels of antialiasing


        m_renderWindow=new sf::RenderWindow(m, _app.getTitle(), style, Settings);

        if(glewInit() != GLEW_OK)
            return false;

        //Resize the app so perspective gets set up
        _app.resize( _app.getWindowWidth(), _app.getWindowHeight() );

        m_renderWindow->PreserveOpenGLStates(true);

        //loading..
        sf::Image loading;
        loading.LoadFromFile("data/loading.jpg");
        sf::Sprite load_sprite(loading);

        m_renderWindow->Draw(load_sprite);
        m_renderWindow->Display();

        if (!_app.init())
            return false;

        return true;
    }

    void SfmlApp::run(MyApp& _app)
    {
        // Create a clock for measuring time elapsed
        sf::Clock Clock;

        float time=0.0;

        bool running = true;
        while (running)
        {
            running=processEvents(_app);
            // Render the App
            _app.draw();
            _app.update(Clock.GetElapsedTime()-time);


            // Draw some text on top of our OpenGL object
            std::stringstream str;
            str<<"FPS: "<<1/(Clock.GetElapsedTime()-time);
            time=Clock.GetElapsedTime();
            sf::String Text(str.str());
            Text.SetPosition(10.0f, 10.0f);
            Text.SetColor(sf::Color(255, 255, 255));
            m_renderWindow->Draw(Text);

            // Finally, display rendered frame on screen
            m_renderWindow->Display();
        }

    }

    bool SfmlApp::processEvents(MyApp& _app)
    {
        // Process events
        sf::Event Event;

        while (m_renderWindow->GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                return false;

            if (Event.Type == sf::Event::KeyPressed)
            {
                // Escape key : exit
                if (Event.Key.Code == sf::Key::Escape)
                    return false;
                else
                    if(!_app.getController()->keyPressedEvent(Event.Key.Code))
                        return false;
            }

            if (Event.Type == sf::Event::Resized)
            {
                _app.resize(Event.Size.Width,Event.Size.Height);
            }

            if (Event.Type == sf::Event::MouseWheelMoved)
                _app.getController()->mouseWheel(Event.MouseWheel.Delta);

            if(Event.Type == sf::Event::MouseMoved)
                _app.getController()->mouseMove(Event.MouseMove.X, Event.MouseMove.Y);


            if(Event.Type == sf::Event::MouseButtonPressed)
            {
                int btn=-1;
                switch(Event.MouseButton.Button)
                {
                    case sf::Mouse::Left: btn=0; break;
                    case sf::Mouse::Right: btn=1; break;
                    case sf::Mouse::Middle: btn=2; break;
                    default: break;
                }
                _app.getController()->mouseDownEvent(btn);
            }

            if(Event.Type == sf::Event::MouseButtonReleased)
            {
                int btn=-1;
                switch(Event.MouseButton.Button)
                {
                    case sf::Mouse::Left: btn=0; break;
                    case sf::Mouse::Right: btn=1; break;
                    case sf::Mouse::Middle: btn=2; break;
                    default: break;
                }
                _app.getController()->mouseUpEvent(btn);
            }

        }
        return true;
    }

}//namespace sfmlFramework
