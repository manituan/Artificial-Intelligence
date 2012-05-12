#ifndef SIONTOWER_TRUNK_SRC_INCLUDE_STEERING_H_
#define SIONTOWER_TRUNK_SRC_INCLUDE_STEERING_H_

#include <iostream>


#include <Ogre.h>

struct Steering {
    public:
    
        /**
         * Constructor
         * 
         * @param linear aceleración lineal
         * @param angular aceleración angular
         */
        Steering(const Ogre::Vector3& linear = Ogre::Vector3::ZERO, Ogre::Real angular = 0.0f){};
        
        /**
         * @return aceleración lineal
         */
        const Ogre::Vector3& getLinear() const;
        
        /**
         * @return aceleración angular
         */
        Ogre::Real getAngular() const;
        
        /**
         * @return true si tanto la aceleración lineal como angular son nulas
         */
        bool isNone() const;
        
        /**
         * @param linear nueva aceleración lineal
         */
        void setLinear(const Ogre::Vector3 linear);
        
        /**
         * @param angular nueva aceleración angular
         */
        void setAngular(Ogre::Real angular);
        
        /**
         * Hace que tanto la aceleración angular como lineal sean nulas
         */
        void setNone();
        
        
        // Operadores
        /**
         * @param other otro steering para comparar
         * @return true si ambos tienen la misma aceleración (angular y lineal)
         */
        bool operator == (const Steering& other);
        
        /**
         * @param other otro steering
         * @return nuevo steering cuyas aceleraciones son la suma de los dos
         */
        Steering& operator + (const Steering& other);
        
        /**
         * @param other otro steering
         * @return steering inicial modificado, sus aceleraciones se han sumado con las de other
         */
        Steering& operator += (const Steering& other);
        
        /**
         * @param f factor
         * @return nuevo steering cuyas aceleraciones son el producto de las anteriores por f
         */
        Steering& operator * (Ogre::Real f);
        
        /**
         * @param f factor
         * @return steering modificados cuyas aceleraciones son el producto de las anteriores por f
         */
        Steering& operator *= (Ogre::Real f);
        
        /**
         * @param f factor
         * @return nuevo steering cuyas aceleraciones son el cociente de las anteriores entre f
         */
        Steering& operator / (Ogre::Real f);
        
        /**
         * @param f factor
         * @return steering modificado cuyas aceleraciones son el cociente de las anteriores entre f
         */
        Steering& operator /= (Ogre::Real f);
        
        
    private:
        Ogre::Vector3 _linear;
        Ogre::Real _angular;
};


// Métodos inline
inline const Ogre::Vector3& Steering::getLinear() const {return _linear;}
inline Ogre::Real Steering::getAngular() const {return _angular;}
inline bool Steering::isNone() const {return _linear == Ogre::Vector3::ZERO && _angular == 0.0f;}
        
inline void Steering::setLinear(const Ogre::Vector3 linear) {_linear = linear;}
inline void Steering::setAngular(Ogre::Real angular) {_angular = angular;}
inline void Steering::setNone() {_linear = Ogre::Vector3::ZERO; _angular = 0.0f;}

// Inserción en flujo
inline std::ostream& operator << (std::ostream& os, const Steering& s) {
    os << "Steering:   linear: " << s.getLinear() << " angular: " << s.getAngular() << std::endl;
    
    return os;
}


#endif  // SIONTOWER_TRUNK_SRC_INCLUDE_STEERING_H_
