#ifndef MULTIPLE_BASES_CGNS_CREATOR_3D_HPP
#define MULTIPLE_BASES_CGNS_CREATOR_3D_HPP

#include <CgnsInterface/CgnsCreator.hpp>

class MultipleBasesCgnsCreator3D : public CgnsCreator {
	public:
		MultipleBasesCgnsCreator3D(std::vector<boost::shared_ptr<GridData>> gridDatas, std::vector<std::string> baseNames, std::string folderPath);

	private:
		void initialize();
		void checkDimension() override;
		void setDimensions() override;
		void writeCoordinates() override;
		void buildGlobalConnectivities() override;
		void writeSections() override;
		void writeRegions() override;
		void writeBoundaries() override;
		void writeWells();

		std::vector<boost::shared_ptr<GridData>> gridDatas;
		std::vector<std::string> baseNames;
		bool firstCall;
};

#endif