#ifndef GRID_READER_3D_HPP
#define GRID_READER_3D_HPP

#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <IO/GridReader.hpp>

#include <string>
#include <fstream>

class GridReader3D : public GridReader {
	public:
		GridReader3D() = default;
		GridReader3D(std::string&&);

	private:
		void readPhysicalEntities() override;
		void addElements() override;
};

#endif