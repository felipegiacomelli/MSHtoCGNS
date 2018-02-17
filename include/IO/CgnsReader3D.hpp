#ifndef CGNS_READER_3D_HPP
#define CGNS_READER_3D_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <IO/CgnsReader.hpp>

#include <string>
#include <fstream>

class CgnsReader3D : public CgnsReader {
	public:
		CgnsReader3D() = default;
		CgnsReader3D(const std::string&);

	private:
		void readNodes() override;
		void readElements() override;
		void defineBoundaryVerticesIndices() override;
};

#endif