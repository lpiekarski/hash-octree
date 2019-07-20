#ifndef HASH_OCTREE_IMPORTER_H
#define HASH_OCTREE_IMPORTER_H

#include "hash-octree.h"
#include "point-data.h"
#include "data-utils.h"

namespace HashOctree {

    class Importer {
    public:
        template <typename ILM, typename IE>
        static HashOctree<ILM, IE> fromByteArray(const std::vector<char> &array) {
            HashOctree<ILM, IE> ret;
            size_t read_id = 0;
            DataUtils::read_int<uint64_t>(array, read_id, ret.root);
            read_id += 8;

            while (read_id < array.size()) {
                uint64_t key;
                DataUtils::read_int<uint64_t>(array, read_id, key);
                read_id += 8;
                uint64_t data;
                DataUtils::read_int<uint64_t>(array, read_id, data);
                read_id += 8;
                uint64_t children[8];
                for (int i = 0; i < 8; i++) {
                    DataUtils::read_int<uint64_t>(array, read_id, children[i]);
                    read_id += 8;
                }
                ret.lookupMethod.lookup(key).key = key;
                ret.lookupMethod.lookup(key).node.data = (void *) data;
                for (int i = 0; i < 8; i++)
                    ret.lookupMethod.lookup(key).node.children[i] = children[i];
            }

            ret.recountRefs();

            return ret;
        }

        template <typename ILM, typename IE>
        static HashOctree<ILM, IE> fromJson(const std::string &str) {
            HashOctree<ILM, IE> ret;

            return ret;
        }

        template <typename ILM, typename IE>
        static HashOctree<ILM, IE> fromPointData(const PointData &pdt) {
            HashOctree<ILM, IE> ret;

            return ret;
        }
    };

}

#endif //HASH_OCTREE_IMPORTER_H
