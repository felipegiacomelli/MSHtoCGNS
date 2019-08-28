#include "MSHtoCGNS/Manipulation/CgnsReader/SpecialCgnsReader.hpp"
#include <cgnslib.h>

SpecialCgnsReader::SpecialCgnsReader(std::string filePath) : CgnsReader(filePath, false) {
    this->read();
}

bool SpecialCgnsReader::skipSection(std::string name, int elementType) {
    if (name.substr(name.length() - 3) == "_1D" || name.substr(name.length() - 3) == "_0D")
        return true;

    if (elementType == BAR_2)
        return true;

    return false;
}
