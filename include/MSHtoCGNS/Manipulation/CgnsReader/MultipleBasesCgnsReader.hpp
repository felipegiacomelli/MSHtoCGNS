#ifndef __MANIPULATION_MULTIPLE_BASES_CGNS_READER_HPP__
#define __MANIPULATION_MULTIPLE_BASES_CGNS_READER_HPP__

#include <map>
#include <iostream>
#include <iomanip>

#include "MSHtoCGNS/CgnsInterface/CgnsReader.hpp"

class MultipleBasesCgnsReader : public CgnsReader {
    public:
        MultipleBasesCgnsReader(std::string filePath);

        void print() const;

        bool exists(std::string name) const;

        boost::shared_ptr<GridData> read(std::string name);
        boost::shared_ptr<GridData> read(int baseIndex);

    private:
        void readBases();

        std::map<std::string, int> bases;
};

#endif
