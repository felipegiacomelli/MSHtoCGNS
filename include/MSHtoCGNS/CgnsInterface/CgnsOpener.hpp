#ifndef __CGNS_INTERFACE_CGNS_OPENER_HPP__
#define __CGNS_INTERFACE_CGNS_OPENER_HPP__

#include <string>
#include <set>

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"
#include "MSHtoCGNS/Utilities/Error.hpp"

class CgnsOpener {
    public:
        CgnsOpener(std::string filePath, std::string mode);

        virtual ~CgnsOpener();

    protected:
        void setMode(std::string mode);
        void checkFile();
        void convertToHdf5();
        void openFile();
        void readNumberOfBases();
        void readBase();
        void readZone();

        boost::filesystem::path file, temporaryFile;
        int mode;

        int fileType;
        int fileIndex;
        float fileVersion;
        int numberOfBases;
        char buffer[1024];
        int baseIndex = 1, zoneIndex, cellDimension, physicalDimension;
        int sizes[3];
};

#endif
