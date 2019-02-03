#ifndef __FILE_MEND_SPECIAL_CGNS_READER_3D_HPP__
#define __FILE_MEND_SPECIAL_CGNS_READER_3D_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsReader/CgnsReader3D.hpp"

class SpecialCgnsReader3D : public CgnsReader3D {
    public:
        SpecialCgnsReader3D(std::string filePath);

    private:
        bool skipSection(int elementType) override;
        void addWell(std::string&& name, int elementStart, int end) = delete;
};

#endif
