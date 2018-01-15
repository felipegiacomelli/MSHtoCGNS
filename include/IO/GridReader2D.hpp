#ifndef GRID_READER_2D_HPP
#define GRID_READER_2D_HPP

#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <IO/GridReader.hpp>

#include <string>
#include <fstream>

class GridReader2D : public GridReader {
	public:
		GridReader2D() = default;
		GridReader2D(std::string&&);

	private:
		void readPhysicalEntities() override;
		void readElements() override;
		void addElements() override;
};

#endif