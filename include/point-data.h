#ifndef HASH_OCTREE_POINT_DATA_H
#define HASH_OCTREE_POINT_DATA_H

#include <fstream>
#include <iostream>

#include "hash-octree.h"

namespace HashOctree {

    class Point {
    public:
        NodeDims dim;
        data_t data;

        friend std::istream& operator>>(std::istream &is, const Point &p);

        friend std::ostream& operator<<(std::ostream &os, const Point &p);
    };

    class PointData {
    public:
        PointData();

        PointData(std::string filename);

        int readAll(std::string filename);
    };

}

#endif //HASH_OCTREE_POINT_DATA_H
