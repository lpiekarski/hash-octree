#ifndef HASH_OCTREE_NODE_H
#define HASH_OCTREE_NODE_H

#include "defs.h"

namespace HashOctree {

    class NodeDims {
    public:
        dim_t origin[3];
        dim_t halfDim[3];

        NodeDims() = default;

        NodeDims(const NodeDims &) = default;

        NodeDims &operator=(const NodeDims &) = default;

        NodeDims(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd);
    };

    class Node {
    public:
        key_t children[8];
        void *data;

        Node() = default;

        Node(const key_t *children, const void *data);

        friend bool operator==(const Node &n1, const Node &n2);

        friend bool operator!=(const Node &n1, const Node &n2);

    };

    class NodeControlBlock {
    public:
        key_t key;
        Node node;
        int32_t refs;

        friend bool operator==(const NodeControlBlock &ncb1, const NodeControlBlock &ncb2);

        friend bool operator!=(const NodeControlBlock &ncb1, const NodeControlBlock &ncb2);
    };

    class NodeOperationBlock {
    public:
        NodeControlBlock *ncb;
        NodeDims dim;
        NodeControlBlock *parent;
    };

}

#endif //HASH_OCTREE_NODE_H
