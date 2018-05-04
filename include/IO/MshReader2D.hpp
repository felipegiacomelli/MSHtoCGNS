#ifndef MSH_READER_2D_HPP
#define MSH_READER_2D_HPP

#include <IO/MshReader.hpp>

class MshReader2D : public MshReader {
	public:
		MshReader2D(const std::string& filePath);

		~MshReader2D() = default;

	private:
		void readPhysicalEntities() override;
		void determineNumberOfFacets() override;
		void addRegions() override;
		void addBoundaries() override;
		void defineBoundaryVertices() override;
};

#endif