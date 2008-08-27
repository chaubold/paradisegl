#include "ViewController.h"
#include <iostream>

namespace sfmlFramework
{
    ViewController::ViewController(sfmlFramework::ViewSimpleArcBall& myView)
    {
        m_view=&myView;
    }

    bool ViewController::keyPressedEvent(int keyCode)
    {
        switch(keyCode)
        {
            case sf::Key::R:
            {
                kmVec3 pos = m_view->getLookAt();
                kmVec3 delta;
                kmVec3Fill(&delta, 0.0, 3.0, 0.0);
                kmVec3Add(&pos, &pos, &delta);
                m_view->setLookAt(pos);
            }
            break;
            case sf::Key::F:
            {
                kmVec3 pos = m_view->getLookAt();
                kmVec3 delta;
                kmVec3Fill(&delta, 0.0, -3.0, 0.0);
                kmVec3Add(&pos, &pos, &delta);
                m_view->setLookAt(pos);
            }
            break;
        }
        return true;
    }

    void ViewController::mouseWheel(int delta)
    {
        m_view->setDistance(m_view->getDistance()+delta*10.0);
    }

    void ViewController::mouseMove(int x, int y)
    {
        if(m_mouseBtnStatus[BTN_LEFT])
        {
            m_view->pitch(m_mouseY-y);
            m_view->yaw(x-m_mouseX);
        }
        if(m_mouseBtnStatus[BTN_MIDDLE])
        {
            kmVec3 pos = m_view->getPosition();
            kmVec3 lookAt = m_view->getLookAt();
            //y
            kmVec3 delta;
            kmVec3Subtract(&delta, &lookAt, &pos);
            kmVec3Normalize(&delta, &delta);
            kmVec3Scale(&delta, &delta, m_mouseY - y);
            kmVec3Add(&lookAt, &lookAt, &delta);

            //x
            kmVec3 up;
            kmVec3Fill(&up, 0.0,1.0,0.0);
            kmVec3Subtract(&delta, &lookAt, &pos);
            kmVec3Cross(&delta, &delta, &up);
            kmVec3Normalize(&delta, &delta);
            kmVec3Scale(&delta, &delta, x - m_mouseX);
            kmVec3Add(&lookAt, &lookAt, &delta);

            m_view->setLookAt(lookAt);
        }
        Controller::mouseMove(x,y);
    }
}
