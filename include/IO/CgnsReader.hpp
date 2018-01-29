#ifndef CGNS_READER_HPP
#define CGNS_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>

#include <string>
#include <fstream>

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
		void readSection();
		void readDimension();
		virtual void readNodes() = 0;
		virtual void readElements() = 0;

		std::string filePath;
		char* buffer;
		int cgnsFile, cgnsBase, cgnsZone, numberOfSections, cellDimension, physicalDimension;
		ZoneType_t zoneType;
		std::vector<cgsize_t> zoneSizes;
		cgsize_t numberOfNodes;
		std::vector<std::vector<int>> elements, physicalEntitiesElementIndices;
		GridData gridData;
};

#endif