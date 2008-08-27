#ifndef _VIEW_CONTROLLER
#define _VIEW_CONTROLLER

#include <SFML/Graphics.hpp>

#include "Controller.h"
#include "ViewSimpleArcBall.h"

namespace sfmlFramework
{
    class ViewController: public sfmlFramework::Controller
    {
        private:
            sfmlFramework::ViewSimpleArcBall* m_view;

        public:
            ViewController(){};
            ViewController(sfmlFramework::ViewSimpleArcBall& myView);
            virtual ~ViewController(){}

            virtual bool keyPressedEvent(int keyCode);
            virtual void mouseWheel(int delta);

            virtual void mouseMove(int x, int y);
    };
}

#endif
