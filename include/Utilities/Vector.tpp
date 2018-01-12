/*
// Conversion
*/
template<typename T>
T* determine_array_1d(const std::vector<T>& a) {
	T* b = new T[a.size()];

	for (auto i = 0; i < a.size(); i++) {
		b[i] = a[i];
	}

	return b;
}

template<typename T>
T* determine_array_1d(const std::vector<std::vector<T>>& a) {
	auto rows = a.size();
	auto columns = a[0].size();

	T* b = new T[rows*columns];

	for (auto i = 0; i < rows; i++) {
		for (auto j = 0; j < columns; j++) {
			b[i*columns + j] = a[i][j];
		}
	}

	return b;
}

template<typename T>
T** determine_array_2d(const std::vector<std::vector<T>>& a) {
	T** b = new T*[a.size()];
	for (auto i = 0; i < a.size(); i++) {
		b[i] = new T[a[0].size()];
	}

	for (auto i = 0; i < a.size(); i++) {
		for (auto j = 0; j < a[0].size(); j++) {
			b[i][j] = a[i][j];
		}
	}

	return b;
}

template<typename T>
std::vector<T> make_1d(const std::vector<std::vector<T>>& a) {
	auto rows = a.size(); 
	auto columns = a[0].size();
	std::vector<T> b(rows*columns);
	for (auto i = 0; i < rows; i++) {
		for (auto j = 0; j < columns; j++) {
			b[i*columns + j] = a[i][j];
		}
	}	
	return b;
}

template<typename T, typename U>
std::vector<std::vector<T>> make_2d(const std::vector<T>& a, const U& rows, const U& columns) {
	std::vector< std::vector<T> > b(rows, std::vector<T>(columns));
	for (auto i = 0; i < rows; i++) {
		for (auto j = 0; j < columns; j++) {
			b[i][j] = a[i*columns + j];
		}
	}	
	return b;
}

/*
// Output
*/
template<typename T>
void print(const std::vector<T>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl;
	for (auto i = a.cbegin(); i != a.cend(); i++) {
		std::cout << "\t" << *i;
	}
	std::cout << std::endl;
}

template<typename T>
void print(const std::vector<std::vector<T>>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl;
	for (auto i = a.cbegin(); i != a.cend(); i++) {
		for (auto j = i->cbegin(); j != i->cend(); j++) {
			std::cout << "\t" << *j;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<typename T>
void output(const std::vector<T>& a, std::string&& path) {
	std::ofstream file;
	file.open(std::string(path).c_str());
	for (auto i = a.cbegin(); i != a.cend()-1; i++) {
		file << *i << ", ";
	}
	file << *(a.end()-1);
	file.close();
}