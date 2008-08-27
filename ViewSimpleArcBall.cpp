#include "ViewSimpleArcBall.h"
#include <iostream>

namespace sfmlFramework
{
    ViewSimpleArcBall::ViewSimpleArcBall()
    {
        m_distance=0.0;
        m_pitch=0.0;
        m_yaw=0.0;
    }

    ViewSimpleArcBall::ViewSimpleArcBall(const float& distance, const kmVec3& lookAt)
    {
        m_distance=distance;
        kmVec3Assign(&m_lookAt, &lookAt);
        m_yaw=0.0;
    }

    ViewSimpleArcBall::~ViewSimpleArcBall(){}

    void ViewSimpleArcBall::setDistance(const float& dist)
    {
        //std::cout<<"View: distance "<<dist<<std::endl;
        m_distance=dist;
    }

    void ViewSimpleArcBall::setLookAt(const kmVec3& lookAt)
    {
        m_lookAt=lookAt;
    }

    void ViewSimpleArcBall::pitch(const float& angle)
    {
        m_pitch+=angle;
        if(m_pitch>90.0f) m_pitch=90.0f;
        if(m_pitch<-90.0f) m_pitch=-90.0f;
    }

    void ViewSimpleArcBall::yaw(const float& angle)
    {
        m_yaw+=angle;
    }

    void ViewSimpleArcBall::reset()
    {
        m_pitch=0.0;
        m_yaw=0.0;
    }

    float ViewSimpleArcBall::getDistance()
    {
        return m_distance;
    }

    kmVec3 ViewSimpleArcBall::getPosition()
    {
        kmVec3 vec;
        float deg2rad=M_PI/180;
        kmVec3Fill( &vec, sin(deg2rad*m_yaw)*cos(deg2rad*m_pitch),
                                     sin(deg2rad*m_pitch),
                                    cos(deg2rad*m_yaw)*cos(deg2rad*m_pitch));
        kmVec3Scale( &vec, &vec, m_distance);

        return vec;
    }

    kmVec3 ViewSimpleArcBall::getLookAt()
    {
        return m_lookAt;
    }


void output_matrix(const kmMat4* matrix)
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << " | ";
		for (int j = 0; j < 4; j++)
		{
			std::cout << std::setw(8) << matrix->m_Mat[i + 4 * j] << " ";
		}
		std::cout << " | " << std::endl;
	}
}

    kmMat4 ViewSimpleArcBall::getLookAtMatrix()
    {
        kmVec3 pos = getPosition();
        kmMat4 lookAt;

        kmVec3 eye;
        kmVec3Add(&eye, &pos, &m_lookAt);

        kmVec3 up;
        kmVec3Fill(&up, 0.0,1.0,0.0);

        kmMat4LookAt( &lookAt, &eye, &m_lookAt, &up);

        return lookAt;
    }
}
