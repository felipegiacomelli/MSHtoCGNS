#ifndef CGNS_WRITER_HPP
#define CGNS_WRITER_HPP

#include "MSHtoCGNS/BoostInterface/Filesystem.hpp"

class CgnsWriter {
    public:
        CgnsWriter() = default;
        CgnsWriter(std::string filePath, std::string solutionLocation);

        void writePermanentSolution(std::string solutionName);
        void writePermanentField(std::string scalarFieldName, const std::vector<double>& fieldValues);

        void writeTransientSolution(const double& timeInstant);
        void writeTransientField(const std::vector<double>& fieldValues, std::string fieldName);

        void finalizeTransient();

        virtual ~CgnsWriter();

    private:
        void initialize();
        void checkFile();
        void readBase();
        void readZone();

        std::string filePath;
        int gridLocation;
        int fileIndex, baseIndex, zoneIndex;
        int permanentSolutionIndex, permanentFieldIndex;
        std::vector<int> solutionIndices, fieldsIndices;
        std::vector<double> timeInstants;
        bool isFinalized;
};

#endif
