#include <sstream>
#include <iomanip>

#include "../include/data-utils.h"
#include "../include/encryptor.h"
#include "../include/exporter.h"

namespace HashOctree {
/*
    template <typename ELM, typename EE>
    std::string Exporter<ELM, EE>::toString(const HashOctree<ELM, EE> &ho) {
        std::stringstream ss("");
        ss.fill('0');
        ss << "root: " << std::setw(16) << std::hex << ho.root << "\n";
        ho.lookupMethod.iter([](key_t key, const NodeControlBlock &ncb, LookupMethod &lm) {
            ss << std::setw(16) << std::hex << kv.first << ": ";
            ss << "data: " << std::setw(16) << std::hex << (uint64_t)kv.second.node.data << ", children: ";
            for (size_t i = 0; i < 8; i++)
                ss << std::setw(16) << std::hex << kv.second.node.children[i] << " ";
            ss << "(" << std::dec << kv.second.refs << " ref(s))\n";
        });
        for (const auto &kv : ho.nodes) {

        }

        return ss.str();
    }

    std::string Exporter::toJson(const HashOctree &ho, size_t indentWidth, bool newLines) {
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
        for (const auto &kv : ho.nodes) {
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

    std::vector<char> Exporter::toByteArray(const HashOctree &ho) {
        std::vector<char> ret;
        DataUtils::push_int<uint64_t>(ret, ho.root);

        for (const auto &kv : ho.nodes) {
            DataUtils::push_int<uint64_t>(ret, kv.first);
            DataUtils::push_int<uint64_t>(ret, (uint64_t) kv.second.node.data);
            for (int i = 0; i < 8; i++)
                DataUtils::push_int<uint64_t>(ret, kv.second.node.children[i]);
        }

        return ret;
    }
*/
}