#include <cstdlib>

#include "encryptor.h"

namespace HashOctree {

    const uint64_t PRIME_P = 32416187567;
    const uint64_t PRIME_Q = 1610612741;

    key_t Encryptor::encrypt(const key_t *children, const void *data) {
        key_t ret, p;

        p = PRIME_P;
        ret = (key_t) data * PRIME_Q;
        for (size_t i = 0; i < 8; i++) {
            ret += (children[i] * p);
            p *= PRIME_P;
        }

        return ret;
    }

    key_t Encryptor::encrypt(const void *data) {
        key_t empty_children[8];
        for (size_t i = 0; i < 8; i++)
            empty_children[i] = 0;
        return Encryptor::encrypt(empty_children, data);
    }

    key_t Encryptor::encrypt(const Node &node) {
        return Encryptor::encrypt(node.children, node.data);
    }
}