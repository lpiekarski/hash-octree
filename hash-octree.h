#ifndef HASH_OCTREE_HASH_OCTREE_H
#define HASH_OCTREE_HASH_OCTREE_H

#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <vector>

namespace HashOctree {

    using key_t = uint64_t;
    using dim_t = double;
    using status_t = int64_t;

    const status_t INDEX_OUT_OF_BOUNDS = -7;
    const status_t KEY_COLLISION       = -6;
    const status_t CHILD_DOESNT_EXIST  = -5;
    const status_t NODE_DOESNT_EXIST   = -4;
    const status_t NODE_IS_ROOT        = -3;
    const status_t NODE_HAS_REFS       = -2;
    const status_t NODE_EXISTS         = -1;
    const status_t OK                  = 0;
    const status_t NODE_REPLACED       = 1;

    const int      FL_REC              = 1;
    const int      FL_IGNORE_REFS      = 2;
    const int      FL_FORCE            = 4;

    class Node {
    public:
        key_t children[8];
        void *data;

        Node() = default;

        Node(const key_t *children, const void *data);
    };

    class Encryptor {
    public:
        static key_t encrypt(const Node &node);

        static key_t encrypt(const key_t *children, const void *data);

        static key_t encrypt(const void *data);
    };

    class NodeControlBlock {
    public:
        key_t key;
        Node node;
        int32_t refs;
    };

    class NodeOperationBlock {
    public:
        NodeControlBlock *ncb;
        dim_t origin[3];
        dim_t halfDim[3];
        NodeControlBlock *parent;
    };

    class HashOctree {
        friend class Exporter;
        friend class Importer;
        friend class Debugger;
    private:
        std::unordered_map<key_t, NodeControlBlock> nodes;
        key_t root;
        dim_t origin[3];
        dim_t halfDim[3];
        dim_t precision[3];

        void init(const dim_t *origin, const dim_t *halfDim, const dim_t *precision);

        key_t addDataPointRec(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data,
                const NodeOperationBlock &curr);

    public:
        HashOctree();

        HashOctree(const dim_t *origin, const dim_t *halfDim, const dim_t *precision);

        status_t remove(const NodeControlBlock &ncb, int flags);

        status_t remove(const Node &n, int flags);

        status_t remove(key_t nkey, int flags);

        status_t create(const Node &n, key_t *res, int flags);

        status_t create(const key_t *children, const void *data, key_t *res, int flags);

        status_t create(const void *data, key_t *res, int flags);

        status_t changeRoot(key_t newRoot);

        status_t resize(dim_t halfWidth, dim_t halfHeight, dim_t halfDepth);

        status_t reposition(dim_t x, dim_t y, dim_t z);

        status_t addDataPoint(dim_t x, dim_t y, dim_t z, dim_t hw, dim_t hh, dim_t hd, void *data);

        status_t recountRefs();
    };

    class Exporter {
    public:
        static std::string toString(const HashOctree &ho);

        static std::vector<char> toByteArray(const HashOctree &ho);

        static std::string toJson(const HashOctree &ho, size_t indentWidth, bool newLines);
    };

    class Importer {
    public:
        static HashOctree fromByteArray(const std::vector<char> &array);

        //static HashOctree fromJson(const std::string &str);
    };

    class Debugger {
    public:
        //static bool hasNoRefNodes(const HashOctree &ho);
    };
}

#endif //HASH_OCTREE_HASH_OCTREE_H
