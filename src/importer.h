#ifndef HASH_OCTREE_IMPORTER_H
#define HASH_OCTREE_IMPORTER_H

#include "hash-octree.h"
#include "point-data.h"

namespace HashOctree {

    class Importer {
    public:
        static HashOctree fromByteArray(const std::vector<char> &array);

        static HashOctree fromJson(const std::string &str);

        static HashOctree fromPointData(const PointData &pdt);
    };

}

#endif //HASH_OCTREE_IMPORTER_H
