#ifndef GRID_READER_HPP
#define GRID_READER_HPP

#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>

#include <string>
#include <fstream>

class GridReader {
	public:
		GridReader() = default;
		GridReader(std::string&&);

		~GridReader();

	private:
		void readPhysicalEntities();
		void readNodes();
		void readElements();

		std::string filePath;
		std::ifstream file;
		char* buffer;
		GridData gridData;
};

#endif