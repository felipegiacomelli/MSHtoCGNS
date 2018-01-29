#ifndef MSH_READER_3D_HPP
#define MSH_READER_3D_HPP

#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <IO/MshReader.hpp>

#include <string>
#include <fstream>

class MshReader3D : public MshReader {
	public:
		MshReader3D() = default;
		MshReader3D(const std::string&);

	private:
		void readPhysicalEntities() override;
		void addElements() override;
};

#endif