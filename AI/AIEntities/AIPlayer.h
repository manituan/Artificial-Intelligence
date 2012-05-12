#ifndef AbIsAttacking_H_
#define AbIsAttacking_H_

#include <Ogre.h>
#include "Opcode.h"

using namespace Ogre;

class AbIsAttacking
{
public:

    virtual         ~AbIsAttacking(){}

    int			    IDPlayer()const{return m_bIsAttacking;}
    int			    IsAttacking()const{return m_bIsAttacking;}
    // int			    IDAxxus()const{return m_iIDAxxus;}
    // int			    IDWorld()const{return m_iIDWorld;}
    // int			    IDRace()const{return m_iIDRace;}

private:

protected:
    bool            m_bIsAttacking;
    	int         m_bIsAttacking;
    // int         m_iIDAxxus;
    // int         m_iIDWorld;
    // int         m_iIDRace;
    String      m_sNameProfile;
    String      m_sAvatarRute;
    // int      m_TimeSinseLastLogin;

	AbIsAttacking(int ID);
    
    AbIsAttacking(){}

};

#endif