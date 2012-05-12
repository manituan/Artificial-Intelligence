#include "AISystem.h"

int AISystem::m_iNextValidID = 0;

//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------
AISystem::AISystem(int ID):m_iType(default_entity_type)
{
    m_bTag = false;
    m_bIsAttacking = false;
    m_dRadius = 3.0;
    m_vPos = 0;
    m_vPrevPos = m_vPos;
    m_vOri = 0;
    m_iType = 0;
    m_dTimeSinceLastFrame = 0;

    SetID(ID);
}

//----------------------------- SetID -----------------------------------------
//
//  this must be called within each constructor to make sure the ID is set
//  correctly. It verifies that the value passed to the method is greater
//  or equal to the next valid ID, before setting the ID and incrementing
//  the next valid ID
//-----------------------------------------------------------------------------
void AISystem::SetID(int val)
{
  //make sure the val is equal to or greater than the next available ID
  assert ( (val >= m_iNextValidID) && "<AISystem::SetID>: invalid ID");

  m_ID = val;
    
  m_iNextValidID = m_ID + 1;
}