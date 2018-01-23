#ifndef GRID_READER_HPP
#define GRID_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>

#include <string>
#include <fstream>

class GridReader {
	public:
		GridReader() = default;
		GridReader(const std::string&);

		GridData getGridData() const;

		~GridReader();

	protected:
		void readNodes();
		void readElements();
		virtual void readPhysicalEntities() = 0;
		virtual void addElements() = 0;

		std::string filePath;
		std::ifstream file;
		char* buffer;
		int numberOfPhysicalEntities, geometryNumber;
		std::vector<std::vector<int>> elements, physicalEntitiesElementIndices;
		GridData gridData;
};

#endif