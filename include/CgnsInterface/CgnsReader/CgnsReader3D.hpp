#ifndef CGNS_READER_3D_HPP
#define CGNS_READER_3D_HPP

#include <CgnsInterface/CgnsReader.hpp>

class CgnsReader3D : public CgnsReader {
	public:
		CgnsReader3D(const std::string& filePath);

	private:
		void readCoordinates() override;
		void readSections() override;
};

#endif