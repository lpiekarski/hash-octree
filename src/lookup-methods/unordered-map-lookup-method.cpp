#include "../../include/lookup-methods/unordered-map-lookup-method.h"

namespace HashOctree {


    status_t UnorderedMapLookupMethod::lookup(key_t key, NodeControlBlock **outNcb) {
        if (nodes.count(key) == 0)
            return NODE_DOESNT_EXIST;
        if (outNcb != nullptr)
            *outNcb = &nodes[key];
        return OK;
    }

    bool UnorderedMapLookupMethod::contains(key_t key) {
        return nodes.count(key) > 0;
    }

    status_t UnorderedMapLookupMethod::insert(key_t key, const NodeControlBlock &ncb) {
        nodes[key] = ncb;
        return OK;
    }

    status_t UnorderedMapLookupMethod::erase(key_t key) {
        nodes.erase(key);
        return OK;
    }

    std::vector<std::pair<key_t, NodeControlBlock&>> UnorderedMapLookupMethod::list() {
        std::vector<std::pair<key_t, NodeControlBlock&>> ret;

        for (auto &kv : nodes)
            ret.emplace_back(kv.first, kv.second);

        return ret;
    }

}