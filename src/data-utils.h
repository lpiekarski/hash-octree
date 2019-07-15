#ifndef HASH_OCTREE_DATA_UTILS_H
#define HASH_OCTREE_DATA_UTILS_H

#include <cstdint>
#include <cstdlib>
#include <vector>
#include <type_traits>

namespace HashOctree {

    class DataUtils {
    public:
        template <typename T,
                typename = std::enable_if_t<std::is_integral_v<T>>>
        static int read_int(const std::vector<char> &v, size_t pos, T &ret) {
            size_t bytes = sizeof(T);
            ret = 0;

            while (bytes--) {
                ret = ret << 8;
                ret += (unsigned char) v[pos + bytes];
            }

            return sizeof(T);
        }

        template <typename T,
                typename = std::enable_if_t<std::is_integral_v<T>>>
        static int push_int(std::vector<char> &v, T val) {
            size_t bytes = sizeof(T);

            while (bytes--) {
                v.push_back(val & 255);
                val = val >> 8;
            }

            return sizeof(T);
        }
    };

}

#endif //HASH_OCTREE_DATA_UTILS_H
