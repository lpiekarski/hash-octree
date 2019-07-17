#ifndef HASH_OCTREE_HASH_OCTREE_H
#define HASH_OCTREE_HASH_OCTREE_H

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "node.h"

namespace HashOctree {

    class HashOctree {
        friend class Exporter;
        friend class Importer;
    private:
        std::unordered_map<key_t, NodeControlBlock> nodes;
        key_t root;
        dim_t origin[3];
        dim_t halfDim[3];
        dim_t precision[3];

        /// constructor content function
        void init(const dim_t *origin, const dim_t *halfDim, const dim_t *precision);

        /**
         * Parameters hiding function for addDataPoint.
         * @param x
         * @param y
         * @param z
         * @param hw
         * @param hh
         * @param hd
         * @param data
         * @param curr
         * @return
         */
        status_t addDataPointRec(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data,
                const NodeOperationBlock &curr, key_t *out_key);

    public:
        HashOctree();

        HashOctree(const dim_t *origin, const dim_t *halfDim, const dim_t *precision);

        dim_t getX() const;

        dim_t getY() const;

        dim_t getZ() const;

        dim_t getHalfWidth() const;

        dim_t getHalfHeight() const;

        dim_t getHalfDepth() const;

        status_t remove(const NodeControlBlock &ncb, int flags=0);

        status_t remove(const Node &n, int flags=0);

        status_t remove(key_t nkey, int flags=0);

        status_t create(const Node &n, key_t *res, int flags=0);

        status_t create(const key_t *children, const void *data, key_t *res, int flags=0);

        status_t create(const void *data, key_t *res, int flags=0);

        status_t changeRoot(key_t newRoot);

        status_t resize(dim_t halfWidth, dim_t halfHeight, dim_t halfDepth);

        status_t reposition(dim_t x, dim_t y, dim_t z);

        status_t addDataPoint(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data);

        status_t recountRefs();
    };

}

#endif //HASH_OCTREE_HASH_OCTREE_H
