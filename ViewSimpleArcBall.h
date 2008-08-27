#ifndef _SFML_FRAMEWORK____VIEW_SIMPLE_ARC_BALL
#define _SFML_FRAMEWORK____VIEW_SIMPLE_ARC_BALL

#include <GL/glu.h>
#include <kazmath/vec3.h>
#include <kazmath/mat4.h>
#include <math.h>
#include <iomanip>

namespace sfmlFramework
{
    class ViewSimpleArcBall
    {
        private:
            float m_distance,m_yaw,m_pitch;
            kmVec3 m_lookAt;

        public:
            ViewSimpleArcBall();
            ViewSimpleArcBall(const float& distance, const kmVec3& lookAt);
            ~ViewSimpleArcBall();

            void setDistance(const float& dist);
            void setLookAt(const kmVec3& lookAt);

            void pitch(const float& angle);
            void yaw(const float& angle);

            void reset();

            kmVec3 getLookAt();
            kmVec3 getPosition();
            float getDistance();

            kmMat4 getLookAtMatrix();
    };
}

#endif
