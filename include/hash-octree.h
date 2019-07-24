#ifndef HASH_OCTREE_HASH_OCTREE_H
#define HASH_OCTREE_HASH_OCTREE_H

#include <cstdint>
#include <type_traits>
#include <functional>

#include "node.h"
#include "lookup-methods/unordered-map-lookup-method.h"
#include "encryptor.h"

namespace HashOctree {

    template <typename LM=UnorderedMapLookupMethod,
            typename = typename std::enable_if<std::is_base_of<LookupMethod, LM>::value>::type>
    class HashOctree {
        friend class Exporter;
        friend class Importer;
    private:
        LM lookupMethod;
        key_t root;
        NodeDims dim;
        dim_t precision[3];

        /// constructor content function
        void init(const NodeDims &dim, const dim_t *precision) {
            this->dim = dim;

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

        status_t addDataShapeRec(std::function<bool(const NodeDims&)> intersects, std::function<bool(const NodeDims&)> contains, std::function<data_t(const NodeDims &dims)> dataFunc,
                                 NodeOperationBlock &curr, key_t *out_key) {
            status_t ret = OK;
            key_t cleanupKey = 0;

            // if data point fully contains node dont split
            if (contains(curr.dim)) {
                key_t key;
                ret = this->create(dataFunc(curr.dim), &key);
                if (out_key != nullptr)
                    (*out_key) = key;
                return ret;
            }

            // if data point doesnt have any intersection with node dont do anything
            if (!intersects(curr.dim)) {
                if (out_key != nullptr)
                    (*out_key) = curr.ncb->key;
                return ret;
            }

            // precision bailout
            if (curr.dim.halfDim[0] < this->precision[0] ||
                curr.dim.halfDim[1] < this->precision[1] ||
                curr.dim.halfDim[2] < this->precision[2]) {
                key_t key;
                ret = this->create(dataFunc(curr.dim), &key);
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
                if ((ret = lookupMethod.lookup(key, &curr.ncb)) != OK)
                    return ret;
                cleanupKey = key;
            }

            // add data point recursively
            key_t children[8];
            for (int i = 0; i < 8; i++) {
                NodeOperationBlock childNob = curr.getChildNOB<LM>(i, lookupMethod);
                status_t childStatus = this->addDataShapeRec(intersects, contains, dataFunc, childNob, &children[i]);

                if (childStatus != OK && childStatus != NODE_EXISTS) {
                    ret = childStatus;
                    if (cleanupKey != 0)
                        this->remove(cleanupKey, FL_REC);
                    return ret;
                }
            }

            // if all children store the same data we can merge them
            bool hasSameData = true;
            NodeControlBlock *ch0Ncb;
            if ((ret = lookupMethod.lookup(children[0], &ch0Ncb)) != OK)
                return ret;
            data_t sameData = ch0Ncb->node.data;
            for (int i = 1; i < 8; i++) {
                NodeControlBlock *chNcb;
                if ((ret = lookupMethod.lookup(children[i], &chNcb)) != OK)
                    return ret;
                if (chNcb->node.data != sameData) {
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
            dim_t def_precision[3];

            def_precision[0] = 0.5;
            def_precision[1] = 0.5;
            def_precision[2] = 0.5;

            this->init(NodeDims(0, 0, 0, 32, 32, 32), def_precision);
        }

        HashOctree(const NodeDims &dim, const dim_t *precision) {
            this->init(dim, precision);
        }

        dim_t getX() const { return this->dim.origin[0]; }

        dim_t getY() const { return this->dim.origin[1]; }

        dim_t getZ() const { return this->dim.origin[2]; }

        dim_t getHalfWidth() const { return this->dim.halfDim[0]; }

        dim_t getHalfHeight() const { return this->dim.halfDim[1]; }

        dim_t getHalfDepth() const { return this->dim.halfDim[2]; }

        status_t remove(const NodeControlBlock &ncb, int flags=0) {
            status_t ret = OK;
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
                NodeControlBlock *chNcb;
                if ((ret = lookupMethod.lookup(ncb.node.children[i], &chNcb)) != OK)
                    return ret;
                chNcb->refs--;
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
            status_t ret = OK;
            if (!lookupMethod.contains(nkey))
                return NODE_DOESNT_EXIST;

            NodeControlBlock *ncb;
            if ((ret = lookupMethod.lookup(nkey, &ncb)) != OK)
                return ret;

            return this->remove(*ncb, flags);
        }

        status_t create(const Node &n, key_t *res, int flags=0) {
            key_t nkey;
            status_t ret = OK;

            for (size_t i = 0; i < 8; i++) {
                if (!lookupMethod.contains(n.children[i]))
                    return CHILD_DOESNT_EXIST;
            }

            nkey = Encryptor::encrypt(n);

            if (lookupMethod.contains(nkey)) {
                NodeControlBlock *ncb;
                if ((ret = lookupMethod.lookup(nkey, &ncb)) != OK)
                    return ret;

                if (res != nullptr)
                    (*res) = nkey;

                if (ncb->node == n)
                    return NODE_EXISTS;
                return KEY_COLLISION;
            }

            NodeControlBlock ncb;
            ncb.node = n;
            ncb.refs = 0;
            ncb.key = nkey;

            if ((ret = lookupMethod.insert(nkey, ncb)) != OK)
                return ret;

            NodeControlBlock *chNcb;
            for (size_t i = 0; i < 8; i++) {
                if ((ret = lookupMethod.lookup(n.children[i], &chNcb)) != OK)
                chNcb->refs++;
            }

            if (res != nullptr)
                (*res) = nkey;
            return OK;
        }

        status_t create(const key_t *children, const data_t data, key_t *res, int flags=0) {
            return this->create(Node(children, data), res, flags);
        }

        status_t create(const data_t data, key_t *res, int flags=0) {
            key_t empty_children[8];
            for (size_t i = 0; i < 8; i++)
                empty_children[i] = 0;

            return this->create(empty_children, data, res, flags);
        }

        status_t changeRoot(key_t newRoot) {
            status_t ret = OK;
            if (!lookupMethod.contains(newRoot))
                return NODE_DOESNT_EXIST;
            NodeControlBlock *ncb;
            if ((ret = lookupMethod.lookup(this->root, &ncb)) != OK)
                return ret;
            ncb->refs--;
            if ((ret = lookupMethod.lookup(newRoot, &ncb)) != OK)
                return ret;
            ncb->refs++;
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

        status_t addDataPoint(const NodeDims &dims, data_t data) {
            status_t ret = OK;
            NodeOperationBlock rootNob;
            rootNob.parent = nullptr;
            if ((ret = lookupMethod.lookup(this->root, &rootNob.ncb)) != OK)
                return ret;
            rootNob.dim = this->dim;

            key_t newRoot;

            auto intersects = [&dims](const NodeDims &x) { return dims.intersects(x); };
            auto contains = [&dims](const NodeDims &x) { return dims.contains(x); };
            auto dataFunc = [data](const NodeDims &x) { return data; };

            status_t status = this->addDataShapeRec(intersects, contains, dataFunc, rootNob, &newRoot);

            if (status != OK && status != NODE_EXISTS)
                return status;

            this->changeRoot(newRoot);
            return status;
        }

        status_t recountRefs() {
            auto kvList = lookupMethod.list();
            status_t ret = OK;

            for (auto &kv : kvList)
                for (int i = 0; i < 8; i++)
                    if (!lookupMethod.contains(kv.second.node.children[i]))
                        return CHILD_DOESNT_EXIST;

            for (auto &kv : kvList)
                kv.second.refs = 0;

            NodeControlBlock *ncb;
            if ((ret = lookupMethod.lookup(this->root, &ncb)) != OK)
                return ret;
            ncb->refs++;

            for (auto &kv : kvList) {
                for (int i = 0; i < 8; i++) {
                    if ((ret = lookupMethod.lookup(kv.second.node.children[i], &ncb)) != OK)
                        return ret;
                    ncb->refs++;
                }
            }

            return OK;
        }

        status_t addDataShape(std::function<bool(const NodeDims&)> intersects, std::function<bool(const NodeDims&)> contains, std::function<data_t(const NodeDims &dims)> dataFunc) {
            status_t ret = OK;
            NodeOperationBlock rootNob;
            rootNob.parent = nullptr;
            if ((ret = lookupMethod.lookup(this->root, &rootNob.ncb)) != OK)
                return ret;
            rootNob.dim = this->dim;

            key_t newRoot;

            status_t status = this->addDataShapeRec(intersects, contains, dataFunc, rootNob, &newRoot);

            if (status != OK && status != NODE_EXISTS)
                return status;

            this->changeRoot(newRoot);
            return status;
        }

    };

}

#endif //HASH_OCTREE_HASH_OCTREE_H
