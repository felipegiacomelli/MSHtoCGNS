#ifndef __MANIPULATION_MULTIPLE_BASES_CGNS_CREATOR_HPP__
#define __MANIPULATION_MULTIPLE_BASES_CGNS_CREATOR_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsCreator.hpp"

class MultipleBasesCgnsCreator : public CgnsCreator {
    public:
        MultipleBasesCgnsCreator(std::vector<boost::shared_ptr<GridData>> gridDatas, std::vector<std::string> baseNames, std::vector<std::string> zoneNames, std::string outputPath);

    private:
        void create() override;

        std::vector<boost::shared_ptr<GridData>> gridDatas;
        std::vector<std::string> baseNames;
        std::vector<std::string> zoneNames;
        bool firstCall = true;
};

#endif
