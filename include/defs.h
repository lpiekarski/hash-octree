#ifndef HASH_OCTREE_DEFS_H
#define HASH_OCTREE_DEFS_H

#include <cstdint>

namespace HashOctree {

    using key_t = uint64_t;
    using dim_t = double;
    using status_t = int64_t;
    using data_t = void*;

    const status_t INDEX_OUT_OF_BOUNDS = -7;
    const status_t KEY_COLLISION       = -6;
    const status_t CHILD_DOESNT_EXIST  = -5;
    const status_t NODE_DOESNT_EXIST   = -4;
    const status_t NODE_IS_ROOT        = -3;
    const status_t NODE_HAS_REFS       = -2;
    const status_t NODE_EXISTS         = -1;
    const status_t OK                  = 0;
    const status_t NODE_REPLACED       = 1;

    const int      FL_REC              = 1;
    const int      FL_IGNORE_REFS      = 2;
    const int      FL_FORCE            = 4;

}

#endif //HASH_OCTREE_DEFS_H
