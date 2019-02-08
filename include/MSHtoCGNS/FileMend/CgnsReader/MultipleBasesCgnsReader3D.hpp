#ifndef __FILE_MEND_MULTIPLE_BASES_CGNS_READER_3D_HPP__
#define __FILE_MEND_MULTIPLE_BASES_CGNS_READER_3D_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"

class MultipleBasesCgnsReader3D : public CgnsReader3D {
    public:
        MultipleBasesCgnsReader3D(std::string filePath);
};

#endif
