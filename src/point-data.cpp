#include "point-data.h"
#include "data-utils.h"

namespace HashOctree {

    std::ostream& operator<<(std::ostream &os, const Point &p) {
        if (os.binary) {
            std::vector<char> v;
            //requires implementation of push_float in data-utils
            //push_int(v, ())
            //os.write()
        } else {
            os << p.origin[0] << " " << p.origin[1] << " " << p.origin[2] << " ";
            os << p.halfDim[0] << " " << p.halfDim[1] << " " << p.halfDim[2] << " ";
            os << (uint64_t) p.data;
        }

        return os;
    }

}