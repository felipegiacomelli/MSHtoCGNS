#ifndef __CGNS_INTERFACE_CGNS_WRITER_HPP__
#define __CGNS_INTERFACE_CGNS_WRITER_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsOpener.hpp"

class CgnsWriter : public CgnsOpener {
    public:
        CgnsWriter() = default;
        CgnsWriter(std::string filePath, std::string gridLocation);

        void writePermanentSolution(std::string name);
        void writePermanentField(std::string name, const std::vector<double>& values);
        void writePermanentField(std::string name, const std::vector<int>& values);

        void writeTransientSolution(double timeValue);
        void writeTransientField(std::string name, const std::vector<double>& values);

        void finalizeTransient();

        ~CgnsWriter();

    private:
        void setGridLocation(std::string gridLocation);

        int gridLocation;
        int permanentSolutionIndex, permanentFieldIndex;
        int solutionIndex, fieldIndex;
        std::vector<double> timeValues;
        bool isFinalized = false;
};

#endif
