#include <iostream>

#include "hash-octree.h"

int main() {
    HashOctree::HashOctree ho;
    std::cout << HashOctree::Exporter::toString(ho);
    std::cout << ho.create((void *)5) << "\n";
    std::cout << HashOctree::Exporter::toString(ho);
    std::cout << ho.create((void *)7) << "\n";
    std::cout << ho.changeRoot(HashOctree::Encryptor::encrypt((void *)5)) << "\n";
    std::cout << HashOctree::Exporter::toString(ho);
    std::cout << ho.changeRoot(HashOctree::Encryptor::encrypt((void *)7)) << "\n";
    std::cout << HashOctree::Exporter::toString(ho);
    return 0;
}