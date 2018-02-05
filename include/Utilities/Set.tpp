template<typename T, typename U>
T* determine_array_1d(const std::set<U>& a) {
	T* b = new T[a.size()];
	unsigned int j = 0;
	for (auto i = a.cbegin(); i != a.cend(); i++) {
		b[j] = static_cast<T>(*i);
		j++;
	}
	return b;
}

template<typename T>
void print(const std::set<T>& a, std::string&& message) {
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "\t" << message << std::endl;
	for (auto i = a.cbegin(); i != a.cend(); i++) std::cout << "\t" << *i;		
	std::cout << std::endl;
}

template<typename T>
void output(const std::set<T>& a, std::ofstream& file) {
	file << std::fixed << std::setprecision(3);
	for (auto i = a.cbegin(); i != a.cend(); i ++) file << "\t" << std::setw(6) << std::right << *i;
	file << std::endl;
}