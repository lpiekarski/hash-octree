#ifndef HASH_OCTREE_MAP_LOOKUP_METHOD_H
#define HASH_OCTREE_MAP_LOOKUP_METHOD_H

#include <map>

#include "lookup-methods.h"

namespace HashOctree {

    class MapLookupMethod : public LookupMethod {
    private:
        std::map<key_t, NodeControlBlock> nodes;

    public:
        status_t lookup(key_t key, NodeControlBlock **outNcb);

        bool contains(key_t key);

        status_t insert(key_t key, const NodeControlBlock &ncb);

        status_t erase(key_t key);

        std::vector<std::pair<key_t, NodeControlBlock&>> list();
    };

}

#endif //HASH_OCTREE_MAP_LOOKUP_METHOD_H
