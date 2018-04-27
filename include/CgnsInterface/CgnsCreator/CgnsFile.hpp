#ifndef CGNS_FILE_HPP	
#define CGNS_FILE_HPP

#include <cgnslib.h>

#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Utilities/Set.hpp>
#include <Grid/GridData.hpp>

class CgnsFile {
	public:
		CgnsFile() = default;
		CgnsFile(GridDataShared, const std::string&);

		std::string getFileName() const;

		virtual ~CgnsFile();

	protected:
		void initialize();
		void resizeVectors();
		virtual void setupFile() = 0;
		virtual void defineGeometryType() = 0; 
		virtual void writeBase();
		virtual void writeZone();
		virtual void writeCoordinates() = 0;
		virtual void writeSections() = 0;
		void writeBoundaryConditions();

		GridDataShared gridData;
		std::string folderPath, baseName, zoneName, fileName;
		int cellDimension, physicalDimension, numberOfVertices, numberOfElements, numberOfTimeSteps;
		std::vector<int> zoneSizes;
		std::vector<int> coordinateIndices, sectionIndices, boundaryIndices, solutionIndices, fieldsIndices;
		int fileIndex, baseIndex, zoneIndex, solutionIndex, fieldIndex;
		ElementType_t geometry, boundary;
};

#endif