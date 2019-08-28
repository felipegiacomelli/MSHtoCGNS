#ifndef __MANIPULATION_SPECIAL_CGNS_READER_HPP__
#define __MANIPULATION_SPECIAL_CGNS_READER_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

class SpecialCgnsReader : public CgnsReader {
    public:
        SpecialCgnsReader(std::string filePath);

    private:
        bool skipSection(std::string name, int elementType) override;
        void addWell() = delete;
};

#endif
