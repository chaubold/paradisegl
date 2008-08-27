#ifndef _SFML_FRAMEWORK____SFML_APP
#define _SFML_FRAMEWORK____SFML_APP

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <sstream>

#include "MyApp.h"

namespace sfmlFramework
{
    class SfmlApp
    {
    private:
        sf::RenderWindow *m_renderWindow;

        bool processEvents(MyApp& _app);
    public:
        SfmlApp();
        ~SfmlApp();

        bool init(MyApp& _app);
        void run(MyApp& _app);
    };
} //namespace sfmlFramework

#endif
