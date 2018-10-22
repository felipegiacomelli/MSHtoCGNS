#ifndef CGNS_CREATOR_2D_HPP
#define CGNS_CREATOR_2D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class CgnsCreator2D : public CgnsCreator {
	public:
		CgnsCreator2D(boost::shared_ptr<GridData> gridData, std::string folderPath);

	private:
		void checkDimension() override;
		void setDimensions() override;
		void writeCoordinates() override;
		void buildGlobalConnectivities() override;
		void writeRegions() override;
		void writeBoundaries() override;
};

#endif