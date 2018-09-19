#ifndef MULTIPLE_BASES_CGNS_CREATOR_3D_HPP
#define MULTIPLE_BASES_CGNS_CREATOR_3D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class MultipleBasesCgnsCreator3D : public CgnsCreator {
	public:
		MultipleBasesCgnsCreator3D(std::vector<GridDataShared> gridDatas, std::vector<std::string> zoneNames, std::string folderPath);

	private:
		void initialize();
		void checkDimension() override;
		void setDimensions() override;
		void writeSections() override;
		void writeCoordinates() override;
		void buildElementConnectivities() override;
		void writeRegions() override;
		void buildFacetConnectivities() override;
		void writeBoundaries() override;
		void buildWellConnectivities();
		void writeWells();

		std::vector<GridDataShared> gridDatas;
		std::vector<std::string> zoneNames;
		bool firstCall;

		int numberOfElements;
		int numberOfFacets;
		std::vector<std::vector<int>> wellConnectivities;
};

#endif