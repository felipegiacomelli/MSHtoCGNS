#ifndef CGNS_READER_HPP
#define CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <BoostInterface/SharedPointer.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <Utilities/Set.hpp>

#include <string>
#include <fstream>
#include <numeric>

#include <cgnslib.h>

class CgnsReader {
	public:
		CgnsReader() = default;
		CgnsReader(const std::string&);

		~CgnsReader();

		GridDataShared gridData;

	protected:
		void checkFile();
		void readBase();
		void readZone();
		void readNumberOfSections();
		void readNumberOfBoundaries();
		virtual void readCoordinates() = 0;
		virtual void readSections() = 0;
		void readBoundaries();

		std::string filePath;
		char* buffer;
		int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
		std::vector<int> sectionIndices, boundaryIndices;
		ZoneType_t zoneType;
		std::vector<cgsize_t> zoneSizes;
		cgsize_t numberOfVertices;
};

#endif