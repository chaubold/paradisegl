////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#define _DEBUG

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <sstream>

#include "MyProgram.h"
#include "SfmlApp.h"


int main()
{
    MyProgram myApp;
    sfmlFramework::SfmlApp sfmlApp;

    if(!sfmlApp.init(myApp))
        return EXIT_FAILURE;

    sfmlApp.run(myApp);

    return EXIT_SUCCESS;
}
