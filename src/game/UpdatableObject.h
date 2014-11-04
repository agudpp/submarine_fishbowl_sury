/*
 * UpdatableObject.h
 *
 *  Created on: Oct 18, 2014
 *      Author: agustin
 */

#ifndef UPDATABLEOBJECT_H_
#define UPDATABLEOBJECT_H_


#include <vector>

#include <common/debug/DebugUtil.h>


namespace game {

class UpdatableObject
{
public:
    // @brief This shouldn't be modified by the class nor any other except
    //        the container of the class (game handler probably)
    unsigned int _index;
};


// @brief Here we will define a class that will be used to hold and update
//        objects of type XX that will be defined at compile time and without
//        performance issues. We will specify the functors (should be functors
//        and not functions) and with -O1 the compiler can optimize to have
//        no performance issues (if we use inline methods)
// @param ObjType is the object type we will use and should contain the method
//                update(timeframe) and also the _index attribute for fast
//                checking.
//                We will hold ObjType pointers only
// @param OnRemFunctor is the method used to be called when we need to remove
//                     an object from the list.
//

template<typename ObjType, typename OnRemFunctor>
class UpdatableObjHandler {
public:
    UpdatableObjHandler(){}
    ~UpdatableObjHandler(){}

    // @brief check if an object already exists or not
    //
    inline bool
    exists(const ObjType* obj) const
    {
        return obj != 0 &&
               obj->_index < m_objects.size() &&
               m_objects[obj->_index] == obj;
    }

    // @brief add an element to be updated
    //
    inline void
    add(ObjType* obj)
    {
        if (obj == 0 || exists(obj)) {
            return;
        }
        // add the object if not
        obj->_index = m_objects.size();
        m_objects.push_back(obj);
    }

    // @brief remove an element and call the OnRemFunctor
    //
    inline void
    remove(ObjType* obj)
    {
        if (!exists(obj)) {
            return;
        }
        // swap with last and remove it, then call OnRemFunctor
        m_objects[obj->_index] = m_objects.back();
        m_objects.pop_back();
        obj->_index = -1;
        OnRemFunctor rem;
        rem(obj);
    }

    // @brief update all the elements and remove those who return false
    //
    inline void
    update(float timeFrame)
    {
        unsigned int count = m_objects.size();
        for (unsigned int i = 0; i < count; ++i) {
            if (!m_objects[i]->update(timeFrame)) {
                remove(m_objects[i]);
                --i;
                --count;
            }
        }
    }

private:
    std::vector<ObjType> m_objects;
};

} /* namespace game */

#endif /* UPDATABLEOBJECT_H_ */
