#ifndef _OGRE_UNIT_DEF_H_
#define _OGRE_UNIT_DEF_H_

#include <string>
#include <vector>
#include <map>



struct Ogre_Unit_Def
{
	std::string BodyMeshName;
	std::string BodyMaterialName;
};

//extern std::vector<Unit_Ogre_Def*> Unit_Ogre_Define;
//extern std::vector<std::string> Unit_Ogre_Name;

extern std::map<std::string, Ogre_Unit_Def*> Unit_Ogre_Map;

bool LoadUnitOgreDefinitions( std::string& filename );
void UnloadUnitOgreDefinitions();

#endif