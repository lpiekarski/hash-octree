#include <cstdlib>

#include "../include/node.h"

namespace HashOctree {

    Node::Node(const key_t *children, const void *data) {
        this->data = (void *)data;
        for (size_t i = 0; i < 8; i++)
            this->children[i] = children[i];
    }

}