#ifndef HASH_OCTREE_LOOKUP_METHODS_H
#define HASH_OCTREE_LOOKUP_METHODS_H

#include <unordered_map>
#include <vector>

#include "../node.h"

namespace HashOctree {

    class LookupMethod {
    public:
        virtual NodeControlBlock &lookup(key_t key) = 0;

        virtual const NodeControlBlock &lookup(key_t key) const = 0;

        virtual bool contains(key_t key) = 0;

        virtual void insert(key_t key, const NodeControlBlock &ncb) = 0;

        virtual void erase(key_t key) = 0;

        virtual std::vector<std::pair<key_t, NodeControlBlock&>> list() = 0;

        virtual std::vector<std::pair<key_t, const NodeControlBlock&>> list() const = 0;
    };

}

#endif //HASH_OCTREE_LOOKUP_METHODS_H
