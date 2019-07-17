#ifndef HASH_OCTREE_ENCRYPTOR_H
#define HASH_OCTREE_ENCRYPTOR_H

#include "defs.h"
#include "node.h"

namespace HashOctree {

    class Encryptor {
    public:
        /**
         * Produces a key for a given node.
         * Nodes with the same children keys
         * and same data field value always
         * have the same key. Also a key of
         * a node with all children keys
         * equal to 0 and nullptr data value
         * is also 0.
         * @param node HashOctree node object to be encrypted
         * @return key produced from the node object
         */
        static key_t encrypt(const Node &node);

        /**
         * Produces a key for a given node.
         * Nodes with the same children keys
         * and same data field value always
         * have the same key. Also a key of
         * a node with all children keys
         * equal to 0 and nullptr data value
         * is also 0.
         * @param children children key values of the node object
         * @param data data field value of the node object
         * @return key produced from the node object
         */
        static key_t encrypt(const key_t *children, const void *data);

        /**
         * Produces a key for a given node.
         * Nodes with the same children keys
         * and same data field value always
         * have the same key. Also a key of
         * a node with all children keys
         * equal to 0 and nullptr data value
         * is also 0.
         * @param data data field value of the node object
         * @return key produced from the node object with
         * @p data and children keys equal to 0.
         */
        static key_t encrypt(const void *data);
    };

}

#endif //HASH_OCTREE_ENCRYPTOR_H
