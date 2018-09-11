#ifndef SPECIAL_CGNS_READER_3D_HPP
#define SPECIAL_CGNS_READER_3D_HPP

#include <CgnsInterface/CgnsReader.hpp>

class SpecialCgnsReader3D : public CgnsReader {
	public:
		SpecialCgnsReader3D(std::string filePath);

	private:
		void readCoordinates() override;
		void readSections() override;
		void addRegion(std::string&& name, int elementStart, int numberOfElements);
		void addBoundary(std::string&& name, int elementStart, int numberOfElements);
};

#endif