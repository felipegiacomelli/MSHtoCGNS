#ifndef CGNS_CREATOR_3D_HPP
#define CGNS_CREATOR_3D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class CgnsCreator3D : public CgnsCreator {
	public:
		CgnsCreator3D(GridDataShared gridData, std::string folderPath);

	private:
		void checkDimension() override;
		void setDimensions() override;
		void writeCoordinates() override;
		void buildGlobalConnectivities() override;
		void writeSections() override;
		void writeRegions() override;
		void writeBoundaries() override;
		void writeWells();
};

#endif