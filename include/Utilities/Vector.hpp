#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

template<typename T, typename U>
T* determine_array_1d(const std::vector<U>&);

template<typename T, typename U>
T* determine_array_1d(const std::vector<std::vector<U>>&);

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