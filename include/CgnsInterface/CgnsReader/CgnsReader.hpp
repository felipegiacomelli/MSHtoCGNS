#ifndef CGNS_READER_HPP
#define CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <string>
#include <numeric>

class CgnsReader {
	public:
		CgnsReader(const std::string& filePath);

		~CgnsReader();

		std::vector<double> readField(const int& solutionIndex, const std::string& fieldName);
		int readNumberOfTimeSteps();

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
		std::array<int, 3> sizes;
		int numberOfSections, numberOfBoundaries;
};

#endif