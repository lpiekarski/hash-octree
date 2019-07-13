#ifndef HASH_OCTREE_POINT_DATA_H
#define HASH_OCTREE_POINT_DATA_H

#include <fstream>
#include <iostream>

#include "hash-octree.h"

namespace HashOctree {

    class Point {
    public:
        dim_t origin[3];
        dim_t halfDim[3];
        void *data;

        friend std::istream& operator>>(std::istream &is, const Point &p);

        friend std::ostream& operator<<(std::ostream &os, const Point &p);
    };

    class PointData {
    public:
        PointData(std::string filename);

        int readAll(std::string filename);
    };

}

#endif //HASH_OCTREE_POINT_DATA_H
