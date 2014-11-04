/*
 * Helper.h
 *
 *  Created on: Nov 2, 2014
 *      Author: agustin
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <vector>

#include <common/debug/DebugUtil.h>


namespace common {

namespace Helper {


// @brief Helper method used to remove a specific element from the vector
//        and swap it with the last one.
// @param index      The element index to be removed
// @return true if exists and was removed | false otherwise
//
template <typename T>
inline void
remAndSwapElem(int index, std::vector<T>& vec)
{
    ASSERT(index < vec.size());
    vec[index] = vec.back();
    vec.pop_back();
}

// @brief Search an element in a vector and return the index or < 0 if not found
//
template <typename T>
inline int
findElemVec(const T& elem, const std::vector<T>& vec)
{
    for (int i = 0; i < static_cast<int>(vec.size()); ++i) {
        if (elem == vec[i]) return i;
    }
    return -1;
}


// @brief Helper method used to remove a specific element from the vector
//        and swap it with the last one.
// @param elem      The elem to be removed
// @return true if exists and was removed | false otherwise
//
template<typename T>
inline bool
remAndSwapIfExists(const T& elem, std::vector<T>& vec)
{
    const int index = findElemVec(elem, vec);
    if (index < 0) return false;

    remAndSwapElem(index, vec);
    return true;
}


}

} /* namespace common */

#endif /* HELPER_H_ */
