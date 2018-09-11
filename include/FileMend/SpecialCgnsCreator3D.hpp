#ifndef SPECIAL_CGNS_CREATOR_3D_HPP
#define SPECIAL_CGNS_CREATOR_3D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class SpecialCgnsCreator3D : public CgnsCreator {
	public:
		SpecialCgnsCreator3D(GridDataShared gridData, std::string folderPath);

	private:
		void checkDimension() override;
		void writeSections() override;
		void writeCoordinates() override;
		void buildElementConnectivities() override;
		void writeRegions() override;
		void buildFacetConnectivities() override;
		void writeBoundaries() override;
		void buildWellConnectivities();
		void writeWells();

		int numberOfElements;
		int numberOfFacets;
		std::vector<std::vector<int>> wellConnectivities;
};

#endif