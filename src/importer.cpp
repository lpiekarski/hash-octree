#include "importer.h"

namespace HashOctree {

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

}