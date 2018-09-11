#ifndef IO_MSH_READER_HPP
#define IO_MSH_READER_HPP

#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <numeric>
#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>

class MshReader {
	public:
		MshReader(std::string filePath);

		virtual ~MshReader() = default;

		GridDataShared gridData;

	protected:
		void checkFile();
		void readNodes();
		virtual void readPhysicalEntities() = 0;
		void readConnectivities();
		virtual void determineNumberOfFacets() = 0;
		void divideConnectivities();
		void assignElementsToRegions();
		void assignFacetsToBoundaries();
		virtual void addRegions() = 0;
		virtual void addBoundaries() = 0;
		virtual void defineBoundaryVertices() = 0;

		std::string filePath;
		std::ifstream file;
		char buffer[800];
		int numberOfPhysicalEntities, numberOfBoundaries, numberOfRegions, numberOfFacets;
		std::vector<std::vector<int>> connectivities, elements, facets, regionElements, boundaryFacets;
};

#endif