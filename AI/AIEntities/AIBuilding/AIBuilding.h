#ifndef _BUILDING_AI_H_
#define _BUILDING_AI_H_

#include "AIGame.h"
#include <cassert>

class AIBuilding: public AIGame
{
public:

	AIBuilding():AIGame()
    {
        reconstructionTime = 0;
        constructionTime = 0; 
    } 

protected:

    double reconstructionTime;
    double constructionTime;

};

#endif