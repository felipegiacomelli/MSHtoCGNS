#ifndef CGNS_FILE_HPP
#define CGNS_FILE_HPP

#include <set>
#include <algorithm>
#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

class CgnsCreator {
	public:
		CgnsCreator(GridDataShared gridData, const std::string& folderPath);

		std::string getFileName() const;

		virtual ~CgnsCreator();

	protected:
		void initialize();
		void setupFile();
		virtual void writeBase();
		virtual void writeZone();
		virtual void writeCoordinates() = 0;
		void writeSections();
		virtual void writeRegions() = 0;
		virtual void writeBoundaries() = 0;
		void writeBoundaryConditions();

		GridDataShared gridData;
		std::string folderPath, baseName, zoneName, fileName;
		int cellDimension, physicalDimension;
		std::array<int, 3> sizes;
		int fileIndex, baseIndex, zoneIndex;
		std::vector<int> coordinateIndices, sectionIndices, boundaryIndices;
		// int elementStart, elementEnd;
};

#endif