#ifndef HASH_OCTREE_UNORDERED_MAP_LOOKUP_METHOD_H
#define HASH_OCTREE_UNORDERED_MAP_LOOKUP_METHOD_H

#include "lookup-methods.h"

namespace HashOctree {

    class UnorderedMapLookupMethod : LookupMethod {
    private:
        std::unordered_map<key_t, NodeControlBlock> nodes;

    public:
        NodeControlBlock &lookup(key_t key);

        const NodeControlBlock &lookup(key_t key) const;

        bool contains(key_t key);

        void insert(key_t key, const NodeControlBlock &ncb);

        void erase(key_t key);

        std::vector<std::pair<key_t, NodeControlBlock&>> list();

        std::vector<std::pair<key_t, const NodeControlBlock&>> list() const;
    };

}

#endif //HASH_OCTREE_UNORDERED_MAP_LOOKUP_METHOD_H
