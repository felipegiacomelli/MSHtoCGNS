#ifndef CGNS_FILE_2D_HPP
#define CGNS_FILE_2D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class CgnsCreator2D : public CgnsCreator {
	public:
		CgnsCreator2D(GridDataShared gridData, const std::string& folderPath);

		~CgnsCreator2D() = default;

	private:
		void writeCoordinates() override;
		void writeRegions() override;
		void writeBoundaries() override;
};

#endif