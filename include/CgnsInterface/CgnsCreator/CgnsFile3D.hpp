#ifndef CGNS_FILE_3D_HPP	
#define CGNS_FILE_3D_HPP

#include <CgnsInterface/CgnsCreator/CgnsFile.hpp>

class CgnsFile3D : public CgnsFile {
	public:
		CgnsFile3D() = default;
		CgnsFile3D(GridDataShared, const std::string&);

		~CgnsFile3D() = default;

	private:
		void defineGeometryType(); 
		void setupFile() override;
		void writeCoordinates() override;
		void writeSections() override;

		int geometry, boundary;
};

#endif