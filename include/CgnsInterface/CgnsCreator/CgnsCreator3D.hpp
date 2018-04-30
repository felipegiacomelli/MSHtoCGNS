#ifndef CGNS_FILE_3D_HPP	
#define CGNS_FILE_3D_HPP

#include <CgnsInterface/CgnsCreator/CgnsCreator.hpp>

class CgnsCreator3D : public CgnsCreator {
	public:
		CgnsCreator3D() = default;
		CgnsCreator3D(GridDataShared, const std::string&);

		~CgnsCreator3D() = default;

	private:
		void defineGeometryType(); 
		void setupFile() override;
		void writeCoordinates() override;
		void writeRegions() override;
		void writeBoundaries() override;

		int geometry, boundary;
};

#endif