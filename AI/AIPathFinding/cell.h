#ifndef CELL_
#define CELL_

#include <Ogre.h>

#include "line2D.h"

class Cell {
    public:

        enum CellSide {
            SIDE_AB = 0,
            SIDE_BC,
            SIDE_CA
        };
        
        enum CellVert {
            VERT_A = 0,
            VERT_B,
            VERT_C
        };
        
        enum PathResult {
            NO_RELATIONSHIP,
            ENDING_CELL,	
            EXITING_CELL,
        };

        Cell(int id,
             const Ogre::Vector3& pointA,
             const Ogre::Vector3& pointB,
             const Ogre::Vector3& pointC);

        ~Cell();

        int getId() const;
 
        const Ogre::Vector3& getVertex(int index);
        
        Cell* getLink(CellSide side) const;

        void setLink(CellSide side, Cell* cell);

        bool requestLink(const Ogre::Vector3& pointA,
                         const Ogre::Vector3& pointB,
                         Cell* cell);

        const Ogre::Vector3& getCenter() const;

        bool containsPoint(const Ogre::Vector3& point) const;

        PathResult classifyPathToCell(const Line2D& path,
                                      Cell** nextCell,
                                      CellSide &side,
                                      Ogre::Vector2 &intersection);

        void getHeight(Ogre::Vector3 &point);
    private:

        int _id;

        Ogre::Vector3 _vertex[3];
        Ogre::Vector3 _center;
        Cell* _links[3];
        Line2D _sides[3];
        //Plane _plane;
};

inline int Cell::getId() const {return _id;}

#endif
