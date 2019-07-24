#ifndef HASH_OCTREE_LOOKUP_METHODS_H
#define HASH_OCTREE_LOOKUP_METHODS_H

#include <unordered_map>
#include <vector>

#include "../node.h"

namespace HashOctree {

    class LookupMethod {
    public:
        virtual status_t lookup(key_t key, NodeControlBlock **outNcb) = 0;

        virtual bool contains(key_t key) = 0;

        virtual status_t insert(key_t key, const NodeControlBlock &ncb) = 0;

        virtual status_t erase(key_t key) = 0;

        virtual std::vector<std::pair<key_t, NodeControlBlock&>> list() = 0;
    };

}

#endif //HASH_OCTREE_LOOKUP_METHODS_H
