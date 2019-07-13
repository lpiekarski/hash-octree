#ifndef HASH_OCTREE_EXPORTER_H
#define HASH_OCTREE_EXPORTER_H

#include "hash-octree.h"

namespace HashOctree {

    class Exporter {
    public:
        static std::string toString(const HashOctree &ho);

        static std::vector<char> toByteArray(const HashOctree &ho);

        static std::string toJson(const HashOctree &ho, size_t indentWidth, bool newLines);
    };

}

#endif //HASH_OCTREE_EXPORTER_H
