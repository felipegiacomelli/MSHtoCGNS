#ifndef CGNS_INTERFACE_CGNS_READER_2D_HPP
#define CGNS_INTERFACE_CGNS_READER_2D_HPP

#include <CgnsInterface/CgnsReader.hpp>

class CgnsReader2D : public CgnsReader {
	public:
		CgnsReader2D(std::string filePath);

	private:
		void readCoordinates() override;
		void readSections() override;
};

#endif