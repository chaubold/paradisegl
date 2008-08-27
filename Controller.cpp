#include "Controller.h"

namespace sfmlFramework
{
    Controller::Controller()
    {
        m_mouseX=0;
        m_mouseY=0;
        m_mouseBtnStatus[BTN_LEFT]=false;
        m_mouseBtnStatus[BTN_RIGHT]=false;
        m_mouseBtnStatus[BTN_MIDDLE]=false;
    }

    void Controller::mouseDownEvent(int btn)
    {
        if(0<= btn && btn<3)
            m_mouseBtnStatus[btn]=true;
    }

    void Controller::mouseUpEvent(int btn)
    {
        if(0<= btn && btn<3)
            m_mouseBtnStatus[btn]=false;
    }

    void Controller::mouseMove(int x, int y)
    {
        m_mouseX=x;
        m_mouseY=y;
    }

}
