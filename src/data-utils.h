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
        static int read_int(const std::vector<char> &v, size_t pos, T &ret);

        template <typename T,
                typename = std::enable_if_t<std::is_integral_v<T>>>
        static int push_int(std::vector<char> &v, T val);
    };

}

#endif //HASH_OCTREE_DATA_UTILS_H
