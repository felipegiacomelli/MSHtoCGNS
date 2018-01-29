#ifndef CGNS_READER_2D_HPP
#define CGNS_READER_2D_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <IO/CgnsReader.hpp>

#include <string>
#include <fstream>

class CgnsReader2D : public CgnsReader {
	public:
		CgnsReader2D() = default;
		CgnsReader2D(const std::string&);

	private:
		void readNodes() override;
		void readElements() override;
};

#endif