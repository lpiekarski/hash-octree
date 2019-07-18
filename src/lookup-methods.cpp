#include "../include/lookup-methods.h"

namespace HashOctree {

    NodeControlBlock& UnorderedMapLookupMethod::lookup(key_t key) {
        return nodes[key];
    }

    const NodeControlBlock& UnorderedMapLookupMethod::lookup(key_t key) const {
        return nodes.at(key);
    }

    bool UnorderedMapLookupMethod::contains(key_t key) {
        return nodes.count(key) > 0;
    }

    void UnorderedMapLookupMethod::insert(key_t key, const NodeControlBlock &ncb) {
        nodes[key] = ncb;
    }

    void UnorderedMapLookupMethod::erase(key_t key) {
        nodes.erase(key);
    }

    void UnorderedMapLookupMethod::iter(void (*iterFunc)(key_t, NodeControlBlock &, LookupMethod &)) {
        for (auto &kv : nodes)
            iterFunc(kv.first, kv.second, *this);
    }

    void UnorderedMapLookupMethod::iter(void (*iterFunc)(key_t, const NodeControlBlock &, LookupMethod &)) {
        for (const auto &kv : nodes)
            iterFunc(kv.first, kv.second, *this);
    }
}