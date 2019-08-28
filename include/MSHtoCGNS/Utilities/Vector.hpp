#ifndef __UTILITIES_VECTOR_HPP__
#define __UTILITIES_VECTOR_HPP__

#include <vector>

template<class InputIt, class OutputIt>
void append(InputIt cbegin, InputIt cend, OutputIt output) {
    for (auto i = cbegin; i != cend; ++i)
        for (auto j = i->cbegin(); j != i->cend(); ++j)
            output = *j;
}

template<class InputIt, class OutputIt>
void append(int, InputIt cbegin, InputIt cend, OutputIt output) {
    for (auto i = cbegin; i != cend; ++i)
        for (auto j = i->cbegin() + 1; j != i->cend(); ++j)
            output = *j;
}

template<class T, typename U>
void append(const std::vector<T>& connectivities, std::vector<std::vector<U>>& global) {
    for (const auto& connectivity : connectivities)
        global.emplace_back(connectivity.cbegin(), connectivity.cend());
}

template<typename T, typename U>
void append(int elementType, const std::vector<T>& connectivities, std::vector<std::vector<U>>& global) {
    for (const auto& connectivity : connectivities) {
        auto& element = global.emplace_back(std::vector<U>{elementType});
        for (const auto& index : connectivity) {
            element.emplace_back(index);
        }
    }
}

#endif
