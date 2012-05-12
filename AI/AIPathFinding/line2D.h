#ifndef LINE2D_
#define LINE2D_

#include <cmath>

#include <Ogre.h>

class Line2D {
    public:
        
        enum PointClassification
        {

            ON_LINE,
            
            LEFT_SIDE,

            RIGHT_SIDE
        };

        enum LineClassification
        {

            COLLINEAR,
            
            LINES_INTERSECT,

            SEGMENTS_INTERSECT,

            A_BISECTS_B,

            B_BISECTS_A,

            PARALELL
        };

        Line2D();

        Line2D(const Line2D& line2D);

        Line2D(const Ogre::Vector2& pointA, const Ogre::Vector2& pointB);

        ~Line2D();

        Line2D& operator=(const Line2D& line2D);

        void setBeginPoint(const Ogre::Vector2& point);

        void setEndPoint(const Ogre::Vector2& point);

        void setPoints(const Ogre::Vector2& pointA, const Ogre::Vector2& pointB);

        void setPoints(Ogre::Real pointAx, Ogre::Real pointAy, Ogre::Real pointBx, Ogre::Real pointBy);

        Ogre::Real signedDistance(const Ogre::Vector2 point) const;

        PointClassification classifyPoint(const Ogre::Vector2 point, float epsilon = 0.0f) const;

        LineClassification getIntersection(const Line2D& line, Ogre::Vector2* intersectionPoint = 0) const;

        void getDirection(Ogre::Vector2& direction) const;

        const Ogre::Vector2& getBeginPoint() const;

        const Ogre::Vector2& getEndPoint() const;

        const Ogre::Vector2& getNormal() const;

        Ogre::Real getLength() const;


    private:
        
        Ogre::Vector2 _beginPoint;
        Ogre::Vector2 _endPoint;
        
        mutable Ogre::Vector2 _normal;
        
        mutable bool _normalCalculated;
        
        void computeNormal() const;
};


#endif
