#include <sstream>
#include <iomanip>
#include <iostream>

#include "hash-octree.h"

#define P (32416187567)
#define Q (1610612741)

namespace HashOctree {

    Node::Node(const key_t *children, const void *data) {
        this->data = (void *)data;
        for (size_t i = 0; i < 8; i++)
            this->children[i] = children[i];
    }

    key_t Encryptor::encrypt(const key_t *children, const void *data) {
        key_t ret, p;

        p = P;
        ret = (key_t)data * Q;
        for (size_t i = 0; i < 8; i++) {
            ret += children[i] * p;
            p *= P;
        }

        return ret;
    }

    key_t Encryptor::encrypt(const void *data) {
        key_t empty_children[8];
        for (size_t i = 0; i < 8; i++)
            empty_children[i] = 0;
        return Encryptor::encrypt(empty_children, data);
    }

    key_t Encryptor::encrypt(const Node &node) {
        return Encryptor::encrypt(node.children, node.data);
    }

    void HashOctree::init(const dim_t *origin, const dim_t *halfDim) {
        this->origin[0] = origin[0];
        this->origin[1] = origin[1];
        this->origin[2] = origin[2];

        this->halfDim[0] = halfDim[0];
        this->halfDim[1] = halfDim[1];
        this->halfDim[2] = halfDim[2];

        this->nodes[0].key = 0;
        this->nodes[0].node = Node();
        this->nodes[0].refs = 9;

        this->root = 0;
    }

    HashOctree::HashOctree(const dim_t *origin, const dim_t *halfDim) {
        this->init(origin, halfDim);
    }

    HashOctree::HashOctree() {
        dim_t def_origin[3];
        dim_t def_halfDim[3];

        def_origin[0] = 0;
        def_origin[1] = 0;
        def_origin[2] = 0;

        def_halfDim[0] = 32;
        def_halfDim[1] = 32;
        def_halfDim[2] = 32;

        this->init(def_origin, def_halfDim);
    }

    status_t HashOctree::remove(const NodeControlBlock &ncb) {
        if (this->nodes.count(ncb.key) == 0)
            return NODE_DOESNT_EXIST;
        if (ncb.key == this->root)
            return NODE_IS_ROOT;
        if (ncb.refs != 0)
            return NODE_HAS_REFS;

        for (size_t i = 0; i < 8; i++) {
            if (this->nodes.count(ncb.node.children[i]) == 0)
                return CHILD_DOESNT_EXIST;
        }

        for (size_t i = 0; i < 8; i++) {
            this->nodes[ncb.node.children[i]].refs--;
            this->remove(ncb.node.children[i]);
        }
        this->nodes.erase(ncb.key);

        return OK;
    }

    status_t HashOctree::remove(const Node &n) {
        key_t nkey = Encryptor::encrypt(n);
        if (this->nodes.count(nkey) == 0)
            return NODE_DOESNT_EXIST;

        return this->remove(this->nodes[nkey]);
    }

    status_t HashOctree::remove(key_t nkey) {
        if (this->nodes.count(nkey) == 0)
            return NODE_DOESNT_EXIST;

        return this->remove(this->nodes[nkey]);
    }

    status_t HashOctree::create(const Node &n) {
        key_t nkey;

        for (size_t i = 0; i < 8; i++) {
            if (this->nodes.count(n.children[i]) == 0)
                return CHILD_DOESNT_EXIST;
        }

        nkey = Encryptor::encrypt(n);

        if (this->nodes.count(nkey) != 0) {
            const NodeControlBlock &ncb = this->nodes[nkey];
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

        return OK;
    }

    status_t HashOctree::create(const key_t *children, const void *data) {
        return this->create(Node(children, data));
    }

    status_t HashOctree::create(const void *data) {
        key_t empty_children[8];
        for (size_t i = 0; i < 8; i++)
            empty_children[i] = 0;

        return this->create(empty_children, data);
    }

    status_t HashOctree::changeRoot(key_t newRoot) {
        if (this->nodes.count(newRoot) == 0)
            return NODE_DOESNT_EXIST;
        this->nodes[this->root].refs--;
        this->nodes[newRoot].refs++;
        key_t oldRoot = this->root;
        this->root = newRoot;
        this->remove(oldRoot);

        return OK;
    }

    std::string Exporter::toString(const HashOctree &ho) {
        std::stringstream ss("");
        ss.fill('0');
        ss << "root: " << std::setw(16) << std::hex << ho.root << "\n";
        for (const auto &kv : ho.nodes) {
            ss << std::setw(16) << std::hex << kv.first << ": ";
            ss << "data: " << std::setw(16) << std::hex << (uint64_t)kv.second.node.data << ", children: ";
            for (size_t i = 0; i < 8; i++)
                ss << std::setw(16) << std::hex << kv.second.node.children[i] << " ";
            ss << "(" << std::dec << kv.second.refs << " ref(s))\n";
        }

        return ss.str();
    }

}