#ifndef MSH_READER_2D_HPP
#define MSH_READER_2D_HPP

#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <IO/MshReader.hpp>

#include <string>
#include <fstream>

class MshReader2D : public MshReader {
	public:
		MshReader2D() = default;
		MshReader2D(const std::string&);

	private:
		void readPhysicalEntities() override;
		void addElements() override;
};

#endif