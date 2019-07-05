#include <iostream>

#include "hash-octree.h"

int main() {
    HashOctree::HashOctree ho;
    ho.addDataPoint(0.5, 0.5, 0.5, 0.1, 0.1, 0.1, (void *)5);
    std::cout << HashOctree::Exporter::toString(ho);
    auto exported = HashOctree::Exporter::toByteArray(ho);
    HashOctree::HashOctree ho2 = HashOctree::Importer::fromByteArray(exported);
    std::cout << HashOctree::Exporter::toString(ho2);
    return 0;
}