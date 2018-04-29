#ifndef MSH_READER_HPP
#define MSH_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <Utilities/Set.hpp>

#include <string>
#include <fstream>
#include <numeric>

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
		virtual void assignElementsToRegions() = 0;
		virtual void assignFacetsToBoundaries() = 0;
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