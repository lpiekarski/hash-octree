#include "../../include/lookup-methods/unordered-map-lookup-method.h"

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

    std::vector<std::pair<key_t, NodeControlBlock&>> UnorderedMapLookupMethod::list() {
        std::vector<std::pair<key_t, NodeControlBlock&>> ret;

        for (auto &kv : nodes)
            ret.emplace_back(kv.first, kv.second);

        return ret;
    }

    std::vector<std::pair<key_t, const NodeControlBlock&>> UnorderedMapLookupMethod::list() const {
        std::vector<std::pair<key_t, const NodeControlBlock&>> ret;

        for (const auto &kv : nodes)
            ret.emplace_back(kv.first, kv.second);

        return ret;
    }

}