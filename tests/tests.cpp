#include <iostream>

#include "hash-octree.h"

int main() {
    HashOctree::HashOctree ho;
    ho.addDataPoint(8, 8, 8, 8, 8, 8, (void *)5);
    std::cout << HashOctree::Exporter::toString(ho);
    return 0;
}