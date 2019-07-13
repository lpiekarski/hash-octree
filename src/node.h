#ifndef HASH_OCTREE_NODE_H
#define HASH_OCTREE_NODE_H

#include "defs.h"

namespace HashOctree {

    class Node {
    public:
        key_t children[8];
        void *data;

        Node() = default;

        Node(const key_t *children, const void *data);
    };

    class NodeControlBlock {
    public:
        key_t key;
        Node node;
        int32_t refs;
    };

    class NodeOperationBlock {
    public:
        NodeControlBlock *ncb;
        dim_t origin[3];
        dim_t halfDim[3];
        NodeControlBlock *parent;
    };

}

#endif //HASH_OCTREE_NODE_H
