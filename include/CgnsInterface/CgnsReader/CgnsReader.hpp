#ifndef CGNS_READER_HPP
#define CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <string>
#include <fstream>
#include <numeric>

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
		char buffer[800];
		int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
		std::vector<int> sectionIndices, boundaryIndices;
		std::array<int, 3> zoneSizes;
		int numberOfVertices;
};

#endif