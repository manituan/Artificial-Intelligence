
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

#include "pugixml.hpp"

#include "navigationMesh.h"

using std::cout;
using std::cerr;
using std::endl;

NavigationMesh::NavigationMesh(const Ogre::String& fileName): _cellNumber(0) {
    if (fileName != "") {
        loadCellsFromXML(fileName);
        linkCells();
	initGraph();

	floyd();

	precomputePaths();
    }
}

NavigationMesh::~NavigationMesh() {

    clear();
    
    delete [] _graph;
    delete [] _paths;
}
        
void NavigationMesh::loadCellsFromXML(const Ogre::String& fileName) {
    pugi::xml_document doc;
    std::vector<int> a, b, c;
    std::vector<Ogre::Vector3> vertex;
    
    if (!doc.load_file(fileName.c_str())) {
        cerr << "NavigationMesh::loadCellsFromXML(): error al leer el fichero " << fileName << endl;
        exit(1);
    }

    pugi::xml_node meshNode = doc.child("mesh");
    pugi::xml_node subMeshesNode = meshNode.child("submeshes");
    pugi::xml_node subMeshNode = subMeshesNode.child("submesh");
    pugi::xml_node facesNode = subMeshNode.child("faces");
    
    for (pugi::xml_node faceNode = facesNode.child("face"); faceNode; faceNode = faceNode.next_sibling()) {
        int v1 = faceNode.attribute("v1").as_int();
        int v2 = faceNode.attribute("v2").as_int();
        int v3 = faceNode.attribute("v3").as_int();
        
        a.push_back(v1);
        b.push_back(v2);
        c.push_back(v3);
    }
    
    pugi::xml_node geometryNode = subMeshNode.child("geometry");
    pugi::xml_node vertexBufferNode = geometryNode.child("vertexbuffer");

    for (pugi::xml_node vertexNode = vertexBufferNode.child("vertex"); vertexNode; vertexNode = vertexNode.next_sibling()) {
        pugi::xml_node positionNode = vertexNode.child("position");
        Ogre::Vector3 v = Ogre::Vector3(positionNode.attribute("x").as_float(),
                                       positionNode.attribute("y").as_float(),
                                       positionNode.attribute("z").as_float());
        
        vertex.push_back(v);
    }
    
    int faceCount = a.size();

    for (int i = 0; i < faceCount; ++i)
        addCell(i, vertex[a[i]], vertex[b[i]], vertex[c[i]]);
}

void NavigationMesh::clear() {

    for (Cells::iterator i = _cells.begin(); i != _cells.end(); ++i)
        delete (*i);
    

    _cells.clear();
    
    _cellNumber = 0;
}
        
void NavigationMesh::addCell(int id,
			     const Ogre::Vector3& pointA,
                             const Ogre::Vector3& pointB,
                             const Ogre::Vector3& pointC) {

    Cell* cell = new Cell(id, pointA, pointB, pointC);

    _cells.push_back(cell);
    
    ++_cellNumber;
}
                     
void NavigationMesh::linkCells() {
    Cell* cellA;
    Cell* cellB;

    for (Cells::iterator i = _cells.begin(); i != _cells.end(); ++i) {
        for (Cells::iterator j = _cells.begin(); j != _cells.end(); ++j) {
            if (i != j) {
                cellA = *i;
                cellB = *j;
                
                if (!cellA->getLink(Cell::SIDE_AB) && cellB->requestLink(cellA->getVertex(0), cellA->getVertex(1), cellA))
                    cellA->setLink(Cell::SIDE_AB, cellB);
                else if (!cellA->getLink(Cell::SIDE_BC) && cellB->requestLink(cellA->getVertex(1), cellA->getVertex(2), cellA))
                    cellA->setLink(Cell::SIDE_BC, cellB);
                else if (!cellA->getLink(Cell::SIDE_CA) && cellB->requestLink(cellA->getVertex(2), cellA->getVertex(0), cellA))
                    cellA->setLink(Cell::SIDE_CA, cellB);
            }
        }
    }
}
        
Cell* NavigationMesh::getCell(int index) {
    if (index < 0 || index >= _cellNumber) {
        cerr << "NavigationMesh::getCell(): la celda con índice: " << index << " no existe" << endl;
        exit(1);
    }
    
    return _cells[index];
}
        
int NavigationMesh::getCellNumber() {
    return _cellNumber;
}

Ogre::Vector3 CatmullRollSpline(const Ogre::Vector3& p0,
				const Ogre::Vector3& p1,
				const Ogre::Vector3& p2,
				const Ogre::Vector3& p3,
				Ogre::Real t ) {

	Ogre::Real t2 = t * t;
	Ogre::Real t3 = t2 * t;

	Ogre::Vector3 v = 0.5 * ( (2 * p1)
		    + (-1 * p0 + p2) * t
	            + (2 * p0 - 5 * p1 + 4 * p2 - 1 * p3) * t2 
		    + (-1 * p0 + 3 * p1 - 3 * p2 + 1 * p3) * t3);

	return v;
}

int NavigationMesh::simplifyPath(CellPath& cellPath) {

	CellPath::iterator beginIt = cellPath.begin(),
		endIt;
	size_t sz = cellPath.size();
	

	while( (endIt = getFurthestVisibleCell(cellPath, beginIt)) != cellPath.end() ) {
		
		++beginIt;

		sz = cellPath.size();

		if( beginIt != cellPath.end() )
			beginIt = cellPath.erase(beginIt, endIt);

	}

	return sz - cellPath.size();
}

bool NavigationMesh::makeSpline(PointPath& path) {

	if( path.size() < 2 )
		return false;

	PointPath::iterator p0, p1, p2, p3;

	p0 = p1 = path.begin();
	++p1;
	Ogre::Vector3 pextra = *p0 - ((*p1 - *p0).normalise() * 0.5);
	path.push_front(pextra);

	p0 = p1 = path.end();
	--p1;
	--p0;
	--p0;
	pextra = *p1 + ((*p1 - *p0).normalise() * 0.5);
	path.push_back(pextra);

	int dotsPerUnit = 2;

	p0 = path.begin();

	p1 = p0; 
	++p1;

	p2 = p1; 
	++p2;

	p3 = p2; 
	++p3;


	while( p3 != path.end() ){

		int n = (*p1 - *p2).length() * dotsPerUnit;
		Ogre::Real step = 1.0/n;
		Ogre::Real s = step;

		for(int i = 1; i < n; ++i) {
			path.insert(p2, CatmullRollSpline(*p0, *p1, *p2, *p3, s));
			s += step;
		}

		p0 = p1;
		p1 = p2;
		++p2;
		++p3;

	}

	path.pop_front();
	path.pop_back();

	return true;
}

bool NavigationMesh::buildPath(PointPath& path,
                               const Ogre::Vector3& startPos,
                               const Ogre::Vector3& endPos,
                               Cell* startCell,
                               Cell* endCell) {

    if (!startCell)
	startCell = findCell(startPos);

    if (!endCell)
	endCell = findCell(endPos);

    if (!startCell || !endCell) {
	cout << "Celdas no pertenecen a la rejilla" << endl;
	return false;
    }

    int startId = startCell->getId();
    int endId = endCell->getId();
    
    if (_graph[startId * _cellNumber + endId] == Ogre::Math::POS_INFINITY) {
	cout << "No se ha encontrado camino" << endl;
	return false;
    }

    CellPath cellPath;

    recoverPath(startId, endId, cellPath);

    path.clear();
	
    for(CellPath::iterator i = cellPath.begin(); i != cellPath.end(); ++i)
	path.push_back((*i)->getCenter());

    path.push_front(startPos);

    path.push_back(endPos);

    makeSpline(path);

    return true;
}
        
Cell* NavigationMesh::findCell(const Ogre::Vector3& pos) {
    Ogre::Vector3 v;
    Ogre::Vector3 minDistance = Ogre::Vector3(500.0, 500.0, 500.0);
    Cell* closestCell = 0;
    
    for (Cells::iterator i = _cells.begin(); i != _cells.end(); ++i) {
        if ((*i)->containsPoint(pos)) {
            return *i;
        }
	
	v = (*i)->getCenter() - pos;
	if (v.squaredLength() < minDistance.squaredLength()) {
	    minDistance = v;
	    closestCell = (*i);
	}
    }

    return closestCell;
}

bool NavigationMesh::lineOfSightTest(const Ogre::Vector3& start,
                                     const Ogre::Vector3& end,
                                     Cell* startCell,
                                     Cell* endCell) {
    
    Cell* nextCell = startCell;
    Cell::PathResult result = Cell::EXITING_CELL;
    Line2D line2D(Ogre::Vector2(start.x, start.z),
                  Ogre::Vector2(end.x, end.z));
    Cell::CellSide side;
    Ogre::Vector2 intersection;
    
    while (result == Cell::EXITING_CELL) {
        
        result = nextCell->classifyPathToCell(line2D, &nextCell, side, intersection);
        
        if (!nextCell) {
            return false;
        }
    }
    
    return (result == Cell::ENDING_CELL);
}

NavigationMesh::CellPath::iterator NavigationMesh::getFurthestVisibleCell(CellPath& path, CellPath::iterator startIt) {

    if (startIt == path.end())
        return startIt;

    CellPath::iterator cellIt = startIt;
    ++cellIt;
    
    CellPath::iterator lastIt = cellIt;
    
    if (lastIt == path.end())
	return lastIt;
	
    for (; cellIt != path.end(); ++cellIt) {
	if (lineOfSightTest((*startIt)->getCenter(), (*cellIt)->getCenter(), *startIt, *cellIt))
	    lastIt = cellIt;
	else
	    return lastIt;
    }
    
    return lastIt;
    
}


void NavigationMesh::initGraph() {

    _graph = new Ogre::Real[_cellNumber * _cellNumber];
    _paths = new int[_cellNumber * _cellNumber];
    

    for (int i = 0; i < _cellNumber * _cellNumber; ++i) {

	_graph[i] = Ogre::Math::POS_INFINITY;

	_paths[i] = -1;
    }
    
    for (Cells::iterator it = _cells.begin(); it != _cells.end(); ++it) {

	Cell* cellA = *it;
	int idA = cellA->getId();

	_graph[idA * _cellNumber + idA] = 0.0f;
	
	Cell* cellB;
	int idB;

	cellB = cellA->getLink(Cell::SIDE_AB);
	if (cellB != 0) {
	    idB = cellB->getId();
	    _graph[idA * _cellNumber + idB] = 1.0f;
	    _graph[idB * _cellNumber + idA] = 1.0f;
	}
	
	cellB = cellA->getLink(Cell::SIDE_BC);
	if (cellB != 0) {
	    idB = cellB->getId();
	    _graph[idA * _cellNumber + idB] = 1.0f;
	    _graph[idB * _cellNumber + idA] = 1.0f;
	}
	
	cellB = cellA->getLink(Cell::SIDE_CA);
	if (cellB != 0) {
	    idB = cellB->getId();
	    _graph[idA * _cellNumber + idB] = 1.0f;
	    _graph[idB * _cellNumber + idA] = 1.0f;
	}
    }
}

void NavigationMesh::floyd() {
    for (int k = 0; k < _cellNumber; ++k) {
	for (int i = 0; i < _cellNumber; ++i) {
	    for (int j = 0; j < _cellNumber; ++j) {
		Ogre::Real ikj = _graph[i * _cellNumber + k] + _graph[k * _cellNumber + j];
		if (ikj < _graph[i * _cellNumber + j]) {
		    _graph[i * _cellNumber + j] = ikj;
		    _paths[i * _cellNumber + j] = k;
		}
	    }
	}
    }
}

void NavigationMesh::precomputePaths() {

    for (int i = 0; i < _cellNumber; ++i) {
	for (int j = 0; j < _cellNumber; ++j) {

	    CellPath cellPath;
	    recoverPath(i, j, cellPath);

	    simplifyPath(cellPath);

	    for (CellPath::iterator it = cellPath.begin(); it != cellPath.end(); ++it) {
		CellPath::iterator nextIt = it;
		++nextIt;

		if (nextIt != cellPath.end()) {
		    int idA = (*it)->getId();
		    int idB = (*nextIt)->getId();

		    _paths[idA * _cellNumber + idB] = -1;
		}
	    }
	}
    }
    
    //delete [] _paths;
    //_paths = newPaths;
}

void NavigationMesh::recoverPath(int i, int j, CellPath& cellPath) {

    int k = _paths[i * _cellNumber + j];

    if (k != -1) {

	recoverPath(i, k, cellPath);
	
	cellPath.push_back(_cells[k]);
	
	recoverPath(k, j, cellPath);
    }
}

