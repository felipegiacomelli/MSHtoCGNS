#ifndef MSH_READER_HPP
#define MSH_READER_HPP

#include <BoostInterface/Filesystem.hpp>
#include <Grid/GridData.hpp>
#include <Utilities/Vector.hpp>
#include <Utilities/Set.hpp>

#include <string>
#include <fstream>

enum class MshElement {BAR_2 = 1};

class MshReader {
	public:
		MshReader() = default;
		MshReader(const std::string&);

		GridData getGridData() const;

		~MshReader();

	protected:
		void readNodes();
		void readElements();
		virtual void readPhysicalEntities() = 0;
		virtual void addElements() = 0;
		virtual void defineBoundaryVerticesIndices() = 0;

		std::string filePath;
		std::ifstream file;
		char* buffer;
		int numberOfPhysicalEntities, geometryNumber;
		std::vector<std::vector<int>> elements, physicalEntitiesElementIndices;
		GridData gridData;
};

#endif