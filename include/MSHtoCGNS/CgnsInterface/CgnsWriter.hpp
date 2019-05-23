#ifndef __CGNS_INTERFACE_CGNS_WRITER_HPP__
#define __CGNS_INTERFACE_CGNS_WRITER_HPP__

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"

class CgnsWriter {
    public:
        CgnsWriter() = default;
        CgnsWriter(std::string path, std::string gridLocation);

        void writePermanentSolution(std::string name);
        void writePermanentField(std::string name, const std::vector<double>& values);
        void writePermanentField(std::string name, const std::vector<int>& values);

        void writeTransientSolution(double timeInstant);
        void writeTransientField(std::string name, const std::vector<double>& values);

        void finalizeTransient();

        virtual ~CgnsWriter();

    private:
        void initialize();
        void checkFile();
        void readBase();
        void readZone();

        std::string path;
        int gridLocation;
        int fileIndex, baseIndex, zoneIndex;
        int permanentSolutionIndex, permanentFieldIndex;
        std::vector<int> solutionIndices, fieldsIndices;
        std::vector<double> timeInstants;
        bool isFinalized;
};

#endif
