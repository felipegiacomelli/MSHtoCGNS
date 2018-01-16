#ifndef UTILITIES_VECTOR_HPP
#define UTILITIES_VECTOR_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Conversion
template<typename T>
T* determine_array_1d(const std::vector<T>&);

template<typename T>
T* determine_array_1d(const std::vector<std::vector<T>>&);

template<typename T>
T** determine_array_2d(const std::vector<std::vector<T>>&);

template<typename T>
std::vector<T> make_1d(const std::vector<std::vector<T>>&);

template<typename T, typename U>
std::vector<std::vector<T>> make_2d(const std::vector<std::vector<T>>&, const U&, const U&);

// Output
template<typename T>
void print(const std::vector<T>&, std::string&&);

template<typename T>
void print(const std::vector<std::vector<T>>&, std::string&&);

template<typename T>
void output(const std::vector<T>&, std::ofstream&);

template<typename T>
void output(const std::vector<std::vector<T>>&, std::ofstream&);

#include "Vector.tpp"

#endif