#ifndef _SFML_FRAMEWORK____CONTROLLER
#define _SFML_FRAMEWORK____CONTROLLER
#include <iostream>

namespace sfmlFramework{
    class Controller{
        protected:
            int m_mouseX, m_mouseY;
            bool m_mouseBtnStatus[3];

        public:
            enum Btns
            {
                BTN_LEFT=0,
                BTN_RIGHT,
                BTN_MIDDLE
            };

            Controller();
            virtual ~Controller(){}

            virtual bool keyPressedEvent(int keyCode){ return true;};
            virtual void mouseWheel(int delta){};

            virtual void mouseDownEvent(int btn);
            virtual void mouseUpEvent(int btn);
            virtual void mouseMove(int x, int y);
    };
}
#endif
