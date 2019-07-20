#include "../include/point-data.h"
#include "../include/data-utils.h"

namespace HashOctree {

    std::ostream& operator<<(std::ostream &os, const Point &p) {
        if (os.binary) {
            std::vector<char> v;
            //requires implementation of push_float in data-utils
            //push_int(v, ())
            //os.write()
        } else {
            os << p.dim.origin[0] << " " << p.dim.origin[1] << " " << p.dim.origin[2] << " ";
            os << p.dim.halfDim[0] << " " << p.dim.halfDim[1] << " " << p.dim.halfDim[2] << " ";
            os << (uint64_t) p.data;
        }

        return os;
    }

}