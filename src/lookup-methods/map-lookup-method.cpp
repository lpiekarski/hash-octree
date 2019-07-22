#include "../../include/lookup-methods/map-lookup-method.h"

namespace HashOctree {

    NodeControlBlock& MapLookupMethod::lookup(key_t key) {
        return nodes[key];
    }

    const NodeControlBlock& MapLookupMethod::lookup(key_t key) const {
        return nodes.at(key);
    }

    bool MapLookupMethod::contains(key_t key) {
        return nodes.count(key) > 0;
    }

    void MapLookupMethod::insert(key_t key, const NodeControlBlock &ncb) {
        nodes[key] = ncb;
    }

    void MapLookupMethod::erase(key_t key) {
        nodes.erase(key);
    }

    std::vector<std::pair<key_t, NodeControlBlock&>> MapLookupMethod::list() {
        std::vector<std::pair<key_t, NodeControlBlock&>> ret;

        for (auto &kv : nodes)
            ret.emplace_back(kv.first, kv.second);

        return ret;
    }

    std::vector<std::pair<key_t, const NodeControlBlock&>> MapLookupMethod::list() const {
        std::vector<std::pair<key_t, const NodeControlBlock&>> ret;

        for (const auto &kv : nodes)
            ret.emplace_back(kv.first, kv.second);

        return ret;
    }

}