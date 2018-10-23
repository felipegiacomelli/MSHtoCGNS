#ifndef SPECIAL_CGNS_READER_3D_HPP
#define SPECIAL_CGNS_READER_3D_HPP

#include <CgnsInterface/CgnsReader/CgnsReader3D.hpp>

class SpecialCgnsReader3D : public CgnsReader3D {
	public:
		SpecialCgnsReader3D(std::string filePath);

	private:
		void readSections() override;
		void addWell(std::string&& name, int elementStart, int elementEnd) = delete;
};

#endif