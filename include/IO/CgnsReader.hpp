#ifndef CGNS_READER_HPP
#define CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
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

		GridData getGridData() const;

		~CgnsReader();

	protected:
		void checkFile();
		void readBase();
		void readZone();
		void readNumberOfSections();
		virtual void readNodes() = 0;
		virtual void readElements() = 0;
		virtual void defineBoundaryVerticesIndices() = 0;

		std::string filePath;
		char* buffer;
		int cgnsFile, cgnsBase, cgnsZone, cellDimension, physicalDimension;
		std::vector<int> sectionIndices, boundaryIndices;
		ZoneType_t zoneType;
		std::vector<cgsize_t> zoneSizes;
		cgsize_t numberOfNodes;
		std::vector<std::vector<int>> elements, physicalEntitiesElementIndices;
		GridData gridData;
};

#endif