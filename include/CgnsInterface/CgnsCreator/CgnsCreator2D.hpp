#ifndef CGNS_INTERFACE_CGNS_FILE_2D_HPP
#define CGNS_INTERFACE_CGNS_FILE_2D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class CgnsCreator2D : public CgnsCreator {
	public:
		CgnsCreator2D(GridDataShared gridData, const std::string& folderPath);

	private:
		void checkDimension() override;
		void writeCoordinates() override;
		void buildElementConnectivities() override;
		void writeRegions() override;
		void buildFacetConnectivities() override;
		void writeBoundaries() override;
};

#endif