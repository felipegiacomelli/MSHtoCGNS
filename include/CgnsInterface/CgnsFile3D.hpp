#ifndef CGNS_FILE_3D_HPP	
#define CGNS_FILE_3D_HPP

#include <iostream>
#include <string>
#include <vector>

#include <cgnslib.h>

#include <BoostInterface/Filesystem.hpp>
#include <Utilities/Vector.hpp>
#include <Grid/GridData.hpp>
#include <CgnsInterface/CgnsFile.hpp>

class CgnsFile3D : public CgnsFile {
	public:
		CgnsFile3D() = default;
		CgnsFile3D(const GridData&, const std::string&);

		~CgnsFile3D();

	private:
		void writeBase() override;
		void writeZone() override;
		void writeCoordinates() override;
		void writeSections() override;
		void writeBoundaryConditions() override;
};

#endif