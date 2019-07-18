#ifndef HASH_OCTREE_HASH_OCTREE_H
#define HASH_OCTREE_HASH_OCTREE_H

#include <cstdint>
#include <type_traits>

#include "node.h"
#include "lookup-methods.h"
#include "encryptor.h"

namespace HashOctree {

    template <typename LM=UnorderedMapLookupMethod,
            typename E=std::enable_if_t<std::is_base_of_v<LookupMethod, LM>>>
    class HashOctree {

        friend class Exporter;

        friend class Importer;
    private:
        LM lookupMethod;
        key_t root;
        dim_t origin[3];
        dim_t halfDim[3];
        dim_t precision[3];

        /// constructor content function
        void init(const dim_t *origin, const dim_t *halfDim, const dim_t *precision) {
            this->origin[0] = origin[0];
            this->origin[1] = origin[1];
            this->origin[2] = origin[2];

            this->halfDim[0] = halfDim[0];
            this->halfDim[1] = halfDim[1];
            this->halfDim[2] = halfDim[2];

            this->precision[0] = precision[0];
            this->precision[1] = precision[1];
            this->precision[2] = precision[2];

            NodeControlBlock ncb0;
            ncb0.refs = 9;
            ncb0.key = 0;
            ncb0.node = Node();

            lookupMethod.insert(0, ncb0);

            this->root = 0;
        }

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
                NodeOperationBlock &curr, key_t *out_key) {
            status_t ret = OK;
            key_t cleanupKey = 0;

            // if data point fully contains node dont split
            if (x - hw <= curr.origin[0] - curr.halfDim[0] &&
                curr.origin[0] + curr.halfDim[0] <= x + hw &&
                y - hh <= curr.origin[1] - curr.halfDim[1] &&
                curr.origin[1] + curr.halfDim[1] <= y + hh &&
                z - hd <= curr.origin[2] - curr.halfDim[2] &&
                curr.origin[2] + curr.halfDim[2] <= z + hd) {
                key_t key;
                ret = this->create(data, &key);
                if (out_key != nullptr)
                    (*out_key) = key;
                return ret;
            }

            // if data point doesnt have any intersection with node dont do anything
            if ((x + hw <= curr.origin[0] - curr.halfDim[0] || curr.origin[0] + curr.halfDim[0] <= x - hw) ||
                (y + hh <= curr.origin[1] - curr.halfDim[1] || curr.origin[1] + curr.halfDim[1] <= y - hh) ||
                (z + hd <= curr.origin[2] - curr.halfDim[2] || curr.origin[2] + curr.halfDim[2] <= z - hd)) {
                if (out_key != nullptr)
                    (*out_key) = curr.ncb->key;
                return ret;
            }

            // precision bailout
            if (curr.halfDim[0] < this->precision[0] ||
                curr.halfDim[1] < this->precision[1] ||
                curr.halfDim[2] < this->precision[2]) {
                key_t key;
                ret = this->create(data, &key);
                if (out_key != nullptr)
                    (*out_key) = key;
                return ret;
            }

            // if there is any data in this node, it has to be pushed down the tree
            if (curr.ncb->node.data != nullptr) {
                key_t key;
                key_t unmerge_children[8];
                for (int i = 0; i < 8; i++)
                    unmerge_children[i] = curr.ncb->key;

                ret = this->create(unmerge_children, nullptr, &key);

                // can't create a new node for curr
                if (ret != OK && ret != NODE_EXISTS)
                    return ret;

                // move into a new path in octree
                // we have to mark the new node for cleanup since it is outside
                // of the main root-leaf path and can become unused
                // after coming back from children
                curr.ncb = &lookupMethod.lookup(key);
                cleanupKey = key;
            }

            // add data point recursively
            key_t children[8];
            for (int i = 0; i < 8; i++) {
                NodeOperationBlock childNob;
                childNob.ncb = &lookupMethod.lookup(curr.ncb->node.children[i]);
                childNob.parent = curr.ncb;
                childNob.halfDim[0] = curr.halfDim[0] * 0.5;
                childNob.halfDim[1] = curr.halfDim[1] * 0.5;
                childNob.halfDim[2] = curr.halfDim[2] * 0.5;
                childNob.origin[0] = curr.origin[0] + (2 * (i % 2) - 1) * childNob.halfDim[0];
                childNob.origin[1] = curr.origin[1] + (2 * ((i / 2) % 2) - 1) * childNob.halfDim[1];
                childNob.origin[2] = curr.origin[2] + (2 * (i / 4) - 1) * childNob.halfDim[2];
                status_t childStatus = this->addDataPointRec(x, y, z, hw, hh, hd, data, childNob, &children[i]);

                if (childStatus != OK && childStatus != NODE_EXISTS) {
                    ret = childStatus;
                    if (cleanupKey != 0)
                        this->remove(cleanupKey, FL_REC);
                    return ret;
                }
            }

            // if all children store the same data we can merge them
            bool hasSameData = true;
            void *sameData = lookupMethod.lookup(children[0]).node.data;
            for (int i = 1; i < 8; i++) {
                if (lookupMethod.lookup(children[i]).node.data != sameData) {
                    hasSameData = false;
                    break;
                }
            }
            if (hasSameData && sameData != nullptr) {
                key_t key;
                ret = this->create(sameData, &key, 0);
                if (out_key != nullptr)
                    (*out_key) = key;
                if (cleanupKey != 0)
                    this->remove(cleanupKey, FL_REC);
                return ret;
            }

            key_t key;
            ret = this->create(children, nullptr, &key);
            if (out_key != nullptr)
                (*out_key) = key;
            if (cleanupKey != 0)
                this->remove(cleanupKey, FL_REC);
            return ret;
        }

    public:
        HashOctree() {
            dim_t def_origin[3];
            dim_t def_halfDim[3];
            dim_t def_precision[3];

            def_origin[0] = 0;
            def_origin[1] = 0;
            def_origin[2] = 0;

            def_halfDim[0] = 32;
            def_halfDim[1] = 32;
            def_halfDim[2] = 32;


            def_precision[0] = 0.5;
            def_precision[1] = 0.5;
            def_precision[2] = 0.5;

            this->init(def_origin, def_halfDim, def_precision);
        }

        HashOctree(const dim_t *origin, const dim_t *halfDim, const dim_t *precision) {
            this->init(origin, halfDim, precision);
        }

        dim_t getX() const { return this->origin[0]; }

        dim_t getY() const { return this->origin[1]; }

        dim_t getZ() const { return this->origin[2]; }

        dim_t getHalfWidth() const { return this->halfDim[0]; }

        dim_t getHalfHeight() const { return this->halfDim[1]; }

        dim_t getHalfDepth() const { return this->halfDim[2]; }

        status_t remove(const NodeControlBlock &ncb, int flags=0) {
            if (!lookupMethod.contains(ncb.key))
                return NODE_DOESNT_EXIST;
            if (ncb.key == this->root && !(flags & FL_FORCE))
                return NODE_IS_ROOT;
            if (ncb.refs != 0 && !(flags & FL_IGNORE_REFS) && !(flags & FL_FORCE))
                return NODE_HAS_REFS;

            for (size_t i = 0; i < 8; i++) {
                if (!lookupMethod.contains(ncb.node.children[i]))
                    return CHILD_DOESNT_EXIST;
            }

            for (size_t i = 0; i < 8; i++) {
                lookupMethod.lookup(ncb.node.children[i]).refs--;
                if (flags & FL_REC)
                    this->remove(ncb.node.children[i], flags);
            }

            lookupMethod.erase(ncb.key);

            return OK;
        }

        status_t remove(const Node &n, int flags=0) {
            key_t nkey = Encryptor::encrypt(n);
            if (!lookupMethod.contains(nkey))
                return NODE_DOESNT_EXIST;

            return this->remove(this->nodes[nkey], flags);
        }

        status_t remove(key_t nkey, int flags=0) {
            if (!lookupMethod.contains(nkey))
                return NODE_DOESNT_EXIST;

            return this->remove(lookupMethod.lookup(nkey), flags);
        }

        status_t create(const Node &n, key_t *res, int flags=0) {
            key_t nkey;

            for (size_t i = 0; i < 8; i++) {
                if (!lookupMethod.contains(n.children[i]))
                    return CHILD_DOESNT_EXIST;
            }

            nkey = Encryptor::encrypt(n);

            if (lookupMethod.contains(nkey)) {
                const NodeControlBlock &ncb = lookupMethod.lookup(nkey);

                if (res != nullptr)
                    (*res) = nkey;

                if (ncb.node == n)
                    return NODE_EXISTS;
                return KEY_COLLISION;
            }

            NodeControlBlock ncb;
            ncb.node = n;
            ncb.refs = 0;
            ncb.key = nkey;

            lookupMethod.insert(nkey, ncb);

            for (size_t i = 0; i < 8; i++)
                lookupMethod.lookup(n.children[i]).refs++;

            if (res != nullptr)
                (*res) = nkey;
            return OK;
        }

        status_t create(const key_t *children, const void *data, key_t *res, int flags=0) {
            return this->create(Node(children, data), res, flags);
        }

        status_t create(const void *data, key_t *res, int flags=0) {
            key_t empty_children[8];
            for (size_t i = 0; i < 8; i++)
                empty_children[i] = 0;

            return this->create(empty_children, data, res, flags);
        }

        status_t changeRoot(key_t newRoot) {
            if (!lookupMethod.contains(newRoot))
                return NODE_DOESNT_EXIST;
            lookupMethod.lookup(this->root).refs--;
            lookupMethod.lookup(newRoot).refs++;
            key_t oldRoot = this->root;
            this->root = newRoot;
            this->remove(oldRoot, FL_REC);

            return OK;
        }

        status_t resize(dim_t halfWidth, dim_t halfHeight, dim_t halfDepth) {
            this->halfDim[0] = halfWidth;
            this->halfDim[1] = halfHeight;
            this->halfDim[2] = halfDepth;

            return OK;
        }

        status_t reposition(dim_t x, dim_t y, dim_t z) {
            this->origin[0] = x;
            this->origin[1] = y;
            this->origin[2] = z;

            return OK;
        }

        status_t addDataPoint(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data) {
            NodeOperationBlock rootNob;
            rootNob.parent = nullptr;
            rootNob.ncb = &lookupMethod.lookup(this->root);
            rootNob.origin[0] = this->origin[0];
            rootNob.origin[1] = this->origin[1];
            rootNob.origin[2] = this->origin[2];
            rootNob.halfDim[0] = this->halfDim[0];
            rootNob.halfDim[1] = this->halfDim[1];
            rootNob.halfDim[2] = this->halfDim[2];

            key_t newRoot;
            status_t status = this->addDataPointRec(x, y, z, hw, hh, hd, data, rootNob, &newRoot);

            if (status != OK && status != NODE_EXISTS)
                return status;

            this->changeRoot(newRoot);
            return status;
        }

        status_t recountRefs() {
            auto kvList = lookupMethod.list();

            for (auto &kv : kvList)
                for (int i = 0; i < 8; i++)
                    if (!lookupMethod.contains(kv.second.node.children[i]))
                        return CHILD_DOESNT_EXIST;

            for (auto &kv : kvList)
                kv.second.refs = 0;

            lookupMethod.lookup(this->root).refs++;

            for (auto &kv : kvList)
                for (int i = 0; i < 8; i++)
                    lookupMethod.lookup(kv.second.node.children[i]).refs++;

            return OK;
        }
    };

}

#endif //HASH_OCTREE_HASH_OCTREE_H
