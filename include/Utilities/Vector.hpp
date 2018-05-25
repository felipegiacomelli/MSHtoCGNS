#ifndef UTILITIES_VECTOR_HPP
#define UTILITIES_VECTOR_HPP

#include <vector>

template<class InputIt, class OutputIt>
void append(InputIt cbegin, InputIt cend, OutputIt output) {
	for (auto i = cbegin; i != cend; i++)
		for (auto j = i->cbegin(); j != i->cend(); j++)
			output = *j;
}

#endif