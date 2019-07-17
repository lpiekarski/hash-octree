#include <sstream>
#include <iomanip>
#include <iostream>

#include "../include/hash-octree.h"
#include "../include/encryptor.h"

namespace HashOctree {

    void HashOctree::init(const dim_t *origin, const dim_t *halfDim, const dim_t *precision) {
        this->origin[0] = origin[0];
        this->origin[1] = origin[1];
        this->origin[2] = origin[2];

        this->halfDim[0] = halfDim[0];
        this->halfDim[1] = halfDim[1];
        this->halfDim[2] = halfDim[2];

        this->precision[0] = precision[0];
        this->precision[1] = precision[1];
        this->precision[2] = precision[2];

        this->nodes[0].key = 0;
        this->nodes[0].node = Node();
        this->nodes[0].refs = 9;

        this->root = 0;
    }

    HashOctree::HashOctree(const dim_t *origin, const dim_t *halfDim, const dim_t *precision) {
        this->init(origin, halfDim, precision);
    }

    HashOctree::HashOctree() {
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

    dim_t HashOctree::getX() const { return this->origin[0]; }

    dim_t HashOctree::getY() const { return this->origin[1]; }

    dim_t HashOctree::getZ() const { return this->origin[2]; }

    dim_t HashOctree::getHalfWidth() const { return this->halfDim[0]; }

    dim_t HashOctree::getHalfHeight() const { return this->halfDim[1]; }

    dim_t HashOctree::getHalfDepth() const { return this->halfDim[2]; }

    status_t HashOctree::remove(const NodeControlBlock &ncb, int flags) {
        if (this->nodes.count(ncb.key) == 0)
            return NODE_DOESNT_EXIST;
        if (ncb.key == this->root && !(flags & FL_FORCE))
            return NODE_IS_ROOT;
        if (ncb.refs != 0 && !(flags & FL_IGNORE_REFS) && !(flags & FL_FORCE))
            return NODE_HAS_REFS;

        for (size_t i = 0; i < 8; i++) {
            if (this->nodes.count(ncb.node.children[i]) == 0)
                return CHILD_DOESNT_EXIST;
        }

        for (size_t i = 0; i < 8; i++) {
            this->nodes[ncb.node.children[i]].refs--;
            if (flags & FL_REC)
                this->remove(ncb.node.children[i], flags);
        }
        this->nodes.erase(ncb.key);

        return OK;
    }

    status_t HashOctree::remove(const Node &n, int flags) {
        key_t nkey = Encryptor::encrypt(n);
        if (this->nodes.count(nkey) == 0)
            return NODE_DOESNT_EXIST;

        return this->remove(this->nodes[nkey], flags);
    }

    status_t HashOctree::remove(key_t nkey, int flags) {
        if (this->nodes.count(nkey) == 0)
            return NODE_DOESNT_EXIST;

        return this->remove(this->nodes[nkey], flags);
    }

    status_t HashOctree::create(const Node &n, key_t *res, int flags) {
        key_t nkey;

        for (size_t i = 0; i < 8; i++) {
            if (this->nodes.count(n.children[i]) == 0)
                return CHILD_DOESNT_EXIST;
        }

        nkey = Encryptor::encrypt(n);

        if (this->nodes.count(nkey) != 0) {
            const NodeControlBlock &ncb = this->nodes[nkey];

            if (res != nullptr)
                (*res) = nkey;

            if (ncb.node.data == n.data &&
                ncb.node.children[0] == n.children[0] &&
                ncb.node.children[1] == n.children[1] &&
                ncb.node.children[2] == n.children[2] &&
                ncb.node.children[3] == n.children[3] &&
                ncb.node.children[4] == n.children[4] &&
                ncb.node.children[5] == n.children[5] &&
                ncb.node.children[6] == n.children[6] &&
                ncb.node.children[7] == n.children[7])
                return NODE_EXISTS;
            return KEY_COLLISION;
        }

        NodeControlBlock &ncb = this->nodes[nkey];
        ncb.node = n;
        ncb.refs = 0;
        ncb.key = nkey;

        for (size_t i = 0; i < 8; i++)
            this->nodes[n.children[i]].refs++;

        if (res != nullptr)
            (*res) = nkey;
        return OK;
    }

    status_t HashOctree::create(const key_t *children, const void *data, key_t *res, int flags) {
        return this->create(Node(children, data), res, flags);
    }

    status_t HashOctree::create(const void *data, key_t *res, int flags) {
        key_t empty_children[8];
        for (size_t i = 0; i < 8; i++)
            empty_children[i] = 0;

        return this->create(empty_children, data, res, flags);
    }

    status_t HashOctree::changeRoot(key_t newRoot) {
        if (this->nodes.count(newRoot) == 0)
            return NODE_DOESNT_EXIST;
        this->nodes[this->root].refs--;
        this->nodes[newRoot].refs++;
        key_t oldRoot = this->root;
        this->root = newRoot;
        this->remove(oldRoot, FL_REC);

        return OK;
    }

    status_t HashOctree::resize(dim_t halfWidth, dim_t halfHeight, dim_t halfDepth) {
        this->halfDim[0] = halfWidth;
        this->halfDim[1] = halfHeight;
        this->halfDim[2] = halfDepth;

        return OK;
    }

    status_t HashOctree::reposition(dim_t x, dim_t y, dim_t z) {
        this->origin[0] = x;
        this->origin[1] = y;
        this->origin[2] = z;

        return OK;
    }

    status_t HashOctree::addDataPoint(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data) {
        NodeOperationBlock rootNob;
        rootNob.parent = nullptr;
        rootNob.ncb = &this->nodes[this->root];
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

    status_t HashOctree::addDataPointRec(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data,
            const NodeOperationBlock &curr, key_t *out_key) {
        status_t ret = OK;

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

        // add data point recursively
        key_t children[8];
        for (int i = 0; i < 8; i++) {
            NodeOperationBlock childNob;
            childNob.ncb = &this->nodes[curr.ncb->node.children[i]];
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
                return ret;
            }
        }

        // if all children store the same data we can merge them
        /*bool hasSameData = true;
        void *sameData = this->nodes[children[0]].node.data;
        for (int i = 1; i < 8; i++) {
            if (this->nodes[children[i]].node.data != sameData) {
                hasSameData = false;
                break;
            }
        }
        if (hasSameData && sameData != nullptr) {
            key_t key;
            this->create(sameData, &key, 0);
            return key;
        }*/

        key_t key;
        ret = this->create(children, nullptr, &key);
        if (out_key != nullptr)
            (*out_key) = key;
        return ret;
    }

    status_t HashOctree::recountRefs() {
        for (auto &kv : this->nodes)
            kv.second.refs = 0;


        for (auto &kv : this->nodes)
            for (int i = 0; i < 8; i++)
                if (this->nodes.count(kv.second.node.children[i]) == 0)
                    return CHILD_DOESNT_EXIST;

        this->nodes[this->root].refs++;
        for (auto &kv : this->nodes)
            for (int i = 0; i < 8; i++)
                this->nodes[kv.second.node.children[i]].refs++;

        return OK;
    }

}