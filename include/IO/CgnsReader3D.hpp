#ifndef CGNS_READER_3D_HPP
#define CGNS_READER_3D_HPP

#include <IO/CgnsReader.hpp>

class CgnsReader3D : public CgnsReader {
	public:
		CgnsReader3D() = default;
		CgnsReader3D(const std::string&);

	private:
		void readCoordinates() override;
		void readSections() override;
		void readBoundaries() override;
};

#endif