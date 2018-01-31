#ifndef SET_HPP
#define SET_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <set>
#include <fstream>

template<typename T, typename U>
T* determine_array_1d(const std::set<U>&);

template<typename T>
void print(const std::set<T>&, std::string&&);

template<typename T>
void output(const std::set<T>&, std::ofstream&);

#include "Set.tpp"

#endif