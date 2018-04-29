#include <IO/MshReader.hpp>

MshReader::MshReader(const std::string& filePath) : filePath(filePath) {
	this->checkFile();
	this->gridData = MakeShared<GridData>();
}

void MshReader::checkFile() {
    boost::filesystem::path input(this->filePath);
	if (!boost::filesystem::exists(input.parent_path())) 
		throw std::runtime_error("MshReader: The parent path does not exist");
	
	if (!boost::filesystem::exists(this->filePath)) 
		throw std::runtime_error("MshReader: There is no file in the given path");

	if (input.extension() != ".msh") 
		throw std::runtime_error("MshReader: The file extension is not .msh");

	this->file = std::ifstream(this->filePath.c_str());
}

void MshReader::readNodes() {
	int numberOfVertices, temporary;
	this->file.seekg(0, std::ios::beg); 
	while (strcmp(this->buffer, "$Nodes") && !this->file.eof()) 
		this->file >> this->buffer;
	if (this->file.eof()) 
		throw std::runtime_error("MshReader: There is no Node data in the grid file");
	
	this->file >> numberOfVertices;
	this->gridData->coordinates.resize(numberOfVertices, std::vector<double>(3));
	for (int i = 0; i < numberOfVertices; i++) 
		this->file >> temporary >> this->gridData->coordinates[i][0] >> this->gridData->coordinates[i][1] >> this->gridData->coordinates[i][2];
}

void MshReader::readConnectivities() {
	int numberOfElements;
	this->file.seekg(0, std::ios::beg);
	while (strcmp(this->buffer, "$Elements") && !this->file.eof()) 
		this->file >> this->buffer;
	if (this->file.eof()) 
		this->file.clear(); 
	else {
		this->file >> numberOfElements;
		for (int i = 0; i < numberOfElements+1; i++) {
			std::string line;
			std::getline(this->file, line);
			std::istringstream stream(line);
			std::vector<int> connectivity;
			int value;
			while (stream >> value) {
				value--;
				connectivity.push_back(value);
			}
			this->connectivities.emplace_back(std::move(connectivity));
		}
	}
	this->connectivities.erase(this->connectivities.begin());
	
	if (connectivities[0][2] != 1) 
		throw std::runtime_error("MshReader: Elements must have exactly 2 tags");

	for (auto i = this->connectivities.begin(); i < this->connectivities.end(); i++) {
		i->erase(i->begin());
		i->erase(i->begin()+1);
		i->erase(i->begin()+2);
	}
}