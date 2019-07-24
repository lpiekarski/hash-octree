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

        bool contains(const NodeDims &dims) const;

        bool intersects(const NodeDims &dims) const;
    };

    class Node {
    public:
        key_t children[8];
        data_t data;

        Node() = default;

        Node(const key_t *children, const data_t data);

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

        NodeOperationBlock() = default;

        //TODO error handling in this function
        template <typename LookupMethod>
        NodeOperationBlock getChildNOB(int child_id, LookupMethod &lookupMethod) const {
            NodeOperationBlock ret;

            lookupMethod.lookup(this->ncb->node.children[child_id], &ret.ncb);
            ret.parent = this->ncb;
            ret.dim.halfDim[0] = this->dim.halfDim[0] * 0.5;
            ret.dim.halfDim[1] = this->dim.halfDim[1] * 0.5;
            ret.dim.halfDim[2] = this->dim.halfDim[2] * 0.5;
            ret.dim.origin[0] = this->dim.origin[0] + (2 * (child_id % 2) - 1) * ret.dim.halfDim[0];
            ret.dim.origin[1] = this->dim.origin[1] + (2 * ((child_id / 2) % 2) - 1) * ret.dim.halfDim[1];
            ret.dim.origin[2] = this->dim.origin[2] + (2 * (child_id / 4) - 1) * ret.dim.halfDim[2];

            return ret;
        }
    };

}

#endif //HASH_OCTREE_NODE_H
