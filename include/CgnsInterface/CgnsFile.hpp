#ifndef CGNS_INTERFACE_CGNS_FILE_HPP	
#define CGNS_INTERFACE_CGNS_FILE_HPP

#include <iostream>
#include <string>
#include <vector>

#include <cgnslib.h>

#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

class CgnsFile {
	public:
		CgnsFile() = default;
		CgnsFile(const GridData&, const std::string&);

		void initialize();
		void writePermanentField(const std::vector<double>&, const std::string&);
		void writeTimeSteps(const std::vector<double>&);
		void writeTransientField(const std::vector<std::vector<double>>&, const std::string&);

		~CgnsFile() = default;

	protected:
		virtual void writeBase() = 0;
		virtual void writeZone() = 0;
		virtual void writeCoordinates() = 0;
		virtual void writeSections() = 0;
		//virtual void writeBoundaryConditions() = 0;

		GridData gridData;
		std::string folderPath, baseName, zoneName, fileName;
		cgsize_t cellDimension, physicalDimension, numberOfNodes, numberOfElements, numberOfTimeSteps;
		std::vector<cgsize_t> zoneSizes;
		std::vector<int> coordinateIndices, sectionIndices, boundaryIndices, solutionIndices, fieldsIndices;
		int fileIndex, baseIndex, zoneIndex, solutionIndex, fieldIndex;
};

#endif