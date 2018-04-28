#ifndef CGNS_FILE_HPP	
#define CGNS_FILE_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Utilities/Set.hpp>
#include <Grid/GridData.hpp>

class CgnsCreator {
	public:
		CgnsCreator() = default;
		CgnsCreator(GridDataShared, const std::string&);

		std::string getFileName() const;

		virtual ~CgnsCreator();

	protected:
		void initialize();
		virtual void setupFile() = 0;
		virtual void writeBase();
		virtual void writeZone();
		virtual void writeCoordinates() = 0;
		virtual void writeSections() = 0;
		void writeBoundaryConditions();

		GridDataShared gridData;
		std::string folderPath, baseName, zoneName, fileName;
		int cellDimension, physicalDimension;
		std::array<int, 3> sizes;
		int fileIndex, baseIndex, zoneIndex;
		std::vector<int> coordinateIndices, sectionIndices, boundaryIndices;
};

#endif