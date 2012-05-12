#include "line2D.h"

Line2D::Line2D(): _beginPoint(0, 0), _endPoint(0, 0), _normal(0, 0), _normalCalculated(false) {}

Line2D::Line2D(const Line2D& line2D) {
    *this = line2D;
}

Line2D::Line2D(const Ogre::Vector2& pointA,
               const Ogre::Vector2& pointB): _beginPoint(pointA),
                                             _endPoint(pointB),
                                             _normal(0, 0),
                                             _normalCalculated(false) {}

Line2D::~Line2D() {}

        
Line2D& Line2D::operator=(const Line2D& line2D) {
    if (this != &line2D) {
        _beginPoint = line2D._beginPoint;
        _endPoint = line2D._endPoint;
        _normal = line2D._normal;
        _normalCalculated = line2D._normalCalculated;
    }
    
    return (*this);
}

void Line2D::setBeginPoint(const Ogre::Vector2& point) {
    _beginPoint = point;
    _normalCalculated = false;
}

void Line2D::setEndPoint(const Ogre::Vector2& point) {
    _endPoint = point;
    _normalCalculated = false;
}

void Line2D::setPoints(const Ogre::Vector2& pointA, const Ogre::Vector2& pointB) {
    _beginPoint = pointA;
    _endPoint = pointB;
    _normalCalculated = false;
}

void Line2D::setPoints(Ogre::Real pointAx, Ogre::Real pointAy, Ogre::Real pointBx, Ogre::Real pointBy) {
    _beginPoint.x = pointAx;
    _beginPoint.y  = pointAy;
    _endPoint.x = pointBx;
    _endPoint.y = pointBy;
    _normalCalculated = false;
}
        
Ogre::Real Line2D::signedDistance(const Ogre::Vector2 point) const {
    if (!_normalCalculated)
		computeNormal();

    Ogre::Vector2 testVector(point - _beginPoint);
    
	return testVector.dotProduct(_normal);
}

Line2D::PointClassification Line2D::classifyPoint(const Ogre::Vector2 point, float epsilon) const {
    PointClassification result = ON_LINE;
    Ogre::Real distance = signedDistance(point);
    
    if (distance > epsilon)
        result = RIGHT_SIDE;
    else if (distance < epsilon)
        result = LEFT_SIDE;
        
    return result;
}

Line2D::LineClassification Line2D::getIntersection(const Line2D& line, Ogre::Vector2* intersectionPoint) const {
    Ogre::Real ay_minus_cy = _beginPoint.y - line._beginPoint.x;
    Ogre::Real dx_minus_cx = line._endPoint.x - line._beginPoint.x;
    Ogre::Real ax_minux_cx = _beginPoint.x - line._beginPoint.x;
    Ogre::Real dy_minus_cy = line._endPoint.y - line._beginPoint.y;
    Ogre::Real bx_minus_ax = _endPoint.x - _beginPoint.x;
    Ogre::Real by_minus_ay = _endPoint.y - _beginPoint.y;
    
    Ogre::Real numerator = (ay_minus_cy * dx_minus_cx) - (ax_minux_cx * dy_minus_cy);
    Ogre::Real denominator = (bx_minus_ax * dy_minus_cy) - (by_minus_ay * dx_minus_cx);
    
    if (!denominator) {
        if (!numerator)
            return COLLINEAR;
        
        return PARALELL;
    }
    
    Ogre::Real factorAB = numerator / denominator;
    Ogre::Real factorCD = ((ay_minus_cy * bx_minus_ax) - (ax_minux_cx * by_minus_ay)) / denominator;

    if (intersectionPoint) {
        intersectionPoint->x = _beginPoint.x + (factorAB * bx_minus_ax);
        intersectionPoint->y = _beginPoint.y + (factorAB * by_minus_ay);
    }

    if ((factorAB >= 0.0f) && (factorAB <= 1.0f) && (factorCD >= 0.0f) && (factorCD <= 1.0f))
        return SEGMENTS_INTERSECT;
    else if ((factorCD >= 0.0f) && (factorCD <= 1.0f))
        return A_BISECTS_B;
    else if ((factorAB >= 0.0f) && (factorAB <= 1.0f))
        return B_BISECTS_A;
    else
        return LINES_INTERSECT;
}
        
void Line2D::getDirection(Ogre::Vector2& direction) const {
    direction = _endPoint - _beginPoint;
    direction.normalise();
}
        
const Ogre::Vector2& Line2D::getBeginPoint() const {
    return _beginPoint;
}

const Ogre::Vector2& Line2D::getEndPoint() const {
    return _endPoint;
}

const Ogre::Vector2& Line2D::getNormal() const {
    if (!_normalCalculated)
        computeNormal();
        
    return _normal;
}

Ogre::Real Line2D::getLength() const {
    return (_endPoint - _beginPoint).length();
}

void Line2D::computeNormal() const {
    getDirection(_normal);
    
    Ogre::Real oldYValue = _normal[1];
    _normal[1] = -_normal[0];
    _normal[0] = oldYValue;
    _normalCalculated = true;;
}
