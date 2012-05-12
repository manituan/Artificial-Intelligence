#ifndef STEERING_BEHAVIORS_
#define STEERING_BEHAVIORS_

#include <vector>

#include <Ogre.h>

#include "steering.h"
#include "AIMovil.h"
#include "AIUnit.h"
#include "AIGame.h"
#include "navigationMesh.h"

class AIMovil;
class AIUnit;

// Clase base

//! Clase base abstracta que modela los Steering Behaviours de forma general

class SteeringBehaviour {
    public:
        /** Información cinemática del personaje */
        AIMovil* character;
        
        /**
         * Constructor
         * 
         * @param character información cinemática del personaje sobre el que actuará el comportamiento
         */
        SteeringBehaviour(AIMovil* character = 0);
        
        /**
         * Método virtual puro
         * 
         * @param steering steering sobre el que escribirá el comportamiento
         * 
         * Toma información del entorno y produce un steering que deberá afectar
         * a la cinemática del personaje para conseguir el comportamiento buscado.
         */
        virtual void getSteering(Steering& steering) = 0;
    
    protected:
        inline const Ogre::Vector3 angleToVector(Ogre::Real angle) {
            return Ogre::Vector3(sin(angle), 0.0f, cos(angle));
        }
};



// Steering Behaviours básicos

//! Comportamiento para buscar un objetivo a máxima velocidad

/**
 * 
 * Seek cuenta con un personaje y un objetivo, hace que el personaje se
 * dirija hacia el objetivo a máxima velocidad. No está preparado para que
 * llegue y se pare. Por su aceleración se pasará y tratará de volver.
 * Si se quiere llegar al objetivo, es más recomendable utilizar Arrive.
 */
class Seek: public SteeringBehaviour {
    public:
        AIGame* target;
        Ogre::Real maxAcceleration;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Seek(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Seek
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
};

//! Comportamiento para huir de un objetivo a máxima velocidad

class Flee: public SteeringBehaviour {
    public:
        AIGame*   target;
        Ogre::Real maxAcceleration;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Flee(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Flee
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
};

//! Comportamiento para alcanzar un objetivo decelerando al final

class Arrive: public SteeringBehaviour {
    public:
        AIGame* target;
        Ogre::Real maxAcceleration;
        Ogre::Real targetRadius;
        Ogre::Real slowRadius;
        Ogre::Real timeToTarget;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Arrive(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Arrive
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
};

//! Comportamieno para tomar la misma orientación que el objetivo

class Align: public SteeringBehaviour {
    public:
        AIGame* target;
        Ogre::Real maxAngularAcceleration;
        Ogre::Real maxRotation;
        Ogre::Real targetRadius;
        Ogre::Real slowRadius;
        Ogre::Real timeToTarget;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Align(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Align
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
        
    protected:
        Ogre::Real mapToRange(Ogre::Real angle);
};

//! Comportamiento para alcanzar la misma velocidad que el objetivo

class VelocityMatch: public SteeringBehaviour {
    public:
        AIGame* target;
        Ogre::Real maxAcceleration;
        Ogre::Real timeToTarget;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        VelocityMatch(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de VelocityMatch
         * 
         * @param steering steering a modificar
         */
        virtual void getSteering(Steering& steering);
    
    protected:
        Ogre::Real mapToRange(Ogre::Real angle);
};



// Steering Behaviours compuestos

//! Comportamiento para perseguir un objetivo prediciendo su posición futura

class Pursue: public Seek {
    public:
        AIGame* target; // Tenemos dos: pursue.target y seek.target
        Ogre::Real maxPrediction;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Pursue(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Pursue
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para huir de un objetivo prediciendo su posición futura

class Evade: public Flee {
    public:
        AIGame* target; // Tenemos dos: evade.target y flee.target
        Ogre::Real maxPrediction;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Evade(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Evade
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para mirar hacia un objetivo


class Face: public Align {
    public:
        AIGame* target; // Tenemos dos: face.target y align.target
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param target objetivo
         */
        Face(AIMovil* character = 0, AIGame* target = 0);
        
        /**
         * Modifica el steering según el comportamiento de Face
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para vagar por el espacio de forma aleatoria


class Wander: public Face {
    public:
        Ogre::Real wanderOffset;
        Ogre::Real wanderRadius;
        Ogre::Real wanderRate;
        Ogre::Real wanderOrientation;
        Ogre::Real maxAcceleration;
        
        /**
         * Constructor
         * 
         * @param character personaje
         */
        Wander(AIMovil* character = 0);
        
        /**
         * Modifica el steering según el comportamiento de Wander
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering);
        
    protected:
        inline Ogre::Real randomBinomial() {
            return (rand() - rand()) / (RAND_MAX * 1.0f);
        }
};

//! Comportamiento para evitar colisiones entre enemigos

class CollisionAvoidance: public Flee {
    public:
        Ogre::Real maxAcceleration;
        std::vector<AIGame*> targets;
        AIMovil* myself;
        Ogre::Real radius;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param targets lista de enemigos a evitar
         * @param AIMovil el propio enemigo, no se tiene en cuenta para evitar (o se evitaría a sí mismo)
         */
        CollisionAvoidance(const std::vector<AIGame*> targets,
                           AIUnit* myself);
        
        /**
         * Modifica el steering según el comportamiento de CollisionAvoidance
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering); 
};

//! Comportamiento para seguir un camino PointPath

class FollowPath: public Arrive {
    public:
        NavigationMesh::PointPath* path;
        Ogre::Real pathOffset;
        
        /**
         * Constructor
         * 
         * @param character personaje
         * @param path camino que debe seguir el personaje
         */
        FollowPath(AIMovil* character, NavigationMesh::PointPath* path);
        
        /**
         * Modifica el steering según el comportamiento de FollowPath
         * 
         * @param steering steering a modificar
         */
        void getSteering(Steering& steering);
        
    protected:
        Ogre::Vector3 findClosestPathPoint(const Ogre::Vector3& point);
        Ogre::Vector3 findTargetInPath();
};


#endif
