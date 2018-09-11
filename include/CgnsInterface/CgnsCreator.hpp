#ifndef CGNS_INTERFACE_CGNS_CREATOR_HPP
#define CGNS_INTERFACE_CGNS_CREATOR_HPP

#include <set>
#include <algorithm>
#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>

class CgnsCreator {
	public:
		CgnsCreator(GridDataShared gridData, std::string folderPath);

		std::string getFileName() const;

		~CgnsCreator();

	protected:
		virtual void checkDimension() = 0;
		void setupFile();
		void initialize();
		void writeBase();
		virtual void writeZone();
		virtual void writeCoordinates() = 0;
		virtual void writeSections();
		virtual void buildElementConnectivities() = 0;
		virtual void writeRegions() = 0;
		virtual void buildFacetConnectivities() = 0;
		virtual void writeBoundaries() = 0;
		void writeBoundaryConditions();

		GridDataShared gridData;
		std::string folderPath, baseName, zoneName, fileName;
		int cellDimension, physicalDimension;
		std::array<int, 3> sizes;
		int fileIndex, baseIndex, zoneIndex;
		std::vector<int> coordinateIndices, sectionIndices, boundaryIndices;
		int elementStart, elementEnd;

		std::vector<std::vector<int>> elementConnectivities;
		std::vector<std::vector<int>> facetConnectivities;
};

#endif