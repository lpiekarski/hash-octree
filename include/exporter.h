#ifndef HASH_OCTREE_EXPORTER_H
#define HASH_OCTREE_EXPORTER_H

#include <vector>
#include <sstream>
#include <iomanip>

#include "hash-octree.h"
#include "point-data.h"
#include "data-utils.h"

namespace HashOctree {

    class Exporter {
    public:

        template <typename ELM, typename EE>
        static std::string toString(HashOctree<ELM, EE> &ho) {
            std::stringstream ss("");
            ss.fill('0');
            ss << "root: " << std::setw(16) << std::hex << ho.root << "\n";
            for (const auto &kv : ho.lookupMethod.list()) {
                ss << std::setw(16) << std::hex << kv.first << ": ";
                ss << "data: " << std::setw(16) << std::hex << (uint64_t) kv.second.node.data << ", children: ";
                for (size_t i = 0; i < 8; i++)
                    ss << std::setw(16) << std::hex << kv.second.node.children[i] << " ";
                ss << "(" << std::dec << kv.second.refs << " ref(s))\n";
            }

            return ss.str();
        }

        template <typename ELM, typename EE>
        static std::string toJson(HashOctree<ELM, EE> &ho, size_t indentWidth=0, bool newLines=false) {
            std::stringstream ss("");
            size_t indents = 0;
            char newLine;
            if (newLines)
                newLine = '\n';
            else
                newLine = '\0';
            char space;
            if (newLines)
                space = ' ';
            else
                space = '\0';
            ss << "{" << newLine;
            indents++;
            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "\"root\":" << space << "\"" << ho.root << "\"" << "," << newLine;

            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << " ";
            ss << "\"nodes\":" << space << "{" << newLine;
            indents++;
            bool first = true;
            for (const auto &kv : ho.lookupMethod.list()) {
                if (!first) {
                    ss << "," << newLine;
                }
                else {
                    first = false;
                }

                for (size_t i = 0; i < indents * indentWidth; i++)
                    ss << space;
                ss << "\"" << kv.first << "\":" << space;

                ss << "{" << newLine;
                indents++;

                for (size_t i = 0; i < indents * indentWidth; i++)
                    ss << space;
                ss << "\"data\":" << space << "\"" << (uint64_t)kv.second.node.data << "\"" << "," << newLine;

                for (size_t i = 0; i < indents * indentWidth; i++)
                    ss << space;
                ss << "\"children\":" << space << "[" << newLine;
                indents++;

                for (size_t i = 0; i < 8; i++) {
                    for (size_t j = 0; j < indents * indentWidth; j++)
                        ss << space;

                    ss << "\"" << kv.second.node.children[i] << "\"";
                    if (i != 7)
                        ss << "," << newLine;
                    else
                        ss << newLine;
                }
                indents--;
                for (size_t i = 0; i < indents * indentWidth; i++)
                    ss << space;
                ss << "]" << newLine;

                indents--;
                for (size_t i = 0; i < indents * indentWidth; i++)
                    ss << space;
                ss << "}";

            }
            ss << newLine;
            indents--;
            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "}" << newLine;

            indents--;
            for (size_t i = 0; i < indents * indentWidth; i++)
                ss << space;
            ss << "}" << newLine;

            return ss.str();
        }

        template <typename ELM, typename EE>
        static std::vector<char> toByteArray(HashOctree<ELM, EE> &ho) {
            std::vector<char> ret;
            DataUtils::push_int<uint64_t>(ret, ho.root);

            for (const auto &kv : ho.lookupMethod.list()) {
                DataUtils::push_int<uint64_t>(ret, kv.first);
                DataUtils::push_int<uint64_t>(ret, (uint64_t) kv.second.node.data);
                for (int i = 0; i < 8; i++)
                    DataUtils::push_int<uint64_t>(ret, kv.second.node.children[i]);
            }

            return ret;
        }

        //static PointData toPointData(const HashOctree<ELM, EE> &ho);
    };

}

#endif //HASH_OCTREE_EXPORTER_H
