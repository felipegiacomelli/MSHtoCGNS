#ifndef MSH_READER_HPP
#define MSH_READER_HPP

#include <string>
#include <fstream>
#include <numeric>
#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <Utilities/Set.hpp>

class MshReader {
	public:
		MshReader() = default;
		MshReader(const std::string&);

		virtual ~MshReader() = default;

		GridDataShared gridData;

	protected:
		void checkFile();
		void readNodes();
		void readConnectivities();
		virtual void divideConnectivities() = 0;
		void assignElementsToRegions();
		void assignFacetsToBoundaries();
		virtual void readPhysicalEntities() = 0;
		virtual void addElements() = 0;
		virtual void addFacets() = 0;
		virtual void defineBoundaryVertices() = 0;

		std::string filePath;
		std::ifstream file;
		char buffer[800];
		int numberOfPhysicalEntities, numberOfBoundaries, numberOfRegions;
		std::vector<std::vector<int>> connectivities, elements, facets, regionElements, boundaryFacets;
};

#endif