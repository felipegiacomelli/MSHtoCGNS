#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

template<class InputIt, class T>
bool hasElement(InputIt cbegin, InputIt cend, const T& element) {
    return std::find(cbegin, cend, element) != cend;
}

template<class InputIt>
bool hasElements(InputIt setBegin, InputIt setEnd, InputIt elementBegin, InputIt elementEnd) {
    bool close = true;

    for (auto element = elementBegin; element != elementEnd; element++)
        close &= std::find(setBegin, setEnd, *element) != setEnd;

    return close;
}

#endif