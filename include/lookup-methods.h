#ifndef HASH_OCTREE_LOOKUP_METHODS_H
#define HASH_OCTREE_LOOKUP_METHODS_H

#include <unordered_map>
#include <vector>

#include "node.h"

namespace HashOctree {

    class LookupMethod {
    public:
        virtual NodeControlBlock &lookup(key_t key) = 0;

        virtual const NodeControlBlock &lookup(key_t key) const = 0;

        virtual bool contains(key_t key) = 0;

        virtual void insert(key_t key, const NodeControlBlock &ncb) = 0;

        virtual void erase(key_t key) = 0;

        virtual std::vector<std::pair<key_t, NodeControlBlock&>> list() = 0;

        virtual std::vector<std::pair<key_t, const NodeControlBlock&>> list() const = 0;
    };

    class UnorderedMapLookupMethod : LookupMethod {
    private:
        std::unordered_map<key_t, NodeControlBlock> nodes;

    public:
        NodeControlBlock &lookup(key_t key);

        const NodeControlBlock &lookup(key_t key) const;

        bool contains(key_t key);

        void insert(key_t key, const NodeControlBlock &ncb);

        void erase(key_t key);

        std::vector<std::pair<key_t, NodeControlBlock&>> list();

        std::vector<std::pair<key_t, const NodeControlBlock&>> list() const;
    };

}

#endif //HASH_OCTREE_LOOKUP_METHODS_H
