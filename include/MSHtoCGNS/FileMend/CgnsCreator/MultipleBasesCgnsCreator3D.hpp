#ifndef MULTIPLE_BASES_CGNS_CREATOR_3D_HPP
#define MULTIPLE_BASES_CGNS_CREATOR_3D_HPP

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
