#include "AIMovable.h"

OpenSteer::AIMovable::~AIMovable(void)
{

}

OpenSteer::Vec3 OpenSteer::AIMovable::adjustRawSteeringForce (const Vec3& force,
                                                  const float /* deltaTime */)
{
    
    const float maxAdjustedSpeed = 0.2f * maxSpeed ();

    if ((speed () > maxAdjustedSpeed) || (force == Vec3::zero))
    {
        return force;
    }
    else
    {
        const float range = speed() / maxAdjustedSpeed;
        // const float cosine = interpolate (pow (range, 6), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 10), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 100), 1.0f, -1.0f);
        // const float cosine = interpolate (pow (range, 50), 1.0f, -1.0f);
        const float cosine = interpolate (pow (range, 20), 1.0f, -1.0f);
        return limitMaxDeviationAngle (force, cosine, forward());
    }
    
}

void OpenSteer::AIMovable::applyBrakingForce (const float rate, const float deltaTime)
{
    
    const float rawBraking = speed () * rate;
    const float clipBraking = ((rawBraking < maxForce ()) ?
                               rawBraking :
                               maxForce ());

    setSpeed (speed () - (clipBraking * deltaTime));
    
}


// ----------------------------------------------------------------------------
// apply a given steering force to our momentum,
// adjusting our orientation to maintain velocity-alignment.


void OpenSteer::AIMovable::applySteeringForce (const Vec3& force,
                                              const float elapsedTime)
{
    
    const Vec3 adjustedForce = adjustRawSteeringForce (force, elapsedTime);

    // enforce limit on magnitude of steering force
    const Vec3 clippedForce = adjustedForce.truncateLength (maxForce ());

    // compute acceleration and velocity
    Vec3 newAcceleration = (clippedForce / mass());
    Vec3 newVelocity = velocity();

    // damp out abrupt changes and oscillations in steering acceleration
    // (rate is proportional to time step, then clipped into useful range)
    if (elapsedTime > 0)
    {
        const float smoothRate = clip (9 * elapsedTime, 0.15f, 0.4f);
        blendIntoAccumulator (smoothRate,
                              newAcceleration,
                              m_vSmoothedAcceleration);
    }

    // Euler integrate (per frame) acceleration into velocity
    newVelocity += m_vSmoothedAcceleration * elapsedTime;

    // enforce speed limit
    newVelocity = newVelocity.truncateLength (maxSpeed ());

    // update Speed
    setSpeed (newVelocity.length());

    // Euler integrate (per frame) velocity into position
    setPosition (position() + (newVelocity * elapsedTime));

    // regenerate local space (by default: align vehicle's forward axis with
    // new velocity, but this behavior may be overridden by derived classes.)
    regenerateLocalSpace (newVelocity, elapsedTime);

    // maintain path curvature information
    measurePathCurvature (elapsedTime);

    // running average of recent positions
    blendIntoAccumulator (elapsedTime * 0.06f, // QQQ
                          position (),
                          m_vSmoothedPosition);
}


// ----------------------------------------------------------------------------
// the default version: keep FORWARD parallel to velocity, change UP as
// little as possible.
//
// parameter names commented out to prevent compiler warning from "-W"


void OpenSteer::AIMovable::regenerateLocalSpace (const Vec3& newVelocity,
                                                const float /* elapsedTime */)
{
    
    // adjust orthonormal basis vectors to be aligned with new velocity
    if (speed() > 0) regenerateOrthonormalBasisUF (newVelocity / speed());
    
}


// ----------------------------------------------------------------------------
// alternate version: keep FORWARD parallel to velocity, adjust UP according
// to a no-basis-in-reality "banking" behavior, something like what birds and
// airplanes do

// XXX experimental cwr 6-5-03


void OpenSteer::AIMovable::regenerateLocalSpaceForBanking (const Vec3& newVelocity,
                                                          const float elapsedTime)
{
    
    // the length of this global-upward-pointing vector controls the vehicle's
    // tendency to right itself as it is rolled over from turning acceleration
    const Vec3 globalUp (0, 0.2f, 0);

    // acceleration points toward the center of local path curvature, the
    // length determines how much the vehicle will roll while turning
    const Vec3 accelUp = m_vSmoothedAcceleration * 0.05f;

    // combined banking, sum of UP due to turning and global UP
    const Vec3 bankUp = accelUp + globalUp;

    // blend bankUp into vehicle's UP basis vector
    const float smoothRate = elapsedTime * 3;
    Vec3 tempUp = up();
    blendIntoAccumulator (smoothRate, bankUp, tempUp);
    setUp (tempUp.normalize());

//  annotationLine (position(), position() + (globalUp * 4), gWhite);  // XXX
//  annotationLine (position(), position() + (bankUp   * 4), gOrange); // XXX
//  annotationLine (position(), position() + (accelUp  * 4), gRed);    // XXX
//  annotationLine (position(), position() + (up ()    * 1), gYellow); // XXX

    // adjust orthonormal basis vectors to be aligned with new velocity
    if (speed() > 0) regenerateOrthonormalBasisUF (newVelocity / speed());
    
}


// ----------------------------------------------------------------------------
// measure path curvature (1/turning-radius), maintain smoothed version


void OpenSteer::AIMovable::measurePathCurvature (const float elapsedTime)
{
    
    if (elapsedTime > 0)
    {
        const Vec3 dP = m_vLastPosition - position ();
        const Vec3 dF = (m_vLastForward - forward ()) / dP.length ();
        const Vec3 lateral = dF.perpendicularComponent (forward ());
        const float sign = (lateral.dot (side ()) < 0) ? 1.0f : -1.0f;
        m_fCurvature = lateral.length() * sign;
        blendIntoAccumulator (elapsedTime * 4.0f,
                              m_fCurvature,
                              m_fSmoothedCurvature);
        m_vLastForward = forward ();
        m_vLastPosition = position ();
    }
    
}


// ----------------------------------------------------------------------------
// draw lines from vehicle's position showing its velocity and acceleration


void OpenSteer::AIMovable::annotationVelocityAcceleration (float maxLengthA, 
                                                          float maxLengthV)
{
    /*
    // Linked errors
    const float desat = 0.4f;
    const float aScale = maxLengthA / maxForce ();
    const float vScale = maxLengthV / maxSpeed ();
    const Vec3& p = position();
    const Vec3 aColor (desat, desat, 1); // bluish
    const Vec3 vColor (    1, desat, 1); // pinkish

    annotationLine (p, p + (velocity ()           * vScale), vColor);
    annotationLine (p, p + (_smoothedAcceleration * aScale), aColor);
    */
}


// ----------------------------------------------------------------------------
// predict position of this vehicle at some time in the future
// (assumes velocity remains constant, hence path is a straight line)
//
// XXX Want to encapsulate this since eventually I want to investigate
// XXX non-linear predictors.  Maybe predictFutureLocalSpace ?
//
// XXX move to a vehicle utility mixin?


OpenSteer::Vec3 OpenSteer::AIMovable::predictFuturePosition (const float predictionTime) const
{        
    return position() + (velocity() * predictionTime);
}

