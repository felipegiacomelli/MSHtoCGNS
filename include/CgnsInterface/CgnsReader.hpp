#ifndef CGNS_INTERFACE_CGNS_READER_HPP
#define CGNS_INTERFACE_CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <string>
#include <numeric>

class CgnsReader {
	public:
		CgnsReader(std::string filePath);

		std::vector<double> readField(std::string solutionName, std::string fieldName);
		std::vector<double> readField(const int& solutionIndex, std::string fieldName);
		int readNumberOfTimeSteps();
		std::vector<double> readTimeInstants();

		GridDataShared gridData;

		~CgnsReader();

	protected:
		void checkFile();
		void readBase();
		void readZone();
		void readNumberOfSections();
		void readNumberOfBoundaries();
		virtual void readCoordinates() = 0;
		virtual void readSections() = 0;
		void addRegion(std::string&& name, int elementStart, int numberOfElements);
		void addBoundary(std::string&& name, int elementStart, int numberOfElements);
		void readBoundaries();
		int readSolutionIndex(std::string solutionName);

		std::string filePath;
		char buffer[800];
		int fileIndex, baseIndex, zoneIndex, cellDimension, physicalDimension;
		int sizes[3];
		int numberOfSections, numberOfBoundaries;
};

#endif