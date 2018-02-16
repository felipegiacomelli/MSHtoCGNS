template<typename T, typename U>
T* determine_array_1d(const std::vector<U>& a) {
	T* b = new T[a.size()];
	for (unsigned i = 0; i < a.size(); i++) b[i] = static_cast<T>(a[i]);
	return b;
}

template<typename T, typename U>
T* determine_array_1d(const std::vector<std::vector<U>>& a) {
	unsigned rows = a.size();
	unsigned columns = a[0].size();
	T* b = new T[rows*columns];
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < columns; j++) {
			b[i*columns + j] = static_cast<T>(a[i][j]);
		}
	}
	return b;
}

template<typename T>
void print(const std::vector<T>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl;
	for (auto i = a.cbegin(); i != a.cend(); i++) std::cout << "\t" << *i;
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
void output(const std::vector<T>& a, std::ofstream& file) {
	file << std::fixed << std::setprecision(3);
	for (auto i = a.cbegin(); i != a.cend(); i ++) file << "\t" << std::setw(6) << std::right << *i;		
	file << std::endl;
}

template<typename T>
void output(const std::vector<std::vector<T>>& a, std::ofstream& file) {
	file << std::fixed << std::setprecision(3) << std::right;
	for (auto i = a.cbegin(); i != a.cend(); i ++) {
		for (auto j = i->cbegin(); j != i->cend(); j ++) {
			file << "\t" << std::setw(6) << std::right << *j;
		}
		file << std::endl;
	}
}