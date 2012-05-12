
#ifndef NAVIGATION_MESH_
#define NAVIGATION_MESH_

#include <vector>
#include <list>

#include <Ogre.h>

#include "cell.h"

class NavigationMesh {
    public:
        typedef std::vector<Cell*> Cells;
        
        typedef std::list<Cell*> CellPath;

        typedef std::list<Ogre::Vector3> PointPath;

        NavigationMesh(const Ogre::String& fileName = "");

        ~NavigationMesh();

        void addCell(int id,
                     const Ogre::Vector3& pointA,
                     const Ogre::Vector3& pointB,
                     const Ogre::Vector3& pointC);
        
        void linkCells();
        
        void clear();
        
        Cell* getCell(int index);

        int getCellNumber();

        bool buildPath(PointPath& path,
                       const Ogre::Vector3& startPos,
                       const Ogre::Vector3& endPos,
                       Cell* startCell = 0,
                       Cell* endCell = 0);
        
        Cell* findCell(const Ogre::Vector3& pos);
        
        bool lineOfSightTest(const Ogre::Vector3& start,
                             const Ogre::Vector3& end,
                             Cell* startCell,
                             Cell* endCell);

        CellPath::iterator getFurthestVisibleCell(CellPath& path, CellPath::iterator startIt);
        
    private:
        void loadCellsFromXML(const Ogre::String& fileName);
        bool makeSpline(PointPath& path);
        int simplifyPath(CellPath& cellPath);
    
        Cells _cells;
        int _cellNumber;
        
        Ogre::Real* _graph;
        int* _paths;
        
        void initGraph();
        void floyd();
        void precomputePaths();
        void recoverPath(int i, int j, CellPath& cellPath);
};


#endif
