#include "../include/importer.h"
#include "../include/data-utils.h"

namespace HashOctree {

    HashOctree Importer::fromByteArray(const std::vector<char> &array) {
        HashOctree ret;
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
            ret.nodes[key].key = key;
            ret.nodes[key].node.data = (void *) data;
            for (int i = 0; i < 8; i++)
                ret.nodes[key].node.children[i] = children[i];
        }

        ret.recountRefs();

        return ret;
    }

    HashOctree Importer::fromJson(const std::string &str) {
        HashOctree ret;

        return ret;
    }

    HashOctree Importer::fromPointData(const PointData &pdt) {
        HashOctree ret;

        return ret;
    }

}