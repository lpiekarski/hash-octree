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
            ret += (children[i] * p);
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
        NodeOperationBlock rootNop;
        rootNop.parent = nullptr;
        rootNop.ncb = &this->nodes[this->root];
        rootNop.origin[0] = this->origin[0];
        rootNop.origin[1] = this->origin[1];
        rootNop.origin[2] = this->origin[2];
        rootNop.halfDim[0] = this->halfDim[0];
        rootNop.halfDim[1] = this->halfDim[1];
        rootNop.halfDim[2] = this->halfDim[2];

        this->changeRoot(this->addDataPointRec(x, y, z, hw, hh, hd, data, rootNop));

        return OK;
    }

    key_t HashOctree::addDataPointRec(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data,
            const NodeOperationBlock &curr) {
        // if data point fully contains node dont split
        if (x - hw <= curr.origin[0] - curr.halfDim[0] &&
            curr.origin[0] + curr.halfDim[0] < x + hw &&
            y - hh <= curr.origin[1] - curr.halfDim[1] &&
            curr.origin[1] + curr.halfDim[1] < y + hh &&
            z - hd <= curr.origin[2] - curr.halfDim[2] &&
            curr.origin[2] + curr.halfDim[2] < z + hd) {
            key_t key;
            this->create(data, &key, 0);
            return key;
        }

        // if data point doesnt have any intersection with node dont do anything
        if ((x + hw < curr.origin[0] - curr.halfDim[0] || curr.origin[0] + curr.halfDim[0] < x - hw) &&
            (y + hh < curr.origin[1] - curr.halfDim[1] || curr.origin[1] + curr.halfDim[1] < y - hh) &&
            (z + hd < curr.origin[2] - curr.halfDim[2] || curr.origin[2] + curr.halfDim[2] < z - hd)) {
            return curr.ncb->key;
        }

        // precision bailout
        if (curr.halfDim[0] < this->precision[0] ||
            curr.halfDim[1] < this->precision[1] ||
            curr.halfDim[2] < this->precision[2]) {
            key_t key;
            this->create(data, &key, 0);
            return key;
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
            children[i] = this->addDataPointRec(x, y, z, hw, hh, hd, data, childNob);
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
        this->create(children, nullptr, &key, 0);
        return key;
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

    std::string Exporter::toJson(const HashOctree &ho, size_t indentWidth, bool newLines) {
        std::stringstream ss("");
        size_t indents = 0;
        char newLine;
        if (newLines)
            newLine = '\n';
        else
            newLine = '\0';
        char space;
        if (newLines)
            space = ' ';
        else
            space = '\0';
        ss << "{" << newLine;
        indents++;
        for (size_t i = 0; i < indents * indentWidth; i++)
            ss << space;
        ss << "\"root\":" << space << ho.root << "," << newLine;

        for (size_t i = 0; i < indents * indentWidth; i++)
            ss << " ";
        ss << "\"nodes\":" << space << "[" << newLine;
        indents++;
        bool first = true;
        for (const auto &kv : ho.nodes) {
            if (!first) {
                ss << "," << newLine;
            }
            else {
                first = false;
            }
            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "{" << newLine;
            indents++;

            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "\"key\":" << space << kv.first << "," << newLine;

            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "\"data\":" << space << (uint64_t)kv.second.node.data << "," << newLine;

            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "\"children\":" << space << "[" << newLine;
            indents++;

            for (size_t i = 0; i < 8; i++) {
                for (size_t j = 0; j < indents * indentWidth; j++)
                    ss << space;

                ss << kv.second.node.children[i];
                if (i != 7)
                    ss << "," << newLine;
                else
                    ss << newLine;
            }
            indents--;
            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "]" << newLine;

            indents--;
            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "}";

        }
        ss << newLine;
        indents--;
        for (size_t i = 0; i < indents * indentWidth; i++)
            ss << space;
        ss << "]" << newLine;

        indents--;
        for (size_t i = 0; i < indents * indentWidth; i++)
            ss << space;
        ss << "}" << newLine;

        return ss.str();
    }

    void push_uint64_t(std::vector<char> &v, uint64_t val) {
        size_t bytes = 8;
        while (bytes--) {
            v.push_back(val & 255);
            val = val >> 8;
        }
    }

    uint64_t read_uint64_t(const std::vector<char> &v, size_t pos) {
        size_t bytes = 8;
        uint64_t ret = 0;
        while (bytes--) {
            ret = ret << 8;
            ret += (unsigned char)v[pos + bytes];
        }
        return ret;
    }

    std::vector<char> Exporter::toByteArray(const HashOctree &ho) {
        std::vector<char> ret;
        push_uint64_t(ret, ho.root);

        for (const auto &kv : ho.nodes) {
            push_uint64_t(ret, kv.first);
            push_uint64_t(ret, (uint64_t)kv.second.node.data);
            for (int i = 0; i < 8; i++)
                push_uint64_t(ret, kv.second.node.children[i]);
        }

        return ret;
    }

    HashOctree Importer::fromByteArray(const std::vector<char> &array) {
        HashOctree ret;
        size_t read_id = 0;
        ret.root = read_uint64_t(array, read_id);
        read_id += 8;

        while (read_id < array.size()) {
            uint64_t key = read_uint64_t(array, read_id);
            read_id += 8;
            uint64_t data = read_uint64_t(array, read_id);
            read_id += 8;
            uint64_t children[8];
            for (int i = 0; i < 8; i++) {
                children[i] = read_uint64_t(array, read_id);
                read_id += 8;
            }
            ret.nodes[key].key = key;
            ret.nodes[key].node.data = (void *)data;
            for (int i = 0; i < 8; i++)
                ret.nodes[key].node.children[i] = children[i];
        }

        ret.recountRefs();

        return ret;
    }

}