#ifndef __UTILITIES_ALGORITHM_HPP__
#define __UTILITIES_ALGORITHM_HPP__

#include <algorithm>

template<class InputIt, class T>
bool hasElement(InputIt cbegin, InputIt cend, const T& element) {
    return std::find(cbegin, cend, element) != cend;
}

template<class InputIt>
bool hasElements(InputIt setBegin, InputIt setEnd, InputIt begin, InputIt end) {
    bool close = true;

    for (auto element = begin; element != end; ++element)
        close &= std::find(setBegin, setEnd, *element) != setEnd;

    return close;
}

#endif
