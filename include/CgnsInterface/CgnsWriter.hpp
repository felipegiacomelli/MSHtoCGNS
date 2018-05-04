#ifndef CGNS_WRITER_HPP
#define CGNS_WRITER_HPP

#include <BoostInterface/Filesystem.hpp>

class CgnsWriter {
	public:
		CgnsWriter() = default;
		CgnsWriter(const std::string& filePath);

		void writePermanentSolution();
		void writePermanentField(const std::vector<double>& fieldValues, const std::string& fieldName);

		void writeTimeStep(const double& timeInstant);
		void writeTransientField(const std::vector<double>& fieldValues, const std::string& fieldName);

		void finalizeTransient();

		~CgnsWriter();

	private:
		void initialize();
		void checkFile();
		void readBase();
		void readZone();

		std::string filePath;
		int fileIndex, baseIndex, zoneIndex, solutionIndex, fieldIndex;
		std::vector<int> solutionIndices, fieldsIndices;
		std::vector<double> timeInstants;
		bool isFinalized;
};

#endif