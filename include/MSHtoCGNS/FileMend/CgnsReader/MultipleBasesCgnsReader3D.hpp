#ifndef __FILE_MEND_MULTIPLE_BASES_CGNS_READER_3D_HPP__
#define __FILE_MEND_MULTIPLE_BASES_CGNS_READER_3D_HPP__

#include <map>
#include <iostream>
#include <iomanip>

#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"

class MultipleBasesCgnsReader3D : public CgnsReader3D {
    public:
        MultipleBasesCgnsReader3D(std::string filePath);

        void print() const;

        bool exists(std::string name) const;

        boost::shared_ptr<GridData> read(std::string name);
        boost::shared_ptr<GridData> read(int baseIndex);

    private:
        void readBases();

        std::map<std::string, int> bases;
};

#endif
