#ifndef __CGNS_INTERFACE_CGNS_WRITER_HPP__
#define __CGNS_INTERFACE_CGNS_WRITER_HPP__

#include "MSHtoCGNS/CgnsInterface/CgnsOpener.hpp"

class CgnsWriter : public CgnsOpener {
    public:
        CgnsWriter() = default;
        CgnsWriter(std::string filePath, std::string gridLocation);

        void writeSolution(std::string name);
        void writeSolution(double timeValue);

        void writeField(std::string name, const std::vector<double>& values);
        void writeField(std::string name, const std::vector<int>& values);

        void finalizeTransient();

        ~CgnsWriter();

    private:
        void setGridLocation(std::string gridLocation);

        int gridLocation;
        int solutionIndex, fieldIndex;
        std::vector<double> timeValues;
        bool isFinalized = false;
};

#endif
