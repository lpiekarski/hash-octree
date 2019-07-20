#include <cstdlib>

#include "../include/node.h"

namespace HashOctree {

    NodeDims::NodeDims(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd) {
        origin[0] = x;
        origin[1] = y;
        origin[2] = z;

        halfDim[0] = hw;
        halfDim[1] = hh;
        halfDim[2] = hd;
    }

    Node::Node(const key_t *children, const void *data) {
        this->data = (void *)data;
        for (size_t i = 0; i < 8; i++)
            this->children[i] = children[i];
    }

    bool operator==(const Node &n1, const Node &n2) {
        return (n1.data == n2.data &&
                n1.children[0] == n2.children[0] &&
                n1.children[1] == n2.children[1] &&
                n1.children[2] == n2.children[2] &&
                n1.children[3] == n2.children[3] &&
                n1.children[4] == n2.children[4] &&
                n1.children[5] == n2.children[5] &&
                n1.children[6] == n2.children[6] &&
                n1.children[7] == n2.children[7]);
    }

    bool operator!=(const Node &n1, const Node &n2) {
        return (n1.data != n2.data ||
                n1.children[0] != n2.children[0] ||
                n1.children[1] != n2.children[1] ||
                n1.children[2] != n2.children[2] ||
                n1.children[3] != n2.children[3] ||
                n1.children[4] != n2.children[4] ||
                n1.children[5] != n2.children[5] ||
                n1.children[6] != n2.children[6] ||
                n1.children[7] != n2.children[7]);
    }

    bool operator==(const NodeControlBlock &ncb1, const NodeControlBlock &ncb2) {
        return (ncb1.key == ncb2.key &&
                ncb1.node == ncb2.node &&
                ncb1.refs == ncb2.refs);
    }

    bool operator!=(const NodeControlBlock &ncb1, const NodeControlBlock &ncb2) {
        return (ncb1.key != ncb2.key ||
                ncb1.node != ncb2.node ||
                ncb1.refs != ncb2.refs);
    }

}