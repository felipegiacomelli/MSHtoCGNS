#ifndef __FILE_MEND_MULTIPLE_BASES_CGNS_CREATOR_3D_HPP__
#define __FILE_MEND_MULTIPLE_BASES_CGNS_CREATOR_3D_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsCreator/CgnsCreator3D.hpp"

class MultipleBasesCgnsCreator3D : public CgnsCreator3D {
    public:
        MultipleBasesCgnsCreator3D(std::vector<boost::shared_ptr<GridData>> gridDatas, std::vector<std::string> baseNames, std::string folderPath);

    private:
        void initialize();

        std::vector<boost::shared_ptr<GridData>> gridDatas;
        std::vector<std::string> baseNames;
        bool firstCall = true;
};

#endif
