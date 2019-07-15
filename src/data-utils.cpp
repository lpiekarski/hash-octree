#include "data-utils.h"

namespace HashOctree {
    template<typename T, typename>
    int DataUtils::read_int(const std::vector<char> &v, size_t pos, T &ret) {
        size_t bytes = sizeof(T);
        ret = 0;

        while (bytes--) {
            ret = ret << 8;
            ret += (unsigned char) v[pos + bytes];
        }

        return sizeof(T);
    }

    template<typename T, typename>
    int DataUtils::push_int(std::vector<char> &v, T val) {
        size_t bytes = sizeof(T);

        while (bytes--) {
            v.push_back(val & 255);
            val = val >> 8;
        }

        return sizeof(T);
    }

}